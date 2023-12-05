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
    // 1テクセル
    const float2 texelDistance = 1.0f / float2(400, 400);
    const float2 offset[] =
    {
        float2(-1.0f,-1.0f), float2(0.0f,-1.0f), float2(1.0f,-1.0f),
        float2(-1.0f, 0.0f), float2(0.0f, 0.0f), float2(1.0f, 0.0f),
        float2(-1.0f, 1.0f), float2(0.0f, 1.0f), float2(1.0f, 1.0f),
    };
    const float2 offsetArea[] =
    {
        float2(-1.0f, -1.0f), float2(1.0f, -1.0f),
        float2(-1.0f,  1.0f), float2(1.0f,  1.0f),
    };
    float4 col = tex.Sample(samp, pin.uv);
    float3 bunsan[4];
    float3 average[4];
    [unroll(4)]
    for (int j = 0; j < 4; j++)
    {
        float2 uv = pin.uv + offsetArea[j] * texelDistance;
        // 各領域から平均と分散を求める
        float3 total = float3(0, 0, 0);
        float3 value[9];
        [unroll(9)]
        for (int i = 0; i < 9; i++)
        {
            value[i] = tex.Sample(samp, uv + offset[i] * texelDistance);
            total += value[i];
        }
        average[j] = total / 9;
        total = float3(0, 0, 0);
        
        [unroll(9)]
        for (int i = 0; i < 9; i++)
        {
            total += pow(value[i] - average[j], 2);
        }
        bunsan[j] = total / 9;
    }
    float3 minBunsan = float3(100.0f, 100.0f, 100.0f);
    [unroll(4)]
    for (int i = 0; i < 4; i++)
    {
        if (bunsan[i].r < minBunsan.r)
        {
            col.r = average[i].r;
            minBunsan.r = bunsan[i].r;
        }
        if (bunsan[i].g < minBunsan.g)
        {
            col.g = average[i].g;
            minBunsan.g = bunsan[i].g;
        }
        if (bunsan[i].b < minBunsan.b)
        {
            col.b = average[i].b;
            minBunsan.b = bunsan[i].b;
        }
    }
    
    return col;

}