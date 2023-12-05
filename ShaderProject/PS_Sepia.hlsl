struct PS_IN
{
    float4 pos : SC_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    col = tex.Sample(samp, pin.uv);
    // ‚Ü‚¸ƒ‚ƒmƒNƒ
    float mono = 0;
    mono += col.r * 0.2f;
    mono += col.g * 0.7f;
    mono += col.b * 0.1f;
    col.rgb = mono;
    
    // —Î‚ÆÂ‚Ì¬•ª‚ğ‚¿‚å‚Á‚Æ”²‚­
    col.g *= 0.7f;
    col.b *= 0.4f;
    return col;
}