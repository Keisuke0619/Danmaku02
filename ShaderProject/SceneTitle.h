#pragma once

#include <SceneBase.hpp>
#include <Sprite.h>
class CSceneTitle : public SceneBase
{
public:
	void Init() override;
	void Uninit() override;
	void Update(float tick) override;
	void Draw() override;
private:
	Texture* m_back;
};