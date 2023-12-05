//�O�T�P�T���Ɠ��e��Ō������
struct PS_IN
{
	float4 pos:SV_POSITION;
	float2 uv:TEXCOORD0;
	float3 normal:NORMAL0;
	//�X�N���[�����W
	float4 screenPos:TEXCOORD1;
};

Texture2D tex : register(t0);
//��̃V�F�[�_�[�ŕ����̃e�N�X�`�������p�ł���
Texture2D space : register(t1);
SamplerState samp : register(s0);


float4 main(PS_IN pin) : SV_TARGET
{
	float4 color=float4(0.0f, 0.0f, 0.0f, 0.0f);
	//�X�N���[�����W���s�N�Z���V�F�[�_�[�ŗ��p���鎞
	//xyz�̃p�����͂��Ŋ���Ȃ��Ɛ������l�ɂȂ�Ȃ�
	//kwsk�̓J�����̍s��̒��g���������藝������K�v������
	color = space.Sample(samp, pin.screenPos.xy/ pin.screenPos.w);

	
	return color;

}