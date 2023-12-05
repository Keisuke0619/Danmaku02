struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

sampler samp : register(s0);
Texture2D albedoTex : register(t0);
Texture2D depthTex : register(t1);
Texture2D blurTex : register(t2);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    // �e�����擾
    float4 albedo = albedoTex.Sample(samp, pin.uv);
    float depth = depthTex.Sample(samp, pin.uv).r;
    float4 blur = blurTex.Sample(samp, pin.uv);

    float focus = 5.0f;     // �J��������s���g�������ʒu�܂ł̋���
    float DofRange = 5.0f;  // ��ʊE�[�x�łڂ����̉摜�ɕϊ����鋗��
    
    float centerDist = abs(depth - focus);          // �t�H�[�J�X�ʒu����̋���
    float rate = saturate(centerDist / DofRange);   // ���K��
    
    color = lerp(albedo, blur, rate);
    
    return color;
}