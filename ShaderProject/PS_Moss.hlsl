struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
};

cbuffer Light : register(b0)
{
	// 拡散光(物体に直接あたる光)
	float4 lightColor;		// 光の色
	float4 lightAmbient;	// 環境光(周りに当たって跳ね返る光)
	float4 lightDir;		// 光の向き
};

cbuffer Param : register(b1)
{
	float cbMossRate;		// 消す割合（０表示 １全部消す）
	float cbMossRange;	// 変化する範囲
	float2 d;
};

Texture2D tex : register(t0);
Texture2D dissolve : register(t1);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
	color = tex.Sample(samp, pin.uv);

	// 法線
	float3 N = normalize(pin.normal);
	// 苔が生えるor雪が積もる方向
	float3 MossDir = normalize(float3(0, 1, 0.5f));
	MossDir = normalize(MossDir);

	// 苔の生え具合
	float moss = dot(N, MossDir);
	float mossStart = cbMossRate;	// 変化し始め
	float mossRange = cbMossRange;	// 変化範囲
	moss = moss - mossStart;
	moss = moss / mossRange;
	moss = saturate(moss);	// 0～1に収める
	moss *= 0.8f;

	// 苔を生やす
	float3 mossColor = float3(1, 1, 1);
	color.rgb = lerp(color.rgb, mossColor, moss);

	float3 L = -normalize(lightDir.xyz);
	float diffuse = saturate(dot(N, L));
	color.rgb *= diffuse + lightAmbient;

	return color;
}