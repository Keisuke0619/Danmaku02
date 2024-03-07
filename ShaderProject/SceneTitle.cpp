#include "SceneTitle.h"
#include "Input.h"
#include "SceneRoot.h"
#include "Texture.h"
#include "SoundUtil.h"
#include "ObjectManager.h"
void CSceneTitle::Init()
{
	m_back = new Texture();
	m_back->Create("Assets/Texture/UI/Title_back.png");
	Sound::FadeIn("Babyleaf.wav", 1.0f, 0.2f, true);
}

void CSceneTitle::Uninit()
{
}

void CSceneTitle::Update(float tick)
{
	if (IsTrigger(INPUT_SELECT))
	{
		((SceneRoot*)m_pParent)->SetNextScene(SceneRoot::SCENE_GAME);
		Sound::FadeOut("Babyleaf.wav", 1.0f);
	}
	
}

void CSceneTitle::Draw()
{
	Sprite::SetTexture(m_back);
	Sprite::SetSize(2, 2);
	Sprite::Draw();
}
