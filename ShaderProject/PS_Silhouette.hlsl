struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    float4 screenPos : POSITION0;
};

SamplerState samp : register(s0);
texture2D depthTex : register(t0);
texture2D baseTex : register(t1);
texture2D maskTex : register(t2);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = baseTex.Sample(samp, pin.uv);
    float4 mask = maskTex.Sample(samp, pin.uv);
    // 頂点座標から画面上のUV座標を取得
    float2 screenUV = pin.screenPos.xy / pin.screenPos.w;
    screenUV = screenUV * 0.5f + 0.5f;
    screenUV.y = 1.0f - screenUV.y;
    // 頂点座標より、カメラからの奥行きを取得
    float objDepth = pin.screenPos.z / pin.screenPos.w;
    
    float texDepth = depthTex.Sample(samp, screenUV).r;
    mask.rgb *= 1 - step(texDepth, objDepth);
    col.rgb -= mask.rgb;
    return col;
}