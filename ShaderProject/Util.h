#pragma once

#include <DirectXMath.h>
namespace Util
{
	enum EEaseType
	{
		TYPE_LINEAR,
		TYPE_IN_SINE,
		TYPE_OUT_SINE,
		TYPE_INOUT_SINE,

		TYPE_MAX
	};
	float Lerp(float progress, float begin, float end, EEaseType easeType);
	DirectX::XMFLOAT2 Lerp(float progress, DirectX::XMFLOAT2 begin, DirectX::XMFLOAT2 end, EEaseType easeType);
	DirectX::XMFLOAT3 Lerp(float progress, DirectX::XMFLOAT3 begin, DirectX::XMFLOAT3 end, EEaseType easeType);
	DirectX::XMFLOAT4 Lerp(float progress, DirectX::XMFLOAT4 begin, DirectX::XMFLOAT4 end, EEaseType easeType);
	float Ease(float progress, EEaseType easeType);

	float Rand();
	float Rand(float min, float max);
}