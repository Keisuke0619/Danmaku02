struct gsIn
{
    float4 pos : SV_POSITION;
    float4 scale : TEXCOORD0;
    float4 uv : TEXCOORD1;
};
struct GSOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

struct VP
{
    float4x4 view;
    float4x4 proj;
};
[maxvertexcount(4)]
void main(
	point gsIn input[1] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
    GSOutput vtx;
    gsIn gin = input[0];
    gin.pos = float4(0, 0, 0, 1);
    gin.scale = float4(1, 1, 0, 0);
    gin.uv = float4(0, 0, 1, 1);
    float size = gin.scale.x;
    const float4 vtxOffset[] =
    {
        float4(-size, size, 0, 0),
		float4(size, size, 0, 0),
		float4(-size, -size, 0, 0),
		float4(size, -size, 0, 0)
    };
    
    vtx.pos = gin.pos + vtxOffset[0];
    vtx.uv = gin.uv.xy;
    output.Append(vtx);
    
    vtx.pos = gin.pos + vtxOffset[1];
    vtx.uv = gin.uv.zy;
    output.Append(vtx);
    
    vtx.pos = gin.pos + vtxOffset[2];
    vtx.uv = gin.uv.xw;
    output.Append(vtx);
    
    vtx.pos = gin.pos + vtxOffset[3];
    vtx.uv = gin.uv.zw;
    output.Append(vtx);
}