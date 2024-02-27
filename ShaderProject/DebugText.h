#pragma once
#include <memory>
#include <string>

namespace DebugText
{
	/*
	FPS : xxx
	PLX : -xxx.xxx
	PLY :  xxx.xxx
	OBJ	: xxx
	*/
	enum ESlot
	{
		SLOT_FPS,
		SLOT_PLAYER_POS_X,
		SLOT_PLAYER_POS_Y,
		SLOT_OBJECTS,
		SLOT_COLLISION,
		SLOT_PLAYER_COLLIDED,
		SLOT_SCORE,

		SLOT_MAX
	};
	void Init();
	void Update();
	void SetData(ESlot slot, float data);
	void Draw();

	void StartDrawString(float fontScale);
	void DrawString(float x, float y, std::string str);
	void EndDrawString();
}