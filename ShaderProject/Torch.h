#pragma once

#include <Model.h>
class CTorch : public Model
{
public:
	static void SetTorch(std::string csvPath);
private:
	CTorch(int x, int y, int direction, DirectX::XMFLOAT3 col);
};