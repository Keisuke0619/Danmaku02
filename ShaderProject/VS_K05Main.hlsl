// CPU����n����钸�_�̏����쐬
// �C���v�b�g���C�A�E�g
struct VS_IN
{
    float3 pos : POSITION0; // �Z�}���e�B�N�X
    float3 normal : NORMAL0; // �f�[�^�̗p�r���`����
    float2 uv : TEXCOORD0;
};

// ���_�V�F�[�_����s�N�Z���V�F�[�_�֑���f�[�^�̒�`
struct VS_OUT
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0; // ���_���Ōv�Z�������邳�̏��
    float2 uv : TEXCOORD0; // �e�N�X�`���̓s�N�Z���V�F�[�_�Œ���t��
    float3 normal : NORMAL0;
    float3 wpos : POSITION0;
};

// �萔�o�b�t�@
// CPU��GPU�ł���肷�钲��O�̃f�[�^���i�[�����
cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

cbuffer Light : register(b1)
{
    float4 lightColor;
    float4 lightAmbient;
    float4 lightDir;
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
	// ���[�J�����W���X�N���[�����W�֕ϊ�
    vout.pos = float4(vin.pos, 1.0f); // ���[�J�����W
    vout.pos = mul(vout.pos, world); // ���[���h���W
    vout.wpos = vout.pos;
    vout.pos = mul(vout.pos, view); // �r���[���W
    vout.pos = mul(vout.pos, proj); // �v���W�F�N�V�������W

	// ���_�V�F�[�_�ŉA�̌v�Z
    float3 N = vin.normal;
    float3 L = lightDir.xyz;
	// �v�Z�O�Ɏ��������
    N = mul(N, (float3x3) world);
    L = -L;
	// ���K��
    N = normalize(N);
    L = normalize(L);
	// �A�v�Z
    float diffuse = saturate(dot(N, L));
	// �F����
    vout.color = float4(diffuse + lightAmbient.rgb, 1.0f);
    vout.normal = N;
    vout.uv = vin.uv;

    return vout;
}