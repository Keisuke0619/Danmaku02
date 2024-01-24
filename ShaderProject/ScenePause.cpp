#include "ScenePause.h"
#include "Input.h"
#include "Sprite.h"
void CScenePause::Init()
{
	m_isOverlay = true;
	m_back = new Texture();
	m_back->Create("Assets/Texture/UI/Pause_Back.png");
}

void CScenePause::Uninit()
{
	delete m_back;
}

void CScenePause::Update(float tick)
{
	if (m_frame == 1)
	{
		return;
	}
	if (IsKeyTrigger(VK_ESCAPE))
	{
		m_callBack->CallBack(1);
	}
}

void CScenePause::Draw()
{
	Sprite::Reset();
	Sprite::SetTexture(m_back);
	Sprite::Draw();
	Sprite::Reset();
}

void CScenePause::SetCallBack(IEventCallBack* callBack)
{
	m_callBack = callBack;
}
