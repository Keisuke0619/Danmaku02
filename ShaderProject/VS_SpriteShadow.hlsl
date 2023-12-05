struct VS_IN
{
    float3 pos : POSITION0;
    float2 uv : TEXCOORD0;
};
struct VS_OUT
{
    float4 pos : SV_POSITION;   // ゲーム内のカメラから見た画面位置
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    float4 screenPos : POSITION0;   // 光源のカメラから見た画面位置
};
cbuffer Matrix : register(b0) {
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};
cbuffer Param : register(b1)
{
    float2 offset;
    float2 size;
    float2 uvPos;
    float2 uvScale;
    float4 color;
};

cbuffer LightMat : register(b2)
{
    float4x4 lightWorld;
    float4x4 lightView;
    float4x4 lightProj;
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos.xy *= size;
    vout.pos.xy += offset;
    //float4 pos = vout.pos;
    vout.screenPos = vout.pos;
    // ゲームのカメラ
    vout.pos = mul(vout.pos, world);
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    // 光源のカメラ
    vout.screenPos = mul(vout.screenPos, lightWorld);
    vout.screenPos = mul(vout.screenPos, lightView);
    vout.screenPos = mul(vout.screenPos, lightProj);
    
    vout.uv = vin.uv;
    vout.uv *= uvScale;
    vout.uv += uvPos;
    vout.color = color;
    return vout;
}
