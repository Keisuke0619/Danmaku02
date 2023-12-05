struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    float4 screenPos : POSITION0;
};

SamplerState samp : register(s0);
texture2D depthTex : register(t0);


float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = float4(1, 1, 1, 1);
    // ���_���W�����ʏ��UV���W���擾
    float2 screenUV = pin.screenPos.xy / pin.screenPos.w;
    screenUV = screenUV * 0.5f + 0.5f;
    screenUV.y = 1.0f - screenUV.y;
    // �J��Ԃ��h�~����
    //screenUV = saturate(screenUV);
    // ���_���W���A�J��������̉��s�����擾
    float objDepth = pin.screenPos.z / pin.screenPos.w;
    
    float texDepth = depthTex.Sample(samp, screenUV).r;
    if (screenUV.x < 0.0f || 1.0f < screenUV.x ||
        screenUV.y < 0.0f || 1.0f < screenUV.y)
    {
        texDepth = 1.0f;
    }
    col.rgb = step(objDepth, texDepth);
    return col;
}