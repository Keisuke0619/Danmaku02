struct VS_IN
{
    float3 pos : POSITION0;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};
struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 wPos : POSITION0;
    float4 sPos : POSITION1;
};

cbuffer Matrix1 : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
}

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos = mul(vout.pos, world);
    vout.wPos = vout.pos;
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    vout.sPos = vout.pos;
    vout.uv = vin.uv;
    vout.normal = mul(vin.normal, (float3x3) world);
    return vout;
}
