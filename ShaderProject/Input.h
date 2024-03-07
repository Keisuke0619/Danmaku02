#ifndef __INPUT_H__
#define __INPUT_H__

#include <Windows.h>
#include <DirectXMath.h>
#undef max
#undef min

#define PAD_A	(0x0001)
#define PAD_B	(0x0002)
#define PAD_X	(0x0004)
#define PAD_Y	(0x0008)
#define PAD_LB	(0x0010)
#define PAD_L1	(0x0010)
#define PAD_RB	(0x0010)
#define PAD_R1	(0x0010)
#define PAD_SELECT	(0x0040)
#define PAD_START	(0x0080)
#define PAD_LS	(0x0100)
#define PAD_L3	(0x0100)
#define PAD_RS	(0x0200)
#define PAD_R3	(0x0200)

enum EInputPair
{
	INPUT_SELECT,
	INPUT_CANCEL,
	INPUT_SHOT,
	INPUT_SLOW,
	INPUT_PAUSE,

	INPUT_ENUM_MAX
};

HRESULT InitInput();
void UninitInput();
void UpdateInput();

bool IsKeyPress(BYTE key);
bool IsKeyTrigger(BYTE key);
bool IsKeyRelease(BYTE key);
bool IsKeyRepeat(BYTE key);

bool IsPadPress(long padKey);
bool IsPadTrigger(long padKey);
bool IsPadRelease(long padKey);
bool IsPadRepeat(long padKey);
DirectX::XMFLOAT2 GetLeftStick();
DirectX::XMFLOAT2 GetRightStick();

bool IsPress(EInputPair pair);
bool IsTrigger(EInputPair pair);
bool IsRelease(EInputPair pair);
bool IsRepeat(EInputPair pair);
#endif // __INPUT_H__