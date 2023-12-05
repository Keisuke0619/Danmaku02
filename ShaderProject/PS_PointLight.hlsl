struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 worldPos : POSITION0;
};

// ���C�g�̏��
struct Light
{
    float4 color;
    float3 pos;
    float range;
};

cbuffer LightBuf : register(b0)
{
    Light lights[30];
}


// �e�N�X�`��
Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = float4(0, 0, 0, 1);
    //tex.Sample(samp, pin.uv);
    for (int i = 0; i < 30; i++)
    {
        // �s�N�Z������_�����Ɍ������x�N�g��
            float3 toLight = lights[i].pos - pin.worldPos.xyz;
        float3 V = normalize(toLight);
        float toLightDistance = length(toLight);
    
        float3 N = normalize(pin.normal);
        float dotNV = saturate(dot(N, V));

        // ��������
        float attenuation = saturate(1.0f - toLightDistance / lights[i].range);
        // ���R��
        attenuation = pow(attenuation, 2);
        //col.rgb *= saturate(dotNV * attenuation + 0.1f);
        col.rgb += lights[i].color * dotNV * attenuation;
    }
    return col;
}