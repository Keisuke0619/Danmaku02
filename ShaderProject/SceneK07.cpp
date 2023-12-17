#include "SceneK07.h"

#include "CameraBase.h"
#include "Input.h"
#include "Sprite.h"
#include "Defines.h"
#include "Player.h"
#include "FollowCamera.h"
#include "CollisionSystem.h"
#include "ShotObj.h"
#include "WorldSprite.h"
#include "Enemy.h"
#include "LoadStageData.h"
#include "Wall.h"
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
	CCollisionSystem::GetIns()->Create(6, -50, 100, 50, -100);

	auto pl = CreateObj<CPlayer>("Player");

	auto cam = CreateObj<CFollowCamera>("MainCamera");
	cam->SetTarget(pl);
	cam->SetPosOffset(DirectX::XMFLOAT3(0, 5, -5));
	cam->SetLookOffset(DirectX::XMFLOAT3(0, 0, 4));

	auto field = new CWorldSprite();
	field->LoadTexture("Assets/Texture/BayerMatrix.png");
	field->SetPos(DirectX::XMFLOAT3(0, 0, 0));
	field->SetRot(DirectX::XMFLOAT3(3.141592f / 2, 0, 0));
	field->SetScale(DirectX::XMFLOAT3(1000, 1000, 1));
	field->SetUVScale(DirectX::XMFLOAT2(25, 25));

	auto enemy = new CEnemy();
	enemy->SetPos(DirectX::XMFLOAT3(0, 1, 20));

	auto stageDatas = LoadStageData("Assets/CSV/MapTest.csv");
	for (auto data : *stageDatas)
	{
		auto tile = new CWall(data.x, data.y, data.id, 14,4);
	}
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
		CObjectManager::GetIns()->RemoveUpdate();
		if (IsKeyTrigger('R'))
		{
			auto enemy = new CEnemy();
			enemy->SetPos(DirectX::XMFLOAT3((rand() % 11) - 5, 1, 20 + (rand() % 20)));
		}
	}
	UpdateCamera();
	UpdateCollision();
	
}


void SceneK07::Draw()
{
	auto rtvDefault = GetObj<RenderTarget>("RTV");
	auto dsvDefault = GetObj<DepthStencil>("DSV");

	// スカイキューブ描画
	SetRenderTargets(1, &rtvDefault, nullptr);
	DrawSky();

	SetRenderTargets(1, &rtvDefault, dsvDefault);
	auto vs = GetObj<Shader>("VS_Object");
	auto ps = GetObj<Shader>("PS_TexColor");


	Sprite::SetView(CameraBase::GetPrimary()->GetView());
	Sprite::SetProjection(CameraBase::GetPrimary()->GetProj());
	CObjectManager::GetIns()->Draw(vs, ps, RENDER_MODEL);
	CObjectManager::GetIns()->Draw(vs, ps, RENDER_ALPHA);

}

void SceneK07::UpdateCamera()
{
	// DCCカメラと現在のメインカメラを取得
	CameraBase* cameraDCC = GetObj<CameraBase>("Camera");
	auto primary = CameraBase::GetPrimary();
	// DCCカメラがメインではないならアップデート。
	if (primary != cameraDCC || IsKeyPress(VK_RETURN))
	{
		CObjectManager::GetIns()->Update();
		CObjectManager::GetIns()->RemoveUpdate();
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

void SceneK07::UpdateCollision()
{
	// 当たり判定
	auto collisionList = CCollisionSystem::GetIns()->GetList();
	for (auto itr = collisionList->begin(); itr != collisionList->end(); itr++)
	{
		auto firstPos = itr->first->GetPos();
		auto secondPos = itr->second->GetPos();
		auto firstScale = itr->first->GetColliderScale();
		auto secondScale = itr->second->GetColliderScale();
		float xzPowLength = powf(firstPos.x - secondPos.x, 2) + powf(firstPos.z - secondPos.z, 2);
		float colliderPowLength = powf(firstScale + secondScale, 2);
		if (xzPowLength < colliderPowLength)
		{
			itr->first->OnCollision(itr->second);
			itr->second->OnCollision(itr->first);
		}
	}

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
	const DirectX::XMFLOAT3 skyPos[] =
	{
		{0, -0.5f, 0},
		{0, 0.5f, 0},
		{0, 0, 0.5f},
		{0.5f, 0, 0},
		{0, 0, -0.5f},
		{-0.5f, 0, 0},
	};
	const DirectX::XMFLOAT2 skyRot[] =
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
