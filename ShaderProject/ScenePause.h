#pragma once

#include "SceneBase.hpp"
#include "EventCallback.h"
#include "Texture.h"
class CScenePause : public SceneBase
{
public:
	void Init() override;
	void Uninit() override;
	void Update(float tick) override;
	void Draw() override;
	void SetCallBack(IEventCallBack* callBack);
private:
	IEventCallBack* m_callBack;
	Texture* m_back;
};