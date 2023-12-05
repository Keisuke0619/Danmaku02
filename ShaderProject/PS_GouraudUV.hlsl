struct PS_IN
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0; // ���_���Ōv�Z�������邳�̏��
    float2 uv : TEXCOORD0; // �e�N�X�`���̓s�N�Z���V�F�[�_�Œ���t��
    float3 normal : NORMAL0;
    float3 wpos : POSITION0;
};


Texture2D tex : register(t0);
SamplerState samp : register(s0);

cbuffer UVScale : register(b0)
{
    float2 uvScale;
    float d[2];
};
cbuffer Camera : register(b1)
{
    float4 cameraPos;
};

// Fog's data
cbuffer Fog : register(b2)
{
    float4 fogColor;
    float fogStart; // fog's start dictance.
    float fogRange;
    float2 dd;
};
cbuffer Snow : register(b3)
{
    float snowRange;
    float3 snowD;
};
float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = tex.Sample(samp, pin.uv * uvScale);
    col.rgb = lerp(float3(0.1f, 0.1f, 0.27f), col.rgb, pow(pin.normal.y, 2));
    float snow = pin.wpos.y + 1.0f;
    snow /= snowRange;
    snow = saturate(snow);
    col.rgb = lerp(col.rgb, float3(0.9f, 0.9f, 0.9f), snow * saturate(pin.normal.y));
    col.rgb *= pin.color.rgb;
    	// �t�H�O�̐F���v�Z����n�_�܂ł̋���
    float3 V = pin.wpos.xyz - cameraPos.xyz;
    float dist = length(V);

	// �t�H�O�̐F��ω�������W���̌v�Z
    float fog = dist - fogStart; // �t�H�O�̊J�n�����ň���
    fog /= fogRange; // �t�H�O�̓K�p�͈͂Ŋ���
    fog = saturate(fog); // 0�`1��
    

	// �W�����猳�̐F�ƃt�H�O�̐F���u�����h������
    col.rgb = lerp(col.rgb, fogColor.rgb, fog);

    return col;
}