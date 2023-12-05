struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0; // モデルの法線
};

// ライトの情報
cbuffer Light : register(b0)
{
    float4 lightColor; // 光の色
    float4 lightAmbient; // 環境光(周りに当たって跳ね返る光)
    float4 lightDir; // 光の向き
}

// テクスチャ
Texture2D tex : register(t0);
Texture2D rampTex : register(t1);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

    float3 N = normalize(pin.normal);
    float3 L = normalize(lightDir.xyz);

    L = -L;

    float diffuse = saturate(dot(N, L)); // dot:内積計算
#if 1
    float2 rampUV = float2(diffuse * 0.98f + 0.01f, 0.5f);
    diffuse = rampTex.Sample(samp, rampUV).r;
#else
    diffuse *= 2.0f;
    diffuse += 0.5f;
    diffuse = floor(diffuse);
    diffuse /= 2.0f;
#endif
	// テクスチャの色も考慮して陰を付ける
    float4 texColor = tex.Sample(samp, pin.uv);
    color.rgb = texColor.rgb * (diffuse + lightAmbient.rgb);
    
    return color;
}