struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 worldPos : POSITION0;
};
// 複数出力があるので構造体にまとめる。セマンティクスはSV_TARGET[n]で指定する
struct PS_OUT
{
    float4 albedo : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 world : SV_TARGET2;
};

Texture2D tex : register(t0);
sampler samp : register(s0);

PS_OUT main(PS_IN pin)
{
    PS_OUT pout;
    pout.albedo = tex.Sample(samp, pin.uv);
    pout.normal = float4(pin.normal, 1);
    pout.world = pin.worldPos;
    return pout;
}