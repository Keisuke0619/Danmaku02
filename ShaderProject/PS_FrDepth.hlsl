struct PS_IN
{
    float4 pos : SV_POSITION0;
};

float4 main(PS_IN pin) : SV_TARGET
{
    float depth = pin.pos.z / pin.pos.w;
    return float4(depth, 0, 0, 1.0f);
}