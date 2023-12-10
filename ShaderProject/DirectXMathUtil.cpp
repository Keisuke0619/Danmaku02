#include "DirectXMathUtil.h"



void DirectXUtil::Increment(DirectX::XMFLOAT2* dest, DirectX::XMFLOAT2 src)
{
    dest->x += src.x;
    dest->y += src.y;
}

void DirectXUtil::Decriment(DirectX::XMFLOAT2* dest, DirectX::XMFLOAT2 src)
{
    dest->x -= src.x;
    dest->y -= src.y;
}

void DirectXUtil::Increment(DirectX::XMFLOAT3* dest, DirectX::XMFLOAT3 src)
{
    dest->x += src.x;
    dest->y += src.y;
    dest->z += src.z;
}

void DirectXUtil::Decriment(DirectX::XMFLOAT3* dest, DirectX::XMFLOAT3 src)
{
    dest->x -= src.x;
    dest->y -= src.y;
    dest->z -= src.z;
}

void DirectXUtil::Increment(DirectX::XMFLOAT4* dest, DirectX::XMFLOAT4 src)
{
    dest->x += src.x;
    dest->y += src.y;
    dest->z += src.z;
    dest->w += src.w;
}

void DirectXUtil::Decriment(DirectX::XMFLOAT4* dest, DirectX::XMFLOAT4 src)
{
    dest->x -= src.x;
    dest->y -= src.y;
    dest->z -= src.z;
    dest->w -= src.w;
}

DirectX::XMFLOAT2 DirectXUtil::Add(DirectX::XMFLOAT2 left, DirectX::XMFLOAT2 right)
{
    return DirectX::XMFLOAT2(left.x + right.x, left.y + right.y);
}

DirectX::XMFLOAT2 DirectXUtil::Sub(DirectX::XMFLOAT2 left, DirectX::XMFLOAT2 right)
{
    return DirectX::XMFLOAT2(left.x - right.x, left.y - right.y);
}

DirectX::XMFLOAT2 DirectXUtil::Mul(DirectX::XMFLOAT2 src, float num)
{
    return DirectX::XMFLOAT2(src.x * num, src.y * num);
}

DirectX::XMFLOAT3 DirectXUtil::Add(DirectX::XMFLOAT3 left, DirectX::XMFLOAT3 right)
{
    return DirectX::XMFLOAT3(left.x + right.x, left.y + right.y, left.z + right.z);
}

DirectX::XMFLOAT3 DirectXUtil::Sub(DirectX::XMFLOAT3 left, DirectX::XMFLOAT3 right)
{
    return DirectX::XMFLOAT3(left.x - right.x, left.y - right.y, left.z - right.z);
}

DirectX::XMFLOAT3 DirectXUtil::Mul(DirectX::XMFLOAT3 src, float num)
{
    return DirectX::XMFLOAT3(src.x * num, src.y * num, src.z * num);
}

DirectX::XMFLOAT4 DirectXUtil::Add(DirectX::XMFLOAT4 left, DirectX::XMFLOAT4 right)
{
    return DirectX::XMFLOAT4(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
}

DirectX::XMFLOAT4 DirectXUtil::Sub(DirectX::XMFLOAT4 left, DirectX::XMFLOAT4 right)
{
    return DirectX::XMFLOAT4(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
}

DirectX::XMFLOAT4 DirectXUtil::Mul(DirectX::XMFLOAT4 src, float num)
{
    return DirectX::XMFLOAT4(src.x * num, src.y * num, src.z * num, src.w * num);
}
