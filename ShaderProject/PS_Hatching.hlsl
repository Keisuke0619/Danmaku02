struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
	// send for pixel shader that world position
    float4 worldPos : POSITION0;
};
cbuffer Light : register(b0)
{
    float4 lightColor; // 光の色
    float4 lightAmbient; // 環境光(周りに当たって跳ね返る光)
    float4 lightDir; // 光の向き
}

// テクスチャ
Texture2D tex : register(t0);
//Texture2D rampTex : register(t1);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = tex.Sample(samp, pin.uv);
    float3 N = normalize(pin.normal);
    float3 L = -normalize(lightDir.xyz);

    float dotNL = dot(N, L);
    float diffuse = saturate(dot(N, L)); // dot:内積計算
    float hatchingDif = 1.0f - pow(1.0f - saturate(dotNL), 2);
    float rand = sin(pin.worldPos.x * 10.0f);
    
    float sideLine = sin(pin.worldPos.y * 300.0f + rand * 10);
    sideLine = step(-0.5f, sideLine);
    
    float slash = sin(pin.worldPos.y * 300.0f + pin.worldPos.x * 150.0f);
    slash = step(-0.5f, slash);
    col.rgb *= lerp(sideLine, 1, hatchingDif);
    col.rgb *= lerp(slash, 1, step(0, dotNL));
    col.rgb = dot(col.rgb, float3(0.3f, 0.6f, 0.1f));
    return col;

}