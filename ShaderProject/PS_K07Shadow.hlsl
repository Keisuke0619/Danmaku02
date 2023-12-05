struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 screenPos : POSITION0;
};


SamplerState samp : register(s0);
texture2D tex : register(t0);
texture2D depthTex : register(t1);

cbuffer Light : register(b0)
{
	// 拡散光(物体に直接あたる光)
    float4 lightColor; // 光の色
    float4 lightAmbient; // 環境光(周りに当たって跳ね返る光)
    float4 lightDir; // 光の向き
}

float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = tex.Sample(samp, pin.uv);
    
    // 頂点座標から画面上のUV座標を取得
    float2 screenUV = pin.screenPos.xy / pin.screenPos.w;
    screenUV = screenUV * 0.5f + 0.5f;
    screenUV.y = 1.0f - screenUV.y;
    // 頂点座標より、カメラからの奥行きを取得
    float objDepth = pin.screenPos.z / pin.screenPos.w - 0.001f;
    
    float texDepth = depthTex.Sample(samp, screenUV).r;
    if (screenUV.x < 0.0f || 1.0f < screenUV.x ||
        screenUV.y < 0.0f || 1.0f < screenUV.y)
    {
        texDepth = 1.0f;
    }
    // フォンシェーディング（ライトの向きとピクセルの法線で内積）
    float3 N = normalize(pin.normal);
    float3 L = normalize(lightDir.xyz);
    L = -L;
    float diffuse = saturate(dot(N, L));
    // 影が落ちるなら拡散光を０に
    diffuse *= step(objDepth, texDepth);
    // フォンシェーディング（色付け）
    col.rgb *= (diffuse + lightAmbient.rgb);

    return col;
#if 0
    if (objDepth > texDepth)
    {
        col.rgb *= lightAmbient.rgb;
    }
    else
    {
        float3 N = normalize(pin.normal);
        float3 L = normalize(lightDir.xyz);
        
        L = -L;
        
        float diffuse = saturate(dot(N, L)); // dot:内積計算
        
        col.rgb *= (diffuse + lightAmbient.rgb);
    }
#endif
}