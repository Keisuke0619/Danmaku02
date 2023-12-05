struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 worldPos : POSITION0;
};

// ライトの情報
cbuffer Light : register(b0)
{
    float4 lightColor;
    float3 lightPos;
    float lightRange;
    float3 lightDirection;
    float lightAngle;
}

// テクスチャ
Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = tex.Sample(samp, pin.uv);
    // ピクセルから点光源に向かうベクトル
    float3 toLight = lightPos - pin.worldPos.xyz;
    float3 V = normalize(toLight);
    float toLightDistance = length(toLight);
    
    float3 N = normalize(pin.normal);
    float dotNV = saturate(dot(N, V));

    // 減衰処理
    float attenuation = saturate(1.0f - toLightDistance / lightRange);
    // 自然に
    attenuation = pow(attenuation, 2);
    
    // スポットライトの処理
    float3 L = normalize(-lightDirection);
    // 二つのベクトルが正規化されている場合、間の角度は
    // dot(v1,v2) = cosθ
    // acos(dot(v1, v2)) = θで求めることができる。
    float dotVL = dot(V, L);
    float angle = acos(dotVL);
    // ズバッときっちゃう
    //float power = 1 - step(lightAngle / 2, angle);
    //attenuation *= power;
    
    // 角度に応じて明るさを計算
    float diffAngle = (lightAngle * 0.5f) * 0.1f;   // 本来は定数で設定するといい。
    float spotAngle = ((lightAngle * 0.5f) + diffAngle);
    float spotRate = (spotAngle - angle) / diffAngle;
    spotRate = saturate(spotRate);
    col.rgb *= saturate(dotNV * attenuation * spotRate + 0.1f);
    return col;
}