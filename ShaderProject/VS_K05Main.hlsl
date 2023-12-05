// CPUから渡される頂点の情報を作成
// インプットレイアウト
struct VS_IN
{
    float3 pos : POSITION0; // セマンティクス
    float3 normal : NORMAL0; // データの用途を定義する
    float2 uv : TEXCOORD0;
};

// 頂点シェーダからピクセルシェーダへ送るデータの定義
struct VS_OUT
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0; // 頂点側で計算した明るさの情報
    float2 uv : TEXCOORD0; // テクスチャはピクセルシェーダで張り付け
    float3 normal : NORMAL0;
    float3 wpos : POSITION0;
};

// 定数バッファ
// CPUとGPUでやり取りする調停外のデータを格納する先
cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

cbuffer Light : register(b1)
{
    float4 lightColor;
    float4 lightAmbient;
    float4 lightDir;
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
	// ローカル座標をスクリーン座標へ変換
    vout.pos = float4(vin.pos, 1.0f); // ローカル座標
    vout.pos = mul(vout.pos, world); // ワールド座標
    vout.wpos = vout.pos;
    vout.pos = mul(vout.pos, view); // ビュー座標
    vout.pos = mul(vout.pos, proj); // プロジェクション座標

	// 頂点シェーダで陰の計算
    float3 N = vin.normal;
    float3 L = lightDir.xyz;
	// 計算前に手を加える
    N = mul(N, (float3x3) world);
    L = -L;
	// 正規化
    N = normalize(N);
    L = normalize(L);
	// 陰計算
    float diffuse = saturate(dot(N, L));
	// 色決定
    vout.color = float4(diffuse + lightAmbient.rgb, 1.0f);
    vout.normal = N;
    vout.uv = vin.uv;

    return vout;
}