struct PS_IN
{
	float4 pos : SV_POSITION0;
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
	float dissolveRate;		// 消す割合（０表示 １全部消す）
	float dissolveRange;	// 変化する範囲
	float2 d;
};

Texture2D tex : register(t0);
Texture2D dissolve : register(t1);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 col = tex.Sample(samp, pin.uv);

	float3 N = normalize(pin.normal);
	float3 L = normalize(-lightDir.xyz);

	float diffuse = saturate(dot(N, L));
	col.rgb *= saturate(diffuse + lightAmbient);

	float value = dissolve.Sample(samp, pin.uv).r;
#if 0	// stepで真っ二つにわける
	col.a = step(dissolveRate, value);
#else
	// 変化範囲も考慮した全体の変化量
	float len = 1.0f - dissolveRange;
	// 変化範囲に合わせてしきい値を補正
	value += dissolveRange;
	// 基準値（Rate）が０～１ではなく、０～１＋変化範囲となるように変更
	float rate = dissolveRate / len;
	// 蒸気を踏まえて、０の時は不透明、１の時にはすべて透明になるように計算
	col.a = saturate((value - rate) / dissolveRange);
#endif
	return col;
}