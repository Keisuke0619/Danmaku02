struct PS_IN
{
	float4 pos : SV_POSITION0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

cbuffer Param : register(b0)
{
	float innerDistance;
	float outerDistance;
	float gageRate;
	float dummy;
}
Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 col = float4(0.0f,0.0f,0.0f,0.0f);
	// UV�̒l����A���S�܂ł̋��������߂�
	float2 center = float2(0.5f, 0.5f);
	float2 offset = pin.uv - center;
	float r = length(offset);

	// ���S����̋����ŕ\�����邩���Ȃ�������B������if�����g���Ƃ��ׂẴs�N�Z���Ōv�Z���邱�ƂɂȂ�̂ŁAif�����g�킸�ɕ\���̔���i�v�Z�j���s���B
	col.a = 1.0f - step(innerDistance, r);
	col.a *= step(outerDistance, r);

	// �ɍ��W�̊p�x�����߂�
	float pi = 3.141592f;
	float rad = atan2(offset.y, offset.x);
	rad -= pi * 0.5f;	// �Q�[�W�̃X�^�[�g�n�_��ύX
	rad /= pi;			// -3.14�`3.14��-1�`1�ɕύX
	rad = rad * 0.5f + 0.5f;	// -1�`1��0�`1�ɕύX
	rad = frac(rad);	// �����_�����������o��
	// �Q�[�W�̃w����\��
	float gage = rad;
	gage = 1.0f - step(gageRate, gage);
	float3 frontCol = float3(0.0f, 1.0f, 0.3f);
	float3 backCol = float3(0.3f, 0.3f, 0.3f);

	frontCol = tex.Sample(samp, float2(rad, r * 2));
	col.rgb = lerp(backCol, frontCol, gage);
	return col;
}