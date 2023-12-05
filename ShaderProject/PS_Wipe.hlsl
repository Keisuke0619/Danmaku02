// 頂点シェーダから受け取るためのデータを定義
struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
};

// テクスチャを受け取る
Texture2D tex : register(t0);
SamplerState samp : register(s0); // テクスチャの繰り返し設定

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = tex.Sample(samp, pin.uv);

    float4 frame = float4(0.0f, 0.0f, 0.0f, 1.0f);
    frame.rg += step(sin((pin.uv.x + pin.uv.y) * 3.14f * 8), 0) * 0.2f;
    float2 uv = pin.uv;
    
    // 立体的に見せる陰の計算
    // 領域を／で区切って明るい部分と暗い部分を計算
    frame.rgb += step(uv.x + uv.y, 1.0f) * 0.3f;
    frame.rgb += step(uv.y - uv.x, 0.0f) * 0.2f;
    frame.rgb = saturate(frame.rgb);
    uv -= 0.5f;
    uv = abs(uv);
    float mask = 1 - step(max(uv.x, uv.y), 0.4f);
    
    color = lerp(color, frame, mask);
    
    return color;
}