struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0; // モデルの法線
	// pixel's world position, passed by vertex shader.
    float4 worldPos : POSITION0;
};

// camera's data.
cbuffer Camera : register(b0)
{
    float4 cameraPos;
};

// Fog's data
cbuffer Fog : register(b1)
{
    float4 fogColor;
    float fogStart; // fog's start dictance.
    float fogRange;
    float2 d;
};
cbuffer Snow : register(b2)
{
    float snowRange;
    float3 snowD;
};

cbuffer Light : register(b3)
{
	// 拡散光(物体に直接あたる光)
    float4 lightColor; // 光の色
    float4 lightAmbient; // 環境光(周りに当たって跳ね返る光)
    float4 lightDir; // 光の向き
};

// テクスチャ
Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = tex.Sample(samp, pin.uv);
    float snow = pin.worldPos.y + 1.0f;
    snow /= snowRange;
    snow = saturate(snow);
    col.rgb = lerp(col.rgb, float3(0.9f, 0.9f, 0.9f), snow * saturate(pin.normal.y));
    float3 N = normalize(pin.normal);
    float3 L = normalize(lightDir.xyz);
    L = -L;
    float diffuse = saturate(dot(N, L));
    col.rgb *= (diffuse + lightAmbient.rgb);

    //col.a *= step(0.0f, pin.worldPos.y);
    // ここまでで影
    
    
	// フォグの色を計算する地点までの距離
    float3 V = pin.worldPos.xyz - cameraPos.xyz;
    float dist = length(V);

	// フォグの色を変化させる係数の計算
    float fog = dist - fogStart; // フォグの開始距離で引く
    fog /= fogRange; // フォグの適用範囲で割る
    fog = saturate(fog); // 0～1に
    

	// 係数から元の色とフォグの色をブレンドさせる
    //col.rgb = lerp(col.rgb, fogColor.rgb, fog);

    
    return col;
}