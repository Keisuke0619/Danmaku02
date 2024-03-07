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





//--- グローバル変数
BYTE g_keyTable[256];
BYTE g_oldTable[256];
JOYINFOEX g_joyPad;
JOYINFOEX g_oldJoyPad;

HRESULT InitInput()
{
	// 一番最初の入力
	GetKeyboardState(g_keyTable);
	// コントローラーの設定
	g_joyPad.dwSize = sizeof(JOYINFOEX);
	g_joyPad.dwFlags = JOY_RETURNALL;
	return S_OK;
}
void UninitInput()
{
}
void UpdateInput()
{
	// 古い入力を更新
	memcpy_s(g_oldTable, sizeof(g_oldTable), g_keyTable, sizeof(g_keyTable));
	memcpy_s(&g_oldJoyPad, sizeof(g_oldJoyPad), &g_joyPad, sizeof(g_joyPad));
	// 現在の入力を取得
	GetKeyboardState(g_keyTable);
	// コントローラー入力
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
	DirectX::XMFLOAT2 ret;	// 戻り値用
	// それぞれの軸の値が0x0000～0xffffで帰ってくる。
	// 使用する際は-1～1のほうが扱いやすいので変形させる。
	// まずは軸の値を0～2にするために0x7fffで割る。（片方Floatにすることで結果もFloatになり、小数点以下の数値を受け取る）
	// その後、-1することで-1～1になる
	ret.x = ((float)g_joyPad.dwXpos / 0x7fff) - 1.0f;
	ret.y = ((float)g_joyPad.dwYpos / 0x7fff) - 1.0f;
	// Y軸は上が-1下が+1になっている。使いづらいので正負を逆転させる。
	ret.y *= -1.0f;
	// 浮動小数点数は精度の問題がある＆スティックに触れていなくても若干入力があるので、しきい値で０にする。
	if (fabsf(ret.x) + fabsf(ret.y) < INPUT_STICK_THRESHOLD) { ret.x = 0; ret.y = 0; }
	return ret;
}

DirectX::XMFLOAT2 GetRightStick()
{
	// 内容はGetLeftStickとほぼ同じ。
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
