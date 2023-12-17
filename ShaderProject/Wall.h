#pragma once
#include <Model.h>

class CWall : public Model
{
public:
	CWall(int x, int y, int id, float offsetX, float offsetY);
	void Update() override;
};