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
	float4 col = tex.Sample(samp, pin.uv);

	// 光のベクトルと法線から、反射するベクトル（反射ベクトル）を求める
	// 光のベクトルを法線方向に２回動かすと、反射ベクトルになる！
	float3 L = normalize(lightDir.xyz);
	float3 N = normalize(pin.normal.xyz);
	float3 R = L + 2 * N * -dot(L, N);
	// float3 R = reflect(L, N);
	// 反射地点（ワールド座標）からカメラに向かうベクトル
	float3 V = normalize(cameraPos.xyz - pin.worldPos.xyz);
	// 反射ベクトルと、カメラに向かうベクトルが同じ方向ならまぶしくなる。
	float specular = saturate(dot(R, V));
	// 塁上の計算を行うと、局所的に明るくなる。
	specular = pow(specular, 20);

	// 陰をつけるフォンシェーディングの要素を追加
	float diffuse = saturate(dot(N, -L));	// dot:内積計算
	col.rgb *= diffuse + lightAmbient.rgb;

	col.rgb += specular;

	return col;
}