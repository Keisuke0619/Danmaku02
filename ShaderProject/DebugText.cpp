#include "DebugText.h"
#include "Sprite.h"
#include "Input.h"
namespace DebugText
{
	const std::string Header[SLOT_MAX] =
	{
		"FPS    :",
		"PosX   :",
		"PosY   :",
		"Objects:",
		"ColList:",
	};
	float g_datas[SLOT_MAX] = 
	{
		0,
		0,
		0,
		0,
		0,
	};
	Texture* g_texture;
	bool g_enable = false;
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
	const float uvSize = 1.0f / 16;
	const float fontSize = 0.06f;
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
	Sprite::Reset();
	Sprite::SetTexture(g_texture);
	Sprite::SetUVScale(uvSize, uvSize);
	Sprite::SetSize(fontSize, fontSize);
	while (*ptr != '\0')
	{
		char uvX = *ptr & 0xf;
		char uvY = (*ptr & 0xf0) >> 4;
		Sprite::SetUVPos(uvX * uvSize, uvY * uvSize);
		Sprite::SetOffset(posX, posY);
		posX += fontSize * 0.4f;
		if (*ptr == '\n')
		{
			posX = anchorX;
			posY -= fontSize;
		}
		Sprite::Draw();
		ptr++;
	}
	Sprite::SetOffset(0, 0);
	Sprite::SetUVPos(0, 0);
	Sprite::SetUVScale(1, 1);
	Sprite::SetSize(1, 1);
	Sprite::SetTexture(nullptr);
}
