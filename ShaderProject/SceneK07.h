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

enum EIconID
{
	ICON_KEY_ARROW,
	ICON_KEY_ESC,
	ICON_KEY_SHIFT,
	ICON_KEY_Z,
	ICON_PAD_A,
	ICON_PAD_B,
	ICON_PAD_X,
	ICON_PAD_Y,
	ICON_PAD_START,
	ICON_PAD_LB,
	ICON_PAD_STICK_L,

	ICON_MAX
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
	void DrawInfo();


	CEnemySpawner *m_spawner;
	CObject* m_player;
	bool m_pause = false;
	bool m_eventFlag = false;
	bool m_isEvent = false;
	Texture* m_icons[ICON_MAX];
	Texture* m_infoBack;
	// IEventCallBack ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	void CallBack(int eventID) override;
};
