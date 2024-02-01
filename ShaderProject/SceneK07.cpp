#include "SceneK07.h"

#include "Pipeline.h"
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
#include "EventCamera.h"
#include "DataPool.h"
#include "BossStage01.h"
#include "ScenePause.h"
#include "Torch.h"
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

	m_player = new CPlayer;
	auto cam = CreateObj<CFollowCamera>("MainCamera");
	cam->SetTarget(m_player);
	cam->SetPosOffset(DirectX::XMFLOAT3(0, 10, -5));
	cam->SetLookOffset(DirectX::XMFLOAT3(0, 0, 4));
	//auto field = new CWorldSprite();
	//field->LoadTexture("Assets/Texture/Ground.jpg");
	//field->SetPos(DirectX::XMFLOAT3(0, 0, 0));
	//field->SetRot(DirectX::XMFLOAT3(3.141592f / 2, 0, 0));
	//field->SetScale(DirectX::XMFLOAT3(1000, 1000, 1));
	//field->SetUVScale(DirectX::XMFLOAT2(25, 25));
	auto field = new Model();
	field->Load("Assets/Model/Wall/Ground.fbx", 1000, false, {0, -5.5f, 0});
	field->SetAutoHidden(false);
	//CreateStage("Assets/CSV/MapTest.csv", 14, 4);
	CreateStage("Assets/CSV/Map01.csv", 10, 2);
	//CreateStage("Assets/CSV/Map02.csv", 4, 3);
	
	m_spawner = new CEnemySpawner("Assets/CSV/EnemyTest.csv");
	PipelineInit();
	CTorch::SetTorch("Assets/CSV/PointLight01.csv");
}

void SceneK07::Uninit()
{
	PipelineUninit();
	delete m_spawner;
	MeshPool::Ins()->DeleteAll();
	delete CCollisionSystem::GetIns();
	CWorldSprite::ReleaseTexture();
	DataPool::ClearAll();
}

void SceneK07::Update(float tick)
{
	if (m_pause)
	{
		return;
	}

	InputPause();

	// DCCカメラと現在のメインカメラを取得
	CameraBase* cameraDCC = GetObj<CameraBase>("Camera");
	auto primary = CameraBase::GetPrimary();
	
	UpdateCamera();
	UpdateEvent();
	if (m_player)
		m_spawner->Update(DirectX::XMFLOAT2(m_player->GetPos().x, m_player->GetPos().z));

	CObjectManager::GetIns()->Update();
	CObjectManager::GetIns()->RemoveUpdate();

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

	PipelineDraw(rtvDefault, dsvDefault);
	

}

void SceneK07::InputPause()
{
	if (m_pSubScene)
	{
		RemoveSubScene();
		return;
	}
	if (IsKeyTrigger(VK_ESCAPE))
	{
		auto scene = AddSubScene<CScenePause>();
		scene->SetCallBack(this);
		m_pause = true;
		return;
	}
}

void SceneK07::UpdateCamera()
{
	// DCCカメラと現在のメインカメラを取得
	CameraBase* cameraDCC = GetObj<CameraBase>("Camera");
	auto primary = CameraBase::GetPrimary();
	// CtrlでDCCカメラと普通のカメラをSwitch
	if (IsKeyTrigger(VK_CONTROL))
	{
		auto defaultCamera = GetObj<CameraBase>("MainCamera");
		CameraBase::SetPrimary(primary == cameraDCC ? defaultCamera : cameraDCC);
	}
	
}

void SceneK07::UpdateEvent()
{
	static bool testOnceFlag = false;
	if(testOnceFlag == false && m_player && m_player->GetPos().z > 44 * WALL_SCALE)
	{
		auto evCam = new CEventCamera("Assets/CSV/EventCamera.csv");
		evCam->AddCallBack(this);
		testOnceFlag = true;
		new CBossStage01();
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
	return;
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

void SceneK07::CallBack(int eventID)
{
	if (eventID == EVENT_FINISH_EVENT_CAMERA)
	{
		auto defaultCamera = GetObj<CameraBase>("MainCamera");
		CameraBase::SetPrimary(defaultCamera);
		defaultCamera->SetFovY(90);
	}
	if (eventID == EVENT_CLOSE_PAUSE_WINDOW)
	{
		m_pause = false;
	}
}

