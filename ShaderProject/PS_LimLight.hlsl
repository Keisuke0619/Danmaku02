struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;	// ���f���̖@��
	// pixel's world position, passed by vertex shader.
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

// �e�N�X�`��
Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 col =  tex.Sample(samp, pin.uv);
	// �t�����ǂ������v�Z
	float3 L = normalize(lightDir.xyz);
	float3 V = normalize(cameraPos.xyz - pin.worldPos.xyz);
	float backLight = saturate(dot(L, V));
	// ���̕����̌v�Z�i�@���ɑ΂��Ď����������ɓ�����ӏ��̌v�Z�j
	float3 N = normalize(pin.normal);

	//float edge = 1 - abs(dot(N, V));
	float edge = dot(N, V);
	edge = 1 - abs(edge);
	float lim = pow(backLight * edge, 6) * 5;
	// �t�H���V�F�[�f�B���O
	float diffuse = saturate(dot(N, -L));	// dot:���όv�Z
	col.rgb *= diffuse + lightAmbient;

	col.rgb += lim;
	return col;
}