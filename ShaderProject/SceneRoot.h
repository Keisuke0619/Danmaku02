#ifndef __SCENE_ROOT_H__
#define __SCENE_ROOT_H__

#include "SceneBase.hpp"
#include "EventCallback.h"

class SceneRoot : public SceneBase, IEventCallBack
{
public:
	enum ESceneID
	{
		SCENE_TITLE,
		SCENE_GAME,
		SCENE_RESULT,
		SCENE_NONE
	};
public:
	void Init();
	void Uninit();
	void Update(float tick);
	void Draw();
	void SetNextScene(ESceneID id, float delay = 0);
private:
	int m_index;
	ESceneID m_nextSceneID;
	float m_sceneDelay;
	float m_time;
	void SceneChange();
	//void DelayChange();

	// IEventCallBack ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	void CallBack(int eventID) override;
};

#endif // __SCENE_ROOT_H__