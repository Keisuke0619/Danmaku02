struct PS_IN
{
    float4 pos : SC_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);
float3 ToHSV(float r, float g, float b);
float3 ToRGB(float h, float s, float v);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    // 解像度を下げてドット絵のような表現
    const float size = 8;
    const float2 dotScreenSize = float2(1920.0f / size, 1080.0f / size);
    float2 i_uv = pin.uv * dotScreenSize;
    i_uv = floor(i_uv) + 0.5f;
    i_uv /= dotScreenSize;
    
    
    col = tex.Sample(samp, i_uv);
    const int colScale = 4;
    float3 tone = float3(4, 2, 4);
#if 0
    col *= colScale;
    col = floor(col + 0.5f);
    col /= colScale;
#else
    float3 hsv = ToHSV(col.r, col.g, col.b);
    hsv *= tone;
    hsv = floor(hsv + 0.5f);
    hsv /= tone;
    col.rgb = ToRGB(hsv.x, hsv.y, hsv.z);
#endif
    //col.rgb = ToRGB(fmod(pin.uv.x + pin.uv.y, 1), 1, 1);
    return col;
}

float3 ToHSV(float r, float g, float b)
{
    float max = r > g ? r : g;
    max = max > b ? max : b;
    float min = r < g ? r : g;
    min = min < b ? min : b;
    float h = max - min;
    if (h > 0.0f)
    {
        if (max == r)
        {
            h = (g - b) / h;
            if (h < 0.0f)
            {
                h += 6.0f;
            }
        }
        else if (max == g)
        {
            h = 2.0f + (b - r) / h;
        }
        else
        {
            h = 4.0f + (r - g) / h;
        }
    }
    h /= 6.0f;
    float s = (max - min);
    if (max != 0.0f)
        s /= max;
    float v = max;
    return float3(h, s, v);

}

float3 ToRGB(float h, float s, float v)
{
    float r = v;
    float g = v;
    float b = v;
    if (s > 0.0f)
    {
        h *= 6.0f;
        int i = (int) h;
        float f = h - (float) i;
        switch (i)
        {
            default:
            case 0:
                g *= 1 - s * (1 - f);
                b *= 1 - s;
                break;
            case 1:
                r *= 1 - s * f;
                b *= 1 - s;
                break;
            case 2:
                r *= 1 - s;
                b *= 1 - s * (1 - f);
                break;
            case 3:
                r *= 1 - s;
                g *= 1 - s * f;
                break;
            case 4:
                r *= 1 - s * (1 - f);
                g *= 1 - s;
                break;
            case 5:
                g *= 1 - s;
                b *= 1 - s * f;
                break;
        }
    }
    return float3(r, g, b);

}