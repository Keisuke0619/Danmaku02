struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    float3 wPos : POSITION0;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);


cbuffer Camera : register(b0)
{
    float4x4 view;
    float4x4 proj;
};

cbuffer Param : register(b1)
{
    float time;
    float3 d;
}

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // 水面のカメラから見た座標系へ変換
    float4 pos = float4(pin.wPos, 1.0f);
    pos = mul(pos, view);
    pos = mul(pos, proj);

    // 頂点シェーダから渡される座標は勝手に計算されている。
    pos.xyz /= pos.w;
    
    // スクリーン座標をテクスチャ座標に変換
    pos.xy *= 0.5f;
    pos.xy += 0.5f;
    pos.y = 1.0f - pos.y;
    
    // ラスタスクロール
    float2 uv = pos.xy;
    uv.x += sin(uv.y * 3.141592f * 6 + time * 10) * 0.01f;
    
    color = tex.Sample(samp, uv);
    // 書き込まれていない部分を水っぽく
    color.a *= 0.7f;
    color = lerp(float4(0.2f, 0.5f, 0.8f, 0.7f), color, color.a);
    //color = float4(uv.x, uv.y, 1, 1);
    return color;
}