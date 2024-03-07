#include "Input.h"
#include <Windows.h>
#pragma comment(lib, "winmm.lib")

#define INPUT_STICK_THRESHOLD (0.3f)

struct TInputKeyAndPad
{
	BYTE key;
	long pad;
};

const static TInputKeyAndPad KeyAndPad[INPUT_ENUM_MAX] = 
{
	{VK_RETURN, PAD_A},
	{VK_ESCAPE, PAD_B},
	{'Z', PAD_A},
	{VK_SHIFT, PAD_L1},
	{VK_ESCAPE, PAD_START},
};





//--- �O���[�o���ϐ�
BYTE g_keyTable[256];
BYTE g_oldTable[256];
JOYINFOEX g_joyPad;
JOYINFOEX g_oldJoyPad;

HRESULT InitInput()
{
	// ��ԍŏ��̓���
	GetKeyboardState(g_keyTable);
	// �R���g���[���[�̐ݒ�
	g_joyPad.dwSize = sizeof(JOYINFOEX);
	g_joyPad.dwFlags = JOY_RETURNALL;
	return S_OK;
}
void UninitInput()
{
}
void UpdateInput()
{
	// �Â����͂��X�V
	memcpy_s(g_oldTable, sizeof(g_oldTable), g_keyTable, sizeof(g_keyTable));
	memcpy_s(&g_oldJoyPad, sizeof(g_oldJoyPad), &g_joyPad, sizeof(g_joyPad));
	// ���݂̓��͂��擾
	GetKeyboardState(g_keyTable);
	// �R���g���[���[����
	joyGetPosEx(0, &g_joyPad);

}

bool IsKeyPress(BYTE key)
{
	return g_keyTable[key] & 0x80;
}
bool IsKeyTrigger(BYTE key)
{
	return (g_keyTable[key] ^ g_oldTable[key]) & g_keyTable[key] & 0x80;
}
bool IsKeyRelease(BYTE key)
{
	return (g_keyTable[key] ^ g_oldTable[key]) & g_oldTable[key] & 0x80;
}
bool IsKeyRepeat(BYTE key)
{
	return false;
}

bool IsPadPress(long padKey)
{
	return g_joyPad.dwButtons & padKey;
}

bool IsPadTrigger(long padKey)
{
	return (g_joyPad.dwButtons ^ g_oldJoyPad.dwButtons) & g_joyPad.dwButtons & padKey;
}

bool IsPadRelease(long padKey)
{
	return (g_joyPad.dwButtons ^ g_oldJoyPad.dwButtons) & g_oldJoyPad.dwButtons & padKey;
}

bool IsPadRepeat(long padKey)
{
	return false;
}

DirectX::XMFLOAT2 GetLeftStick()
{
	DirectX::XMFLOAT2 ret;	// �߂�l�p
	// ���ꂼ��̎��̒l��0x0000�`0xffff�ŋA���Ă���B
	// �g�p����ۂ�-1�`1�̂ق��������₷���̂ŕό`������B
	// �܂��͎��̒l��0�`2�ɂ��邽�߂�0x7fff�Ŋ���B�i�Е�Float�ɂ��邱�ƂŌ��ʂ�Float�ɂȂ�A�����_�ȉ��̐��l���󂯎��j
	// ���̌�A-1���邱�Ƃ�-1�`1�ɂȂ�
	ret.x = ((float)g_joyPad.dwXpos / 0x7fff) - 1.0f;
	ret.y = ((float)g_joyPad.dwYpos / 0x7fff) - 1.0f;
	// Y���͏オ-1����+1�ɂȂ��Ă���B�g���Â炢�̂Ő������t�]������B
	ret.y *= -1.0f;
	// ���������_���͐��x�̖�肪���違�X�e�B�b�N�ɐG��Ă��Ȃ��Ă��኱���͂�����̂ŁA�������l�łO�ɂ���B
	if (fabsf(ret.x) + fabsf(ret.y) < INPUT_STICK_THRESHOLD) { ret.x = 0; ret.y = 0; }
	return ret;
}

DirectX::XMFLOAT2 GetRightStick()
{
	// ���e��GetLeftStick�Ƃقړ����B
	DirectX::XMFLOAT2 ret;
	ret.x = ((float)g_joyPad.dwXpos / 0x7fff) - 1.0f;
	ret.y = ((float)g_joyPad.dwYpos / 0x7fff) - 1.0f;
	ret.y *= -1.0f;
	if (fabsf(ret.x) + fabsf(ret.y) < INPUT_STICK_THRESHOLD) { ret.x = 0; ret.y = 0; }
	return ret;
}

bool IsPress(EInputPair pair)
{
	return IsKeyPress(KeyAndPad[pair].key) || IsPadPress(KeyAndPad[pair].pad);
}

bool IsTrigger(EInputPair pair)
{
	return IsKeyTrigger(KeyAndPad[pair].key) || IsPadTrigger(KeyAndPad[pair].pad);
}

bool IsRelease(EInputPair pair)
{
	return IsKeyRelease(KeyAndPad[pair].key) || IsPadRelease(KeyAndPad[pair].pad);
}

bool IsRepeat(EInputPair pair)
{
	return false;
}
