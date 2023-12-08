#include "DebugWindow.hpp"
#include <iostream>
#include <string>
FILE* CDebugWindow::m_fp = nullptr;
bool CDebugWindow::m_enable = true;

#ifndef _DEBUG
//#define DISABLE_DEBUG_WINDOW      // Releaseビルドでも表示させたい場合はこの行をコメントアウト
#endif // _DEBUG

void CDebugWindow::Create()
{
#ifndef DISABLE_DEBUG_WINDOW
    if (m_fp != nullptr)
        return;
    if (m_enable == false)
        return;
    AllocConsole();
    freopen_s(&m_fp, "CONOUT$", "w", stdout);
    freopen_s(&m_fp, "CONIN$", "r", stdin);

    // コンソールの名前変更
    SetConsoleTitle("DebugWindow");
#endif // _DEBUG
}

void CDebugWindow::Close()
{
#ifndef DISABLE_DEBUG_WINDOW
    if (m_fp == nullptr)
        return;
    auto hWnd = FindWindow(NULL, "DebugWindow");
    FreeConsole();
    fclose(m_fp);
    m_fp = nullptr;
#endif // _DEBUG
}

void CDebugWindow::Print(const char* src)
{
#ifndef DISABLE_DEBUG_WINDOW
    printf(src);
#endif // _DEBUG
}

void CDebugWindow::Print(std::string str)
{
#ifndef DISABLE_DEBUG_WINDOW
    str += '\n';
    printf(str.c_str());
#endif // _DEBUG

}

void CDebugWindow::Print(const char* label, int data)
{
#ifndef DISABLE_DEBUG_WINDOW
    char tmp[128] = {};
    sprintf(tmp, "%s\t:%d\n", label, data);
    printf(tmp);
#endif // _DEBUG
}

void CDebugWindow::Print(const char* label, float data)
{
#ifndef DISABLE_DEBUG_WINDOW
    char tmp[128] = {};
    sprintf(tmp, "%s\t:%.4f\n", label, data);
    printf(tmp);
#endif // _DEBUG
}


void CDebugWindow::Print(const char* label, DirectX::XMFLOAT3 data)
{
#ifndef DISABLE_DEBUG_WINDOW
    char tmp[128] = {};
    sprintf(tmp, "%s\t:( %9.4f, %9.4f, %9.4f )\n", label, data.x, data.y, data.z);
    printf(tmp);
#endif // _DEBUG
}

void CDebugWindow::Print(const char* label, DirectX::XMVECTOR data)
{
#ifndef DISABLE_DEBUG_WINDOW
    DirectX::XMFLOAT3 tmp;
    DirectX::XMStoreFloat3(&tmp, data);
    Print(label, tmp);
#endif // _DEBUG
}

void CDebugWindow::Print(const char* label, DirectX::XMMATRIX data)
{
#ifndef DISABLE_DEBUG_WINDOW
    char tmp[256] = {};
    sprintf(tmp, "%s\t:\n{ %9.4f,%9.4f,%9.4f,%9.4f\n  %9.4f,%9.4f,%9.4f,%9.4f\n  %9.4f,%9.4f,%9.4f,%9.4f\n  %9.4f,%9.4f,%9.4f,%9.4f }\n", label, 
        data.r[0].m128_f32[0], data.r[1].m128_f32[0], data.r[2].m128_f32[0], data.r[3].m128_f32[0],
        data.r[0].m128_f32[1], data.r[1].m128_f32[1], data.r[2].m128_f32[1], data.r[3].m128_f32[1],
        data.r[0].m128_f32[2], data.r[1].m128_f32[2], data.r[2].m128_f32[2], data.r[3].m128_f32[2],
        data.r[0].m128_f32[3], data.r[1].m128_f32[3], data.r[2].m128_f32[3], data.r[3].m128_f32[3]
        );
    printf(tmp);
#endif // _DEBUG
}
