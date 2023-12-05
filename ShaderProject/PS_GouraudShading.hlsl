struct PS_IN
{
	float4 pos : SV_POSITION0;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
	
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);


float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = pin.color * tex.Sample(samp, pin.uv);

	return col;
}