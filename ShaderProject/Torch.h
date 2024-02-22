#pragma once

#include <Model.h>
#include "Effect.h"

class CTorch : public Model
{
public:
	static void SetTorch(std::string csvPath);
	void Update() override;
private:
	CTorch(int x, int y, int direction, DirectX::XMFLOAT3 col, float range);
	
	DirectX::XMFLOAT3 m_lightPos;
	Effekseer::Handle m_efk = -1;
};