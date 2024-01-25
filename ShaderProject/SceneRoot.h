#ifndef __SCENE_ROOT_H__
#define __SCENE_ROOT_H__

#include "SceneBase.hpp"
#include "EventCallback.h"

class SceneRoot : public SceneBase
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
	void SetNextScene(ESceneID id);

private:
	int m_index;
	ESceneID m_nextSceneID;
	void SceneChange();
};

#endif // __SCENE_ROOT_H__