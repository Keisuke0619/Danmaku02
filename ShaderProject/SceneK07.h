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

	void DrawContent(std::list<struct ModelData> models, class Shader* vs, class Shader* ps, DirectX::XMFLOAT4X4 *mat);
private:
	ModelData CreateData(class Model* model, DirectX::XMFLOAT3 pos = { 0,0,0 }, DirectX::XMFLOAT3 rot = { 0,0,0 }, DirectX::XMFLOAT3 scale = { 1,1,1 });
	void InitSky();
	void DrawSky();

};
