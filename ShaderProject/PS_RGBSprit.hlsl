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
    
    col = tex.Sample(samp, pin.uv);
    
    float2 center = float2(0.5f, 0.5f);
    float2 vec = pin.uv - center;
    float distance = length(vec);
    distance = pow(distance, 1.5);
    distance *= 10;
    float2 diff = float2(distance / 1280.0f, 0);
    col.r = tex.Sample(samp, pin.uv + diff).r;
    col.g = tex.Sample(samp, pin.uv - diff).g;
    return col;
}