#include "Main.h"
#include <memory>
#include "DirectX.h"
#include "Geometory.h"
#include "Sprite.h"
#include "Input.h"
#include "SceneRoot.h"
#include "Defines.h"
#include "DebugText.h"
#include "Effect.h"
#include "SoundUtil.h"
#include "Fade.h"
//--- グローバル変数
std::shared_ptr<SceneRoot> g_pScene;
HRESULT Init(HWND hWnd, UINT width, UINT height)
{
	HRESULT hr;
	hr = InitDirectX(hWnd, width, height, false);
	if (FAILED(hr)) { return hr; }
	Geometory::Init();
	Sprite::Init();
	InitInput();

	// シーン作成
	g_pScene = std::make_shared<SceneRoot>();
	g_pScene->Init();

	// 初期リソース作成
	auto rtv = g_pScene->CreateObj<RenderTarget>("RTV");
	rtv->CreateFromScreen();
	auto dsv = g_pScene->CreateObj<DepthStencil>("DSV");
	hr = dsv->Create(width, height, false);

	SetRenderTargets(1, &rtv, dsv);
	// debug text
	DebugText::Init();
	Efk::Init();

	return hr;
}

void Uninit()
{
	Fade::Release();
	Sound::ReleaseAll();
	Efk::Uninit();
	g_pScene->Uninit();
	g_pScene.reset();
	UninitInput();
	Sprite::Uninit();
	Geometory::Uninit();
	UninitDirectX();
}

void Update(float tick)
{
	UpdateInput();
	g_pScene->_update(tick);
	DebugText::Update();
	Efk::Update(tick);
	Sound::Update(tick);
	Fade::Update();
}

void Draw()
{
	auto rtv = g_pScene->GetObj<RenderTarget>("RTV");
	auto dsv = g_pScene->GetObj<DepthStencil>("DSV");
	float color[4] = { 0.005f, 0.005f, 0.01f, 1.0f };
	GetContext()->ClearRenderTargetView(rtv->GetView(), color);
	GetContext()->ClearDepthStencilView(dsv->GetView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	g_pScene->_draw();
	DebugText::Draw();
	SetRenderTargets(1, &rtv, nullptr);
	Fade::Draw();
	SwapDirectX();
}

// EOF