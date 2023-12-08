#pragma once
#include <DirectXMath.h>

namespace DirectXUtil
{
	void Increment(DirectX::XMFLOAT2* dest, DirectX::XMFLOAT2 src);
	void Decriment(DirectX::XMFLOAT2* dest, DirectX::XMFLOAT2 src);

	void Increment(DirectX::XMFLOAT3* dest, DirectX::XMFLOAT3 src);
	void Decriment(DirectX::XMFLOAT3* dest, DirectX::XMFLOAT3 src);

	void Increment(DirectX::XMFLOAT4* dest, DirectX::XMFLOAT4 src);
	void Decriment(DirectX::XMFLOAT4* dest, DirectX::XMFLOAT4 src);

	DirectX::XMFLOAT2 Add(DirectX::XMFLOAT2 left, DirectX::XMFLOAT2 right);
	DirectX::XMFLOAT2 Sub(DirectX::XMFLOAT2 left, DirectX::XMFLOAT2 right);

	DirectX::XMFLOAT3 Add(DirectX::XMFLOAT3 left, DirectX::XMFLOAT3 right);
	DirectX::XMFLOAT3 Sub(DirectX::XMFLOAT3 left, DirectX::XMFLOAT3 right);

	DirectX::XMFLOAT4 Add(DirectX::XMFLOAT4 left, DirectX::XMFLOAT4 right);
	DirectX::XMFLOAT4 Sub(DirectX::XMFLOAT4 left, DirectX::XMFLOAT4 right);

}