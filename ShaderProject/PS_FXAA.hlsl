// ���_�V�F�[�_����󂯎�邽�߂̃f�[�^���`
struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
};

// �e�N�X�`�����󂯎��
Texture2D tex : register(t0);
SamplerState samp : register(s0); // �e�N�X�`���̌J��Ԃ��ݒ�

// �P�x�̌v�Z
float Luminance(float3 _col)
{
    return _col.r * 0.3f + _col.g * 0.6f + _col.b * 0.1f;
}

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = tex.Sample(samp, pin.uv);
    // �����`
    float2 diff = float2(1.0f / 1280.0f, 1.0f / 720.0f);
    // ���ӐF�擾
    float lumi00 = Luminance(tex.Sample(samp, saturate(pin.uv + diff * float2( 0, 0))).rgb);
    float lumi01 = Luminance(tex.Sample(samp, saturate(pin.uv + diff * float2(-1,-1))).rgb);
    float lumi02 = Luminance(tex.Sample(samp, saturate(pin.uv + diff * float2( 1,-1))).rgb);
    float lumi03 = Luminance(tex.Sample(samp, saturate(pin.uv + diff * float2(-1, 1))).rgb);
    float lumi04 = Luminance(tex.Sample(samp, saturate(pin.uv + diff * float2( 1, 1))).rgb);
    // ��Ԗ��邢�P�x�ƒႢ�P�x���擾���A�v�Z
    float lumiMax = max(max(lumi01, lumi02), max(lumi03, lumi04));
    float lumiMin = min(min(lumi01, lumi02), min(lumi03, lumi04));
    float lumiSub = max(lumiMax, lumi00) - min(lumiMin, lumi00);
    
    if (lumiSub < 0.25f)
    {
        return color;
    }
    float2 dir = float2(lumi02 - lumi03, lumi04 - lumi01);
    dir = normalize(dir);
    dir *= diff;
    
    // �F�̍������킹
    float4 col1 = tex.Sample(samp, pin.uv + dir);
    float4 col2 = tex.Sample(samp, pin.uv - dir);
    color = (col1 + col2) * 0.5f;
    return color;
}