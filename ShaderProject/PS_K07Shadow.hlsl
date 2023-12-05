struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 screenPos : POSITION0;
};


SamplerState samp : register(s0);
texture2D tex : register(t0);
texture2D depthTex : register(t1);

cbuffer Light : register(b0)
{
	// �g�U��(���̂ɒ��ڂ������)
    float4 lightColor; // ���̐F
    float4 lightAmbient; // ����(����ɓ������Ē��˕Ԃ��)
    float4 lightDir; // ���̌���
}

float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = tex.Sample(samp, pin.uv);
    
    // ���_���W�����ʏ��UV���W���擾
    float2 screenUV = pin.screenPos.xy / pin.screenPos.w;
    screenUV = screenUV * 0.5f + 0.5f;
    screenUV.y = 1.0f - screenUV.y;
    // ���_���W���A�J��������̉��s�����擾
    float objDepth = pin.screenPos.z / pin.screenPos.w - 0.001f;
    
    float texDepth = depthTex.Sample(samp, screenUV).r;
    if (screenUV.x < 0.0f || 1.0f < screenUV.x ||
        screenUV.y < 0.0f || 1.0f < screenUV.y)
    {
        texDepth = 1.0f;
    }
    // �t�H���V�F�[�f�B���O�i���C�g�̌����ƃs�N�Z���̖@���œ��ρj
    float3 N = normalize(pin.normal);
    float3 L = normalize(lightDir.xyz);
    L = -L;
    float diffuse = saturate(dot(N, L));
    // �e��������Ȃ�g�U�����O��
    diffuse *= step(objDepth, texDepth);
    // �t�H���V�F�[�f�B���O�i�F�t���j
    col.rgb *= (diffuse + lightAmbient.rgb);

    return col;
#if 0
    if (objDepth > texDepth)
    {
        col.rgb *= lightAmbient.rgb;
    }
    else
    {
        float3 N = normalize(pin.normal);
        float3 L = normalize(lightDir.xyz);
        
        L = -L;
        
        float diffuse = saturate(dot(N, L)); // dot:���όv�Z
        
        col.rgb *= (diffuse + lightAmbient.rgb);
    }
#endif
}