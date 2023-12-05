struct PS_IN
{
	float4 pos : SV_POSITION0;
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
	float dissolveRate;		// ���������i�O�\�� �P�S�������j
	float dissolveRange;	// �ω�����͈�
	float2 d;
};

Texture2D tex : register(t0);
Texture2D dissolve : register(t1);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 col = tex.Sample(samp, pin.uv);

	float3 N = normalize(pin.normal);
	float3 L = normalize(-lightDir.xyz);

	float diffuse = saturate(dot(N, L));
	col.rgb *= saturate(diffuse + lightAmbient);

	float value = dissolve.Sample(samp, pin.uv).r;
#if 0	// step�Ő^����ɂ킯��
	col.a = step(dissolveRate, value);
#else
	// �ω��͈͂��l�������S�̂̕ω���
	float len = 1.0f - dissolveRange;
	// �ω��͈͂ɍ��킹�Ă������l��␳
	value += dissolveRange;
	// ��l�iRate�j���O�`�P�ł͂Ȃ��A�O�`�P�{�ω��͈͂ƂȂ�悤�ɕύX
	float rate = dissolveRate / len;
	// ���C�𓥂܂��āA�O�̎��͕s�����A�P�̎��ɂ͂��ׂē����ɂȂ�悤�Ɍv�Z
	col.a = saturate((value - rate) / dissolveRange);
#endif
	return col;
}