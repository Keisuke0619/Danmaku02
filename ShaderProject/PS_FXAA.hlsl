// 頂点シェーダから受け取るためのデータを定義
struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
};

// テクスチャを受け取る
Texture2D tex : register(t0);
SamplerState samp : register(s0); // テクスチャの繰り返し設定

// 輝度の計算
float Luminance(float3 _col)
{
    return _col.r * 0.3f + _col.g * 0.6f + _col.b * 0.1f;
}

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = tex.Sample(samp, pin.uv);
    // 差を定義
    float2 diff = float2(1.0f / 1280.0f, 1.0f / 720.0f);
    // 周辺色取得
    float lumi00 = Luminance(tex.Sample(samp, saturate(pin.uv + diff * float2( 0, 0))).rgb);
    float lumi01 = Luminance(tex.Sample(samp, saturate(pin.uv + diff * float2(-1,-1))).rgb);
    float lumi02 = Luminance(tex.Sample(samp, saturate(pin.uv + diff * float2( 1,-1))).rgb);
    float lumi03 = Luminance(tex.Sample(samp, saturate(pin.uv + diff * float2(-1, 1))).rgb);
    float lumi04 = Luminance(tex.Sample(samp, saturate(pin.uv + diff * float2( 1, 1))).rgb);
    // 一番明るい輝度と低い輝度を取得し、計算
    float lumiMax = max(max(lumi01, lumi02), max(lumi03, lumi04));
    float lumiMin = min(min(lumi01, lumi02), min(lumi03, lumi04));
    float lumiSub = max(lumiMax, lumi00) - min(lumiMin, lumi00);
    
    if (lumiSub < 0.25f)
    {
        return color;
    }
    float2 dir = float2(lumi02 - lumi03, lumi04 - lumi01);
    dir = normalize(dir);
    dir *= diff;
    
    // 色の混ぜ合わせ
    float4 col1 = tex.Sample(samp, pin.uv + dir);
    float4 col2 = tex.Sample(samp, pin.uv - dir);
    color = (col1 + col2) * 0.5f;
    return color;
}