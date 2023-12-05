struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

sampler samp : register(s0);
Texture2D albedoTex : register(t0);
Texture2D depthTex : register(t1);
Texture2D blurTex : register(t2);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    // 各情報を取得
    float4 albedo = albedoTex.Sample(samp, pin.uv);
    float depth = depthTex.Sample(samp, pin.uv).r;
    float4 blur = blurTex.Sample(samp, pin.uv);

    float focus = 5.0f;     // カメラからピントがあう位置までの距離
    float DofRange = 5.0f;  // 被写界深度でぼかしの画像に変換する距離
    
    float centerDist = abs(depth - focus);          // フォーカス位置からの距離
    float rate = saturate(centerDist / DofRange);   // 正規化
    
    color = lerp(albedo, blur, rate);
    
    return color;
}