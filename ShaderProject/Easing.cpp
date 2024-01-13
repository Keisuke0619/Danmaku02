#include "Easing.h"
#include <math.h>

float Liner(float progress);
float InSine(float p);
float OutSine(float p);
float InOutSine(float p);


static const float PI = 3.1415926535f;
float (*g_func[Util::EType::TYPE_MAX])(float) = 
{
    &Liner,
    &InSine,
    &OutSine,
    &InOutSine,
};

float Util::Lerp(float progress, float begin, float end, EType easeType)
{
    return begin + (end - begin) * Ease(progress, easeType);
}

DirectX::XMFLOAT2 Util::Lerp(float progress, DirectX::XMFLOAT2 begin, DirectX::XMFLOAT2 end, EType easeType)
{
    return DirectX::XMFLOAT2(
        Lerp(progress, begin.x, end.x, easeType),
        Lerp(progress, begin.y, end.y, easeType)
    );
}

DirectX::XMFLOAT3 Util::Lerp(float progress, DirectX::XMFLOAT3 begin, DirectX::XMFLOAT3 end, EType easeType)
{
    return DirectX::XMFLOAT3(
        Lerp(progress, begin.x, end.x, easeType),
        Lerp(progress, begin.y, end.y, easeType),
        Lerp(progress, begin.z, end.z, easeType)
    );
}

DirectX::XMFLOAT4 Util::Lerp(float progress, DirectX::XMFLOAT4 begin, DirectX::XMFLOAT4 end, EType easeType)
{
    return DirectX::XMFLOAT4(
        Lerp(progress, begin.x, end.x, easeType),
        Lerp(progress, begin.y, end.y, easeType),
        Lerp(progress, begin.z, end.z, easeType),
        Lerp(progress, begin.w, end.w, easeType)
    );
}

float Util::Ease(float progress, EType easeType)
{
    return (*g_func[easeType])(progress);
}

float Liner(float progress)
{
    return progress;
}

float InSine(float p)
{
    return 1 - cosf((p * PI) / 2);
}

float OutSine(float p)
{
    return sinf((p * PI) / 2);
}

float InOutSine(float p)
{
    return -(cosf(p * PI) - 1) / 2;
}
