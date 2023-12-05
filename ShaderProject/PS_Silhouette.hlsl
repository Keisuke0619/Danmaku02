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
    // ���_���W�����ʏ��UV���W���擾
    float2 screenUV = pin.screenPos.xy / pin.screenPos.w;
    screenUV = screenUV * 0.5f + 0.5f;
    screenUV.y = 1.0f - screenUV.y;
    // ���_���W���A�J��������̉��s�����擾
    float objDepth = pin.screenPos.z / pin.screenPos.w;
    
    float texDepth = depthTex.Sample(samp, screenUV).r;
    mask.rgb *= 1 - step(texDepth, objDepth);
    col.rgb -= mask.rgb;
    return col;
}