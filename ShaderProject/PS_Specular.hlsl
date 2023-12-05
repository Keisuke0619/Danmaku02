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
	float4 col = tex.Sample(samp, pin.uv);

	// ���̃x�N�g���Ɩ@������A���˂���x�N�g���i���˃x�N�g���j�����߂�
	// ���̃x�N�g����@�������ɂQ�񓮂����ƁA���˃x�N�g���ɂȂ�I
	float3 L = normalize(lightDir.xyz);
	float3 N = normalize(pin.normal.xyz);
	float3 R = L + 2 * N * -dot(L, N);
	// float3 R = reflect(L, N);
	// ���˒n�_�i���[���h���W�j����J�����Ɍ������x�N�g��
	float3 V = normalize(cameraPos.xyz - pin.worldPos.xyz);
	// ���˃x�N�g���ƁA�J�����Ɍ������x�N�g�������������Ȃ�܂Ԃ����Ȃ�B
	float specular = saturate(dot(R, V));
	// �ۏ�̌v�Z���s���ƁA�Ǐ��I�ɖ��邭�Ȃ�B
	specular = pow(specular, 20);

	// �A������t�H���V�F�[�f�B���O�̗v�f��ǉ�
	float diffuse = saturate(dot(N, -L));	// dot:���όv�Z
	col.rgb *= diffuse + lightAmbient.rgb;

	col.rgb += specular;

	return col;
}