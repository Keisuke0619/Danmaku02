// CPUから渡される頂点の情報を作成
// インプットレイアウト
struct VS_IN
{
	float3 pos : POSITION0;		// セマンティクス
	float3 normal : NORMAL0;	// データの用途を定義する
	float2 uv : TEXCOORD0;
};

// 頂点シェーダからピクセルシェーダへ送るデータの定義
struct VS_OUT
{
	float4 pos : SV_POSITION0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	// send for pixel shader that world position
	float4 worldPos : POSITION0;
};

// 定数バッファ
// CPUとGPUでやり取りする調停外のデータを格納する先
cbuffer WVP : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
}

VS_OUT main(VS_IN vin)
{
	VS_OUT vout;
	// ローカル座標をスクリーン座標へ変換
	vout.pos = float4(vin.pos, 1.0f);	// ローカル座標
	vout.pos = mul(vout.pos, world);	// ワールド座標
	// copy to vout for send to pixel shader.
	vout.worldPos = vout.pos;
	vout.pos = mul(vout.pos, view);		// ビュー座標
	vout.pos = mul(vout.pos, proj);		// プロジェクション座標

	// ピクセルシェーダで利用するUV座標を渡す
	vout.uv = vin.uv;
	// ピクセルシェーダで利用する法線を渡す
	// 法線はオブジェクトの開店に合わせって回転させないとおかしな見た目になる。
	// ワールド行列には移動成分も含まれるが、移動を法線に適用してもおかしくなるので、回転成分のみの行列となるように縮小して計算する。
	vout.normal = mul(vin.normal, (float3x3)world);

	return vout;
}