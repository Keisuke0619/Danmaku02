#pragma once
#include <string>
#include <DirectXMath.h>
#include <Effekseer.h>

namespace Efk
{
	void Init();
	void Uninit();
	void Update(float tick);
	void Draw();
	Effekseer::Handle Play(const char16_t* path, float x, float y, float z);
	//Effekseer::Handle Play(const char16_t* path, DirectX::XMFLOAT3 pos) { return Play(path, pos.x, pos.y, pos.z); }
}

