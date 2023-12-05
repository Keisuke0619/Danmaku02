struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0; // ���f���̖@��
	// pixel's world position, passed by vertex shader.
    float4 worldPos : POSITION0;
};


// �e�N�X�`��
Texture2D tex : register(t0);
SamplerState samp : register(s0);

cbuffer Light : register(b0)
{
	// �g�U��(���̂ɒ��ڂ������)
    float4 lightColor; // ���̐F
    float4 lightAmbient; // ����(����ɓ������Ē��˕Ԃ��)
    float4 lightDir; // ���̌���
}

float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = float4(1, 1, 1, 1);
    float3 N = normalize(pin.normal);
    float3 L = normalize(lightDir.xyz);
    L = -L;
    float diffuse = saturate(dot(N, L));
    col.rgb = diffuse;
    float4 texCol = tex.Sample(samp, pin.uv);
    col.rgb = texCol.rgb * (diffuse + lightAmbient.rgb);

    col.a *= step(0.0f, pin.worldPos.y);
    
    return col;
}