struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

sampler samp : register(s0);
Texture2D tex : register(t0);
cbuffer Param : register(b0)
{
    float2 texSize;
    float2 blurDir;
}
float4 main(PS_IN pin) : SV_TARGET
{
    const float value = 1.0f / 9.0f;
    const float filter[9] =
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    float2 uvOffset = 1.0f / texSize;
    uvOffset *= blurDir;
    float2 uv = pin.uv - uvOffset * 4;
    float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 9; i++)
    {
        color += tex.Sample(samp, uv) * filter[i] * value;
        uv += uvOffset;
    }
    
    return color;
}