struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 screenPos : TEXCOORD1;
};
struct PS_OUT
{
    float4 albedo : SV_TARGET0;
    float4 depth : SV_TARGET1;
};
SamplerState samp : register(s0);
Texture2D tex : register(t0);
PS_OUT main(PS_IN pin)
{
    PS_OUT pout;
    
    pout.albedo = tex.Sample(samp, pin.uv);
    
    float depth = pin.screenPos.z;
    pout.depth = float4(depth, depth, depth, 1.0f);
    
    return pout;
}