#include "DebugText.h"
#include "Sprite.h"
#include "Input.h"
namespace DebugText
{
	const std::string Header[SLOT_MAX] =
	{
		"FPS     :",
		"PosX    :",
		"PosY    :",
		"Objects :",
		"ColList :",
		"ColCount:",
		"Score   :"
	};
	float g_datas[SLOT_MAX] = 
	{
		0,
		0,
		0,
		0,
		0,
		0,
		0,
	};
	Texture* g_texture;
	bool g_enable = false;
	const float uvSize = 1.0f / 16;
	float g_fontScale = 0.06f;
}
void DebugText::Init()
{
	g_texture = new Texture;
	g_texture->Create("Assets/Texture/font.png");
}
void DebugText::Update()
{
	if (IsKeyTrigger(VK_F3))
	{
		g_enable = !g_enable;
	}
}
void DebugText::SetData(ESlot slot, float data)
{
	g_datas[slot] = data;
}

void DebugText::Draw()
{
	if (g_enable == false)
	{
		return;
	}
	const float fontSize = 1.0f;
	const float anchorX = -0.95f;
	const float anchorY = 0.95f;
	float posX = anchorX;
	float posY = anchorY;
	// make string
	std::string str = {};
	for (int i = 0; i < SLOT_MAX; i++)
	{
		str += Header[i] + std::to_string(g_datas[i]) + "\n";
	}
	const char* ptr = str.c_str();
	
	StartDrawString(fontSize);

	while (*ptr != '\0')
	{
		char uvX = *ptr & 0xf;
		char uvY = (*ptr & 0xf0) >> 4;
		Sprite::SetUVPos(uvX * uvSize, uvY * uvSize);
		Sprite::SetOffset(posX, posY);
		posX += g_fontScale * 0.4f;
		if (*ptr == '\n')
		{
			posX = anchorX;
			posY -= g_fontScale;
		}
		Sprite::Draw();
		ptr++;
	}

	EndDrawString();
}

void DebugText::StartDrawString(float fontScale)
{
	const float FontScaleCoef = 0.06f;

	g_fontScale = fontScale * FontScaleCoef;
	Sprite::Reset();
	Sprite::SetTexture(g_texture);
	Sprite::SetUVScale(uvSize, uvSize);
	Sprite::SetSize(g_fontScale * 0.75f, g_fontScale);
}

void DebugText::DrawString(float posX, float posY, std::string str)
{
	const char* ptr = str.c_str();

	while (*ptr != '\0')
	{
		char uvX = *ptr & 0xf;
		char uvY = (*ptr & 0xf0) >> 4;
		Sprite::SetUVPos(uvX * uvSize, uvY * uvSize);
		Sprite::SetOffset(posX, posY);
		posX += g_fontScale * 0.5f;
		Sprite::Draw();
		ptr++;
	}
}

void DebugText::EndDrawString()
{
	Sprite::SetOffset(0, 0);
	Sprite::SetUVPos(0, 0);
	Sprite::SetUVScale(1, 1);
	Sprite::SetSize(1, 1);
	Sprite::SetTexture(nullptr);
}
