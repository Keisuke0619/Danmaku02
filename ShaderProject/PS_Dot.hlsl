struct PS_IN
{
    float4 pos : SC_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    // 解像度を下げてドット絵のような表現
    const float size = 8;
    const float2 dotScreenSize = float2(1920.0f / size, 1080.0f / size);
    float2 i_uv = pin.uv * dotScreenSize;
    i_uv = floor(i_uv) + 0.5f;
    i_uv /= dotScreenSize;
    
    
    col = tex.Sample(samp, i_uv);
    return col;
}