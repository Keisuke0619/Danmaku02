#include "SceneTitle.h"
#include "Input.h"
#include "SceneRoot.h"
#include "Texture.h"
#include "SoundUtil.h"
void CSceneTitle::Init()
{
	m_back = new Texture();
	m_back->Create("Assets/Texture/UI/Title_back.png");
	Sound::FadeIn("testSine.wav", 1.0f, 0.2f, true);
}

void CSceneTitle::Uninit()
{
	Sound::FadeOut("testSine.wav", 1.0f);
}

void CSceneTitle::Update(float tick)
{
	if (IsKeyTrigger('K'))
	{
		Sound::FadeOut("testSine.wav", 1.0f);
	}
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
