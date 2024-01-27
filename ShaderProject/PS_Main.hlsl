struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

// ライトの情報
struct Light
{
    float4 color;
    float3 pos;
    float range;
};

cbuffer LightBuf : register(b0)
{
    Light lights[10];
}

sampler samp : register(s0);
Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D worldTex : register(t2);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = float4(0.2f, 0.2f, 0.2f, 1);
    float4 albedo = albedoTex.Sample(samp, pin.uv);
    float3 worldPos = worldTex.Sample(samp, pin.uv).rgb;
    float4 normal = normalTex.Sample(samp, pin.uv);
    if (normal.a < 0.001f)
    {
        discard;
    }
    float3 N = normalize(normal);
    for (int i = 0; i < 10; i++)
    {
        // ピクセルから点光源に向かうベクトル
        float3 toLight = lights[i].pos - worldPos.xyz;
        float3 V = normalize(toLight);
        float toLightDistance = length(toLight);
    
        float dotNV = saturate(dot(N, V));

        // 減衰処理
        float attenuation = saturate(1.0f - toLightDistance / lights[i].range);
        // 自然に
        attenuation = pow(attenuation, 2);
        col.rgb += lights[i].color * dotNV * attenuation;
    }
    return float4(albedo.rgb * col.rgb, 1.0f);

}