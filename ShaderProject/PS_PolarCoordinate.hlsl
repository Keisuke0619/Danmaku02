struct PS_IN
{
	float4 pos : SV_POSITION0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

cbuffer Param : register(b0)
{
	float innerDistance;
	float outerDistance;
	float gageRate;
	float dummy;
}
Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 col = float4(0.0f,0.0f,0.0f,0.0f);
	// UVの値から、中心までの距離を求める
	float2 center = float2(0.5f, 0.5f);
	float2 offset = pin.uv - center;
	float r = length(offset);

	// 中心からの距離で表示するかしないか判定。ただしif文を使うとすべてのピクセルで計算することになるので、if文を使わずに表示の判定（計算）を行う。
	col.a = 1.0f - step(innerDistance, r);
	col.a *= step(outerDistance, r);

	// 極座標の角度を求める
	float pi = 3.141592f;
	float rad = atan2(offset.y, offset.x);
	rad -= pi * 0.5f;	// ゲージのスタート地点を変更
	rad /= pi;			// -3.14～3.14を-1～1に変更
	rad = rad * 0.5f + 0.5f;	// -1～1を0～1に変更
	rad = frac(rad);	// 小数点部分だけ取り出す
	// ゲージのヘリを表現
	float gage = rad;
	gage = 1.0f - step(gageRate, gage);
	float3 frontCol = float3(0.0f, 1.0f, 0.3f);
	float3 backCol = float3(0.3f, 0.3f, 0.3f);

	frontCol = tex.Sample(samp, float2(rad, r * 2));
	col.rgb = lerp(backCol, frontCol, gage);
	return col;
}