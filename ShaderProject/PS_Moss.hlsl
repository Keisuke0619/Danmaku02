struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
};

cbuffer Light : register(b0)
{
	// �g�U��(���̂ɒ��ڂ������)
	float4 lightColor;		// ���̐F
	float4 lightAmbient;	// ����(����ɓ������Ē��˕Ԃ��)
	float4 lightDir;		// ���̌���
};

cbuffer Param : register(b1)
{
	float cbMossRate;		// ���������i�O�\�� �P�S�������j
	float cbMossRange;	// �ω�����͈�
	float2 d;
};

Texture2D tex : register(t0);
Texture2D dissolve : register(t1);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
	color = tex.Sample(samp, pin.uv);

	// �@��
	float3 N = normalize(pin.normal);
	// �ۂ�������or�Ⴊ�ς������
	float3 MossDir = normalize(float3(0, 1, 0.5f));
	MossDir = normalize(MossDir);

	// �ۂ̐����
	float moss = dot(N, MossDir);
	float mossStart = cbMossRate;	// �ω����n��
	float mossRange = cbMossRange;	// �ω��͈�
	moss = moss - mossStart;
	moss = moss / mossRange;
	moss = saturate(moss);	// 0�`1�Ɏ��߂�
	moss *= 0.8f;

	// �ۂ𐶂₷
	float3 mossColor = float3(1, 1, 1);
	color.rgb = lerp(color.rgb, mossColor, moss);

	float3 L = -normalize(lightDir.xyz);
	float diffuse = saturate(dot(N, L));
	color.rgb *= diffuse + lightAmbient;

	return color;
}