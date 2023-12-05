// ���_�V�F�[�_����󂯎�邽�߂̃f�[�^���`
struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
};

// �e�N�X�`�����󂯎��
Texture2D tex : register(t0);
SamplerState samp : register(s0); // �e�N�X�`���̌J��Ԃ��ݒ�

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = tex.Sample(samp, pin.uv);

    float4 frame = float4(0.0f, 0.0f, 0.0f, 1.0f);
    frame.rg += step(sin((pin.uv.x + pin.uv.y) * 3.14f * 8), 0) * 0.2f;
    float2 uv = pin.uv;
    
    // ���̓I�Ɍ�����A�̌v�Z
    // �̈���^�ŋ�؂��Ė��邢�����ƈÂ��������v�Z
    frame.rgb += step(uv.x + uv.y, 1.0f) * 0.3f;
    frame.rgb += step(uv.y - uv.x, 0.0f) * 0.2f;
    frame.rgb = saturate(frame.rgb);
    uv -= 0.5f;
    uv = abs(uv);
    float mask = 1 - step(max(uv.x, uv.y), 0.4f);
    
    color = lerp(color, frame, mask);
    
    return color;
}