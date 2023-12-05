struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;	// モデルの法線
	// pixel's world position, passed by vertex shader.
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

// テクスチャ
Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 col =  tex.Sample(samp, pin.uv);
	// 逆光かどうかを計算
	float3 L = normalize(lightDir.xyz);
	float3 V = normalize(cameraPos.xyz - pin.worldPos.xyz);
	float backLight = saturate(dot(L, V));
	// 縁の部分の計算（法線に対して視線が垂直に当たる箇所の計算）
	float3 N = normalize(pin.normal);

	//float edge = 1 - abs(dot(N, V));
	float edge = dot(N, V);
	edge = 1 - abs(edge);
	float lim = pow(backLight * edge, 6) * 5;
	// フォンシェーディング
	float diffuse = saturate(dot(N, -L));	// dot:内積計算
	col.rgb *= diffuse + lightAmbient;

	col.rgb += lim;
	return col;
}