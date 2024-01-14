#pragma once

#include "SceneBase.hpp"
#include "Model.h"
#include "EnemySpawner.h"
#include <list>
#include "EventCallback.h"

struct ModelData
{
	DirectX::XMFLOAT4X4 world;
	class Model* model;
};

class SceneK07 : public SceneBase, IEventCallBack
{
public:
	void Init();
	void Uninit();
	void Update(float tick);
	void Draw();

	void UpdateCamera();
	void UpdateEvent();
private:
	void InitSky();
	void DrawSky();
	CEnemySpawner *m_spawner;
	CObject* m_player;

	// IEventCallBack ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	void CallBack(int eventID) override;
};
