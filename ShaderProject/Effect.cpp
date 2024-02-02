#include "Effect.h"
#include <EffekseerRendererDX11.h>
#include <map>
#include "CameraBase.h"
#include "DirectX.h"
#include "Defines.h"

#ifdef _DEBUG
#pragma comment(lib, "EffekseerLib/EffekseerRendererDX11d.lib")
#pragma comment(lib, "EffekseerLib/Effekseerd.lib")
#else
#pragma comment(lib, "EffekseerLib/EffekseerRendererDX11.lib")
#pragma comment(lib, "EffekseerLib/Effekseer.lib")

#endif
using namespace Effekseer;

namespace Efk
{
	ManagerRef g_efkManager;
	EffekseerRendererDX11::RendererRef g_efkRender;
	std::map<const char16_t*, EffectRef> g_effects;
	float g_efkTime = 0;
	Matrix44 GetMat(DirectX::XMFLOAT4X4 mat);

}
void Efk::Init()
{
	g_efkManager = Manager::Create(1024);
	auto grpDevice = EffekseerRendererDX11::CreateGraphicsDevice(GetDevice(), GetContext());
	g_efkRender = EffekseerRendererDX11::Renderer::Create(grpDevice, 1024);
	g_efkManager->SetSpriteRenderer(g_efkRender->CreateSpriteRenderer());
	g_efkManager->SetRibbonRenderer(g_efkRender->CreateRibbonRenderer());
	g_efkManager->SetRingRenderer(g_efkRender->CreateRingRenderer());
	g_efkManager->SetTrackRenderer(g_efkRender->CreateTrackRenderer());
	g_efkManager->SetModelRenderer(g_efkRender->CreateModelRenderer());

	// Specify a texture, model, curve and material loader
	// It can be extended by yourself. It is loaded from a file on now.
	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	g_efkManager->SetTextureLoader(g_efkRender->CreateTextureLoader());
	g_efkManager->SetModelLoader(g_efkRender->CreateModelLoader());
	g_efkManager->SetMaterialLoader(g_efkRender->CreateMaterialLoader());
	g_efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}

void Efk::Uninit()
{

}

void Efk::Update(float tick)
{
	g_efkRender->SetTime(g_efkTime+=tick);
}

void Efk::Draw()
{
	auto pos = CameraBase::GetPrimary()->GetPos();
	auto viewerPosition = Vector3D(pos.x, pos.y, pos.z);
	Matrix44 projectionMatrix;
	projectionMatrix.PerspectiveFovRH(90.0f / 180.0f * 3.14f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 500.0f);

	Matrix44 cameraMatrix;
	cameraMatrix.LookAtRH(viewerPosition, Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));


	Effekseer::Manager::LayerParameter layerParameter;
	layerParameter.ViewerPosition = viewerPosition;
	g_efkManager->SetLayerParameter(0, layerParameter);

	g_efkRender->SetProjectionMatrix(projectionMatrix);
	g_efkRender->SetCameraMatrix(cameraMatrix);

	g_efkRender->BeginRendering();

	// Render effects
	// エフェクトの描画を行う。
	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = 0.0f;
	drawParameter.ZFar = 1;
	drawParameter.ViewProjectionMatrix = g_efkRender->GetCameraProjectionMatrix();
	g_efkManager->Draw(drawParameter);

	// Finish to rendering effects
	// エフェクトの描画終了処理を行う。
	g_efkRender->EndRendering();
}

Effekseer::Handle Efk::Play(const char16_t* path, float x, float y, float z)
{
	if (g_effects.count(path) == 0)
	{
		g_effects[path] = Effect::Create(g_efkManager, path);
	}
	auto ret = g_efkManager->Play(g_effects[path], x, y, z);
	g_efkManager->SetRotation(ret, 0, 0, 0);
	g_efkManager->SetScale(ret, 100, 100, 100);
	return ret;
}



// util
Matrix44 Efk::GetMat(DirectX::XMFLOAT4X4 in)
{
	Matrix44 out;
	out.Values[0][0] = in._11;
	out.Values[1][0] = in._12;
	out.Values[2][0] = in._13;
	out.Values[3][0] = in._14;
	out.Values[0][1] = in._21;
	out.Values[1][1] = in._22;
	out.Values[2][1] = in._23;
	out.Values[3][1] = in._24;
	out.Values[0][2] = in._31;
	out.Values[1][2] = in._32;
	out.Values[2][2] = in._33;
	out.Values[3][2] = in._34;
	out.Values[0][3] = in._41;
	out.Values[1][3] = in._42;
	out.Values[2][3] = in._43;
	out.Values[3][3] = in._44;
	return out;
}