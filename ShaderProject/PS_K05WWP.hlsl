struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0; // ���f���̖@��
	// pixel's world position, passed by vertex shader.
    float4 worldPos : POSITION0;
};

// camera's data.
cbuffer Camera : register(b0)
{
    float4 cameraPos;
};

// Fog's data
cbuffer Fog : register(b1)
{
    float4 fogColor;
    float fogStart; // fog's start dictance.
    float fogRange;
    float2 d;
};
cbuffer Snow : register(b2)
{
    float snowRange;
    float3 snowD;
};

cbuffer Light : register(b3)
{
	// �g�U��(���̂ɒ��ڂ������)
    float4 lightColor; // ���̐F
    float4 lightAmbient; // ����(����ɓ������Ē��˕Ԃ��)
    float4 lightDir; // ���̌���
};

// �e�N�X�`��
Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = tex.Sample(samp, pin.uv);
    float snow = pin.worldPos.y + 1.0f;
    snow /= snowRange;
    snow = saturate(snow);
    col.rgb = lerp(col.rgb, float3(0.9f, 0.9f, 0.9f), snow * saturate(pin.normal.y));
    float3 N = normalize(pin.normal);
    float3 L = normalize(lightDir.xyz);
    L = -L;
    float diffuse = saturate(dot(N, L));
    col.rgb *= (diffuse + lightAmbient.rgb);

    //col.a *= step(0.0f, pin.worldPos.y);
    // �����܂łŉe
    
    
	// �t�H�O�̐F���v�Z����n�_�܂ł̋���
    float3 V = pin.worldPos.xyz - cameraPos.xyz;
    float dist = length(V);

	// �t�H�O�̐F��ω�������W���̌v�Z
    float fog = dist - fogStart; // �t�H�O�̊J�n�����ň���
    fog /= fogRange; // �t�H�O�̓K�p�͈͂Ŋ���
    fog = saturate(fog); // 0�`1��
    

	// �W�����猳�̐F�ƃt�H�O�̐F���u�����h������
    //col.rgb = lerp(col.rgb, fogColor.rgb, fog);

    
    return col;
}