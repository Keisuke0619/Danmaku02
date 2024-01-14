#pragma once
#include <memory>


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

		SLOT_MAX
	};
	void Init();
	void Update();
	void SetData(ESlot slot, float data);
	void Draw();
}