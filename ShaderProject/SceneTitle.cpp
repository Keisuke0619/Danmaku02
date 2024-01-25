#include "SceneTitle.h"
#include "Input.h"
#include "SceneRoot.h"
#include "Texture.h"
void CSceneTitle::Init()
{
	m_back = new Texture();
	m_back->Create("Assets/Texture/UI/Title_back.png");
}

void CSceneTitle::Uninit()
{
}

void CSceneTitle::Update(float tick)
{
	if (IsKeyTrigger(VK_RETURN))
	{
		((SceneRoot*)m_pParent)->SetNextScene(SceneRoot::SCENE_GAME);
	}
}

void CSceneTitle::Draw()
{
	Sprite::SetTexture(m_back);
	Sprite::SetSize(2, 2);
	Sprite::Draw();
}
