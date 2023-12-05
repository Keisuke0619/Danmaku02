struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 worldPos : POSITION0;
};

// ���C�g�̏��
cbuffer Light : register(b0)
{
    float4 lightColor;
    float3 lightPos;
    float lightRange;
    float3 lightDirection;
    float lightAngle;
}

// �e�N�X�`��
Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = tex.Sample(samp, pin.uv);
    // �s�N�Z������_�����Ɍ������x�N�g��
    float3 toLight = lightPos - pin.worldPos.xyz;
    float3 V = normalize(toLight);
    float toLightDistance = length(toLight);
    
    float3 N = normalize(pin.normal);
    float dotNV = saturate(dot(N, V));

    // ��������
    float attenuation = saturate(1.0f - toLightDistance / lightRange);
    // ���R��
    attenuation = pow(attenuation, 2);
    
    // �X�|�b�g���C�g�̏���
    float3 L = normalize(-lightDirection);
    // ��̃x�N�g�������K������Ă���ꍇ�A�Ԃ̊p�x��
    // dot(v1,v2) = cos��
    // acos(dot(v1, v2)) = �Ƃŋ��߂邱�Ƃ��ł���B
    float dotVL = dot(V, L);
    float angle = acos(dotVL);
    // �Y�o�b�Ƃ������Ⴄ
    //float power = 1 - step(lightAngle / 2, angle);
    //attenuation *= power;
    
    // �p�x�ɉ����Ė��邳���v�Z
    float diffAngle = (lightAngle * 0.5f) * 0.1f;   // �{���͒萔�Őݒ肷��Ƃ����B
    float spotAngle = ((lightAngle * 0.5f) + diffAngle);
    float spotRate = (spotAngle - angle) / diffAngle;
    spotRate = saturate(spotRate);
    col.rgb *= saturate(dotNV * attenuation * spotRate + 0.1f);
    return col;
}