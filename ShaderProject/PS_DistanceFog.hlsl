struct PS_IN
{
	float4 pos : SV_POSITION0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	// send from pixel shader that world position
	float4 worldPos : POSITION0;
};

// ライトの情報
cbuffer Light : register(b0)
{
	// 拡散光(物体に直接あたる光)
	float4 lightColor;		// 光の色
	float4 lightAmbient;	// 環境光(周りに当たって跳ね返る光)
	float4 lightDir;		// 光の向き
}

// camera's data.
cbuffer Camera : register(b1)
{
	float4 cameraPos;
};

// Fog's data
cbuffer Fog:register(b2)
{
	float4 fogColor;
	float fogStart;		// fog's start dictance.
	float fogRange;
};

texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 col = tex.Sample(samp, pin.uv);

	// フォグの色を計算する地点までの距離
	float3 V = pin.worldPos.xyz - cameraPos.xyz;
	float dist = length(V);

	// フォグの色を変化させる係数の計算
	float fog = dist - fogStart;	// フォグの開始距離で引く
	fog /= fogRange;				// フォグの適用範囲で割る
	fog = saturate(fog);			// 0～1に

	// 元の色を計算
	float3 N = normalize(pin.normal);
	float3 L = normalize(lightDir.xyz);
	float diffuse = saturate(dot(N, -L));
	col.rgb *= diffuse + lightAmbient.rgb;

	// 係数から元の色とフォグの色をブレンドさせる
	col.rgb = lerp(col.rgb, fogColor.rgb, fog);

	return col;
}