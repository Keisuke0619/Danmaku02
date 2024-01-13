#pragma once

#include <DirectXMath.h>
namespace Util
{
	enum EType
	{
		TYPE_LINEAR,
		TYPE_IN_SINE,
		TYPE_OUT_SINE,
		TYPE_INOUT_SINE,

		TYPE_MAX
	};
	float Lerp(float progress, float begin, float end, EType easeType);
	DirectX::XMFLOAT2 Lerp(float progress, DirectX::XMFLOAT2 begin, DirectX::XMFLOAT2 end, EType easeType);
	DirectX::XMFLOAT3 Lerp(float progress, DirectX::XMFLOAT3 begin, DirectX::XMFLOAT3 end, EType easeType);
	DirectX::XMFLOAT4 Lerp(float progress, DirectX::XMFLOAT4 begin, DirectX::XMFLOAT4 end, EType easeType);

	float Ease(float progress, EType easeType);
}