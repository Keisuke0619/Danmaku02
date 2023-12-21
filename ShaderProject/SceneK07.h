#pragma once

#include "SceneBase.hpp"
#include "Model.h"

#include <list>

struct ModelData
{
	DirectX::XMFLOAT4X4 world;
	class Model* model;
};

class SceneK07 : public SceneBase
{
public:
	void Init();
	void Uninit();
	void Update(float tick);
	void Draw();

	void UpdateCamera();
private:
	void InitSky();
	void DrawSky();

};
