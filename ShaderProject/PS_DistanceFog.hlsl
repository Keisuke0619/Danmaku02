struct PS_IN
{
	float4 pos : SV_POSITION0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	// send from pixel shader that world position
	float4 worldPos : POSITION0;
};

// ���C�g�̏��
cbuffer Light : register(b0)
{
	// �g�U��(���̂ɒ��ڂ������)
	float4 lightColor;		// ���̐F
	float4 lightAmbient;	// ����(����ɓ������Ē��˕Ԃ��)
	float4 lightDir;		// ���̌���
}

// camera's data.
cbuffer Camera : register(b1)
{
	float4 cameraPos;
};

// Fog's data
cbuffer Fog:register(b2)
{
	float4 fogColor;
	float fogStart;		// fog's start dictance.
	float fogRange;
};

texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 col = tex.Sample(samp, pin.uv);

	// �t�H�O�̐F���v�Z����n�_�܂ł̋���
	float3 V = pin.worldPos.xyz - cameraPos.xyz;
	float dist = length(V);

	// �t�H�O�̐F��ω�������W���̌v�Z
	float fog = dist - fogStart;	// �t�H�O�̊J�n�����ň���
	fog /= fogRange;				// �t�H�O�̓K�p�͈͂Ŋ���
	fog = saturate(fog);			// 0�`1��

	// ���̐F���v�Z
	float3 N = normalize(pin.normal);
	float3 L = normalize(lightDir.xyz);
	float diffuse = saturate(dot(N, -L));
	col.rgb *= diffuse + lightAmbient.rgb;

	// �W�����猳�̐F�ƃt�H�O�̐F���u�����h������
	col.rgb = lerp(col.rgb, fogColor.rgb, fog);

	return col;
}