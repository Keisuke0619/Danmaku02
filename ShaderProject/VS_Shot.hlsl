struct vsIn
{
    float4 pos : POSITION0;
    float2 uv : TEXCOORD0;
    float2 uvSize : TEXCOORD1;
};

struct vsOut
{
    float4 pos : SV_POSITION;
    float4 scale : TEXCOORD0;
    float4 uv : TEXCOORD1;
};

vsOut main( vsIn vin )
{
    vsOut vout;
    vout.pos = float4(0, 0, 0, 0);
    vout.scale = float4(1,1, 0, 0);
    vout.uv = float4(0,0,1,1);
	return vout;
}