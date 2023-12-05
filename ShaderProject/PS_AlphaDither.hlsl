struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 worldPos : POSITION0;
    float4 sPos : POSITION1;
};

cbuffer Camera : register(b0)
{
    float4 camPos;
};

// �e�N�X�`��
Texture2D tex : register(t0);
Texture2D alphaTex : register(t1); // �����F�����̃e�N�X�`�����W�X�^�ɏ������܂�Ȃ��B�iSceneToon�Q�Ɓj
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = tex.Sample(samp, pin.uv);
    // �����ɉ����ē����x��ύX
    float len = length(pin.worldPos.xyz - camPos.xyz);
    float alpha  = color.a * saturate(len);
    float2 screenUV = pin.sPos.xy / pin.sPos.w;
    screenUV = screenUV * 0.5f + 0.5f;
    screenUV *= float2(1280.0f, 720.0f) / 4.0f;
    float dither = alphaTex.Sample(samp, screenUV).r;
    if( alpha < dither )
        discard;
    
    return color;
}