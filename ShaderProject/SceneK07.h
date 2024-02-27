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

	void InputPause();
	void UpdateCamera();
	void UpdateEvent();
private:
	void InitSky();
	void DrawSky();
	CEnemySpawner *m_spawner;
	CObject* m_player;
	bool m_pause = false;
	bool m_eventFlag = false;
	// IEventCallBack ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	void CallBack(int eventID) override;
};
