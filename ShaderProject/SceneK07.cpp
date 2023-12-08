#include "SceneK07.h"

#include "CameraBase.h"
#include "LightBase.h"
#include "Input.h"
#include "Sprite.h"
#include "Defines.h"
#include "Player.h"
#include "Object.h"
#include "FollowCamera.h"
#include "Input.h"
void SceneK07::Init()
{
	Shader* shader[] = {
		CreateObj<VertexShader>("VS_Object"),
		CreateObj<PixelShader>("PS_TexColor"),
	};
	const char* filePath[] = {
		"Assets/Shader/VS_Object.cso",
		"Assets/Shader/PS_TexColor.cso",
	};
	static_assert(_countof(shader) == _countof(filePath), "");
	for (int i = 0; i < _countof(shader); ++i)
		shader[i]->Load(filePath[i]);
	
	Sprite::SetVertexShader(nullptr);
	Sprite::SetPixelShader(nullptr);

	InitSky();
	auto pl = CreateObj<CPlayer>("Player");
	auto cam = CreateObj<CFollowCamera>("MainCamera");
	cam->SetTarget(pl);
	cam->SetPosOffset(DirectX::XMFLOAT3(0, 10, -3));
	cam->SetLookOffset(DirectX::XMFLOAT3(0, 0, 4));
}

void SceneK07::Uninit()
{
}

void SceneK07::Update(float tick)
{
	// DCCカメラと現在のメインカメラを取得
	CameraBase* cameraDCC = GetObj<CameraBase>("Camera");
	auto primary = CameraBase::GetPrimary();
	// DCCカメラがメインではないならアップデート。
	if (primary != cameraDCC || IsKeyPress(VK_RETURN))
	{
		CObjectManager::GetIns()->Update();
	}
#ifdef _DEBUG
	// CtrlでDCCカメラと普通のカメラをSwitch
	if (IsKeyTrigger(VK_CONTROL))
	{
		auto defaultCamera = GetObj<CameraBase>("MainCamera");
		CameraBase::SetPrimary(primary == cameraDCC ? defaultCamera : cameraDCC);
	}
#endif
}


void SceneK07::Draw()
{
	// スカイキューブ描画
	auto rtvDefault = GetObj<RenderTarget>("RTV");
	auto dsvDefault = GetObj<DepthStencil>("DSV");
	SetRenderTargets(1, &rtvDefault, nullptr);
	DrawSky();
	SetRenderTargets(1, &rtvDefault, dsvDefault);
	auto vs = GetObj<Shader>("VS_Object");
	auto ps = GetObj<Shader>("PS_TexColor");
	
	CObjectManager::GetIns()->Draw(vs, ps);

}


void SceneK07::InitSky()
{
	Texture* texes[] =
	{
		CreateObj<Texture>("Sky01"),
		CreateObj<Texture>("Sky02"),
		CreateObj<Texture>("Sky03"),
		CreateObj<Texture>("Sky04"),
		CreateObj<Texture>("Sky05"),
		CreateObj<Texture>("Sky06"),
	};
	const char* texPath[] =
	{
		"Assets/Texture/Sky/Sky01.tga",
		"Assets/Texture/Sky/Sky02.tga",
		"Assets/Texture/Sky/Sky03.tga",
		"Assets/Texture/Sky/Sky04.tga",
		"Assets/Texture/Sky/Sky05.tga",
		"Assets/Texture/Sky/Sky06.tga",
	};
	for (int i = 0; i < _countof(texes); ++i)
		texes[i]->Create(texPath[i]);
}

void SceneK07::DrawSky()
{
	DirectX::XMFLOAT4X4 mat[3] = {};
	mat[1] = CameraBase::GetPrimary()->GetView();
	mat[2] = CameraBase::GetPrimary()->GetProj();

	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetPixelShader(nullptr);
	Sprite::SetVertexShader(nullptr);

	Texture* skies[] = {
		GetObj<Texture>("Sky01"),
		GetObj<Texture>("Sky04"),
		GetObj<Texture>("Sky02"),
		GetObj<Texture>("Sky03"),
		GetObj<Texture>("Sky05"),
		GetObj<Texture>("Sky06"),
	};
	DirectX::XMFLOAT3 skyPos[] =
	{
		{0, -0.5f, 0},
		{0, 0.5f, 0},
		{0, 0, 0.5f},
		{0.5f, 0, 0},
		{0, 0, -0.5f},
		{-0.5f, 0, 0},
	};
	DirectX::XMFLOAT2 skyRot[] =
	{
		{3,1},
		{3,3},
		{0,0},
		{1,0},
		{2,0},
		{3,0},
	};
	Sprite::SetSize(1, 1);
	Sprite::SetOffset(0, 0);
	for (int i = 0; i < 6; i++)
	{
		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationX(skyRot[i].y * 3.14f / 2) * DirectX::XMMatrixRotationY(skyRot[i].x * 3.14f / 2) * DirectX::XMMatrixTranslation(skyPos[i].x, skyPos[i].y, skyPos[i].z) * DirectX::XMMatrixTranslation(CameraBase::GetPrimary()->GetPos().x, CameraBase::GetPrimary()->GetPos().y, CameraBase::GetPrimary()->GetPos().z)));
		Sprite::SetWorld(mat[0]);
		Sprite::SetTexture(skies[i]);
		Sprite::Draw();
	}
}
