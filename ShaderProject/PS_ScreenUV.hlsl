//０５１５授業内容後で見せるよ
struct PS_IN
{
	float4 pos:SV_POSITION;
	float2 uv:TEXCOORD0;
	float3 normal:NORMAL0;
	//スクリーン座標
	float4 screenPos:TEXCOORD1;
};

Texture2D tex : register(t0);
//一つのシェーダーで複数のテクスチャが利用できる
Texture2D space : register(t1);
SamplerState samp : register(s0);


float4 main(PS_IN pin) : SV_TARGET
{
	float4 color=float4(0.0f, 0.0f, 0.0f, 0.0f);
	//スクリーン座標をピクセルシェーダーで利用する時
	//xyzのパラメはｗで割らないと正しい値にならない
	//kwskはカメラの行列の中身をしっかり理解する必要がある
	color = space.Sample(samp, pin.screenPos.xy/ pin.screenPos.w);

	
	return color;

}