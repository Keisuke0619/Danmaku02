#include "Fade.h"
#include "Sprite.h"
namespace Fade
{
	IEventCallBack* g_fadeCallBack = nullptr;
	int g_fadeFrames;
	float g_fadeAddAlpha;
	float g_fadeAlpha;
	Texture* g_fadeTex = nullptr;
}

void Fade::Start(IEventCallBack* callback, int frames)
{
	if (g_fadeCallBack != nullptr) { return; }
	g_fadeCallBack = callback;
	g_fadeFrames = frames;
	g_fadeAddAlpha = 1.0f / (g_fadeFrames - 1);
	g_fadeAlpha = 0.0f;
	if (g_fadeTex == nullptr)
	{
		g_fadeTex = new Texture();
		g_fadeTex->Create("Assets/Texture/black.png");
	}
}

void Fade::Release()
{
	delete g_fadeTex;
}

void Fade::Update()
{
	if (g_fadeFrames > 0)
	{
		g_fadeAlpha += g_fadeAddAlpha;
	}
	else if (g_fadeFrames == 0)
	{
		g_fadeCallBack->CallBack(EVENT_FADE_DONE);
		g_fadeCallBack = nullptr;
	}
	else
	{
		g_fadeAlpha -= g_fadeAddAlpha;
	}
	g_fadeFrames--;
}

void Fade::Draw()
{
	if (g_fadeAlpha <= 0.0f) { return; }
	Sprite::SetColor(1, 1, 1, g_fadeAlpha);
	Sprite::SetTexture(g_fadeTex);
	Sprite::SetSize(2, 2);
	Sprite::SetOffset(0, 0);
	Sprite::Draw();
	Sprite::Reset();
}
