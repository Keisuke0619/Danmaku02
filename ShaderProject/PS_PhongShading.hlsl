struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;	// ���f���̖@��
};

// ���C�g�̏��
cbuffer Light : register(b0)
{
	// �g�U��(���̂ɒ��ڂ������)
	float4 lightColor;		// ���̐F
	float4 lightAmbient;	// ����(����ɓ������Ē��˕Ԃ��)
	float4 lightDir;		// ���̌���
}


// �e�N�X�`��
Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	// ���̌�����\���x�N�g���ƁA���f���̖@���x�N�g���̓���\��
	// �p�x�̊ւ��āA���������Ȃ�e�ɂȂ�B���Ε������Ɩ��邭�Ȃ�
	float3 N = normalize(pin.normal);
	float3 L = normalize(lightDir.xyz);

	// ���̂܂܌v�Z����ƁA����������ʂɉe���ł��Ă��܂��̂ŁA
	// �����Č��̌����𔽓]������
	L = -L;

	// ��̃x�N�g���̓��ς��瓯���������A���Ό��������ׂ�
	// �����������Ȃ�1(���̒l)�A���Ό����Ȃ�-1(���̒l)
	// �F�͔���1�A����0�ɂȂ�̂ŁA���ς̌��ʂ����̂܂ܖ��邳�Ƃ��Ďg��
	// saturate ... 1���傫�����1�A0��菬������0�ɂ���
	float diffuse = saturate(dot(N, L));	// dot:���όv�Z
	color.rgb = diffuse;

	// �e�N�X�`���̐F���l�����ĉA��t����
	float4 texColor = tex.Sample(samp, pin.uv);
	color.rgb = texColor.rgb * (diffuse + lightAmbient.rgb);

	return color;
}