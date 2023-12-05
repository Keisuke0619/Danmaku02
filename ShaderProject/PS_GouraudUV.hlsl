struct PS_IN
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0; // 頂点側で計算した明るさの情報
    float2 uv : TEXCOORD0; // テクスチャはピクセルシェーダで張り付け
    float3 normal : NORMAL0;
    float3 wpos : POSITION0;
};


Texture2D tex : register(t0);
SamplerState samp : register(s0);

cbuffer UVScale : register(b0)
{
    float2 uvScale;
    float d[2];
};
cbuffer Camera : register(b1)
{
    float4 cameraPos;
};

// Fog's data
cbuffer Fog : register(b2)
{
    float4 fogColor;
    float fogStart; // fog's start dictance.
    float fogRange;
    float2 dd;
};
cbuffer Snow : register(b3)
{
    float snowRange;
    float3 snowD;
};
float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = tex.Sample(samp, pin.uv * uvScale);
    col.rgb = lerp(float3(0.1f, 0.1f, 0.27f), col.rgb, pow(pin.normal.y, 2));
    float snow = pin.wpos.y + 1.0f;
    snow /= snowRange;
    snow = saturate(snow);
    col.rgb = lerp(col.rgb, float3(0.9f, 0.9f, 0.9f), snow * saturate(pin.normal.y));
    col.rgb *= pin.color.rgb;
    	// フォグの色を計算する地点までの距離
    float3 V = pin.wpos.xyz - cameraPos.xyz;
    float dist = length(V);

	// フォグの色を変化させる係数の計算
    float fog = dist - fogStart; // フォグの開始距離で引く
    fog /= fogRange; // フォグの適用範囲で割る
    fog = saturate(fog); // 0～1に
    

	// 係数から元の色とフォグの色をブレンドさせる
    col.rgb = lerp(col.rgb, fogColor.rgb, fog);

    return col;
}