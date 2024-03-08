#include "SceneK07.h"

#include "BossStage01.h"
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
#include "ScenePause.h"
#include "Torch.h"
#include "SoundUtil.h"
#include "DebugText.h"
#include "Score.h"

#include "Effect.h"



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

	m_player = new CPlayer;

	auto cam = CreateObj<CFollowCamera>("MainCamera");
	cam->SetTarget(m_player);
	cam->SetPosOffset(DirectX::XMFLOAT3(0, 10, -5));
	cam->SetLookOffset(DirectX::XMFLOAT3(0, 0, 4));

	auto field = new Model();
	field->Load("Assets/Model/Wall/Ground.fbx", 1000, false, {0, -5.5f, 0});
	field->SetAutoHidden(false);
	CreateStage("Assets/CSV/Map01.csv", 10, 2);
	
	m_spawner = new CEnemySpawner("Assets/CSV/EnemyTest.csv");
	PipelineInit();
	CTorch::SetTorch("Assets/CSV/PointLight01.csv");
	Sound::Play("RASHOMON.wav", true);
	Sound::SetVolume("RASHOMON.wav", 0.4f);

	// �A�C�R���ǂݍ���
	const char* IconPath[ICON_MAX] =
	{
		"Assets/Texture/Button/keyboard_arrows.png",
		"Assets/Texture/Button/keyboard_escape.png",
		"Assets/Texture/Button/keyboard_shift.png",
		"Assets/Texture/Button/keyboard_z.png",
		"Assets/Texture/Button/xbox_button_color_a.png",
		"Assets/Texture/Button/xbox_button_color_b.png",
		"Assets/Texture/Button/xbox_button_color_x.png",
		"Assets/Texture/Button/xbox_button_color_y.png",
		"Assets/Texture/Button/xbox_button_start_icon.png",
		"Assets/Texture/Button/xbox_lb.png",
		"Assets/Texture/Button/xbox_stick_l.png",
	};
	for (int i = 0; i < ICON_MAX; i++)
	{
		m_icons[i] = new Texture;
		m_icons[i]->Create(IconPath[i]);
	}
	m_infoBack = new Texture;
	m_infoBack->Create("Assets/Texture/UI/Info_Back.png");
}

void SceneK07::Uninit()
{
	PipelineUninit();
	delete m_spawner;
	MeshPool::Ins()->DeleteAll();
	delete CCollisionSystem::GetIns();
	CWorldSprite::ReleaseTexture();
	DataPool::ClearAll();
	Sound::FadeOut("Coolness.wav", 1.0f);
	for (int i = 0; i < ICON_MAX; i++)
	{
		delete m_icons[i];
	}
}

void SceneK07::Update(float tick)
{
	// �|�[�Y���Ȃ�A�b�v�f�[�g���Ȃ��B
	if (m_pause)
	{
		return;
	}
	// �|�[�Y�J����������肷�鏈��
	InputPause();

	// DCC�J�����ƌ��݂̃��C���J�������擾
	CameraBase* cameraDCC = GetObj<CameraBase>("Camera");
	auto primary = CameraBase::GetPrimary();
	// DCC�J�����ƃ��C���J�����̃X���b�v�Ƃ��B
	UpdateCamera();
	// �J�����C�x���g�Ƃ�
	UpdateEvent();
	// �G�����I�u�W�F�N�g�̃A�b�v�f�[�g�B�v���C���[�̍��W�𗘗p���邽�߁A�v���C���[�����݂��邩���`�F�b�N����B
	if (m_player)
		m_spawner->Update(DirectX::XMFLOAT2(m_player->GetPos().x, m_player->GetPos().z));

	// �S�I�u�W�F�N�g�̃A�b�v�f�[�g�ƍ폜�����B
	CObjectManager::GetIns()->Update();
	CObjectManager::GetIns()->RemoveUpdate();
	// �����蔻��̍X�V�����B
	UpdateCollision();
}


void SceneK07::Draw()
{
	// �ŏI�I�ɏ����o���o�b�t�@���擾
	auto rtvDefault = GetObj<RenderTarget>("RTV");
	auto dsvDefault = GetObj<DepthStencil>("DSV");
	// �ŏI�I�ɏ����o���o�b�t�@��ݒ肵�A�p�C�v���C������
	PipelineDraw(rtvDefault, dsvDefault);
	// �G�t�F�N�g�̕`�������B
	Efk::Draw();

	

	DrawInfo();
}

void SceneK07::InputPause()
{
	// �|�[�Y������ꂽ��A�|�[�Y�V�[�����폜�B
	if (m_pSubScene)
	{
		RemoveSubScene();
		return;
	}
	// �G�X�P�[�v�������ꂽ��|�[�Y�V�[�����I�[�o�[���C�����B
	if (IsTrigger(INPUT_PAUSE))
	{
		auto scene = AddSubScene<CScenePause>();
		scene->SetCallBack(this);	// �������R�[���o�b�N�ɐݒ�B
		m_pause = true;
		return;
	}
}

void SceneK07::UpdateCamera()
{
	// DCC�J�����ƌ��݂̃��C���J�������擾
	CameraBase* cameraDCC = GetObj<CameraBase>("Camera");
	auto primary = CameraBase::GetPrimary();
	// Ctrl��DCC�J�����ƕ��ʂ̃J������Switch
	if (IsKeyTrigger(VK_CONTROL))
	{
		auto defaultCamera = GetObj<CameraBase>("MainCamera");
		CameraBase::SetPrimary(primary == cameraDCC ? defaultCamera : cameraDCC);
	}
	
}

void SceneK07::UpdateEvent()
{
	// �g���K�[�� chu�������ł��߂�B�w����W�ɗ�����C�x���g�����B
	if(m_eventFlag == false && m_player && m_player->GetPos().z > 44 * WALL_SCALE)
	{
		auto evCam = new CEventCamera("Assets/CSV/EventCamera.csv");
		evCam->AddCallBack(this);
		m_eventFlag = true;
		new CBossStage01();
		Sound::FadeOut("RASHOMON.wav", 1.0f);
		Sound::FadeIn("Coolness.wav", 1.0f, 0.4f, true);
		m_isEvent = true;
	}
}


void SceneK07::InitSky()
{
	// �X�J�C�{�b�N�X�𐶐����鏈���B���g�p�B
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
	// �X�J�C�{�b�N�X��`�悷�鏈���B���g�p�B
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

void SceneK07::DrawInfo()
{
	if (m_isEvent) { return; }
	auto rtvDefault = GetObj<RenderTarget>("RTV");
	auto dsvDefault = GetObj<DepthStencil>("DSV");
	SetRenderTargets(1, &rtvDefault, nullptr);
	const float iconPosX[4] =
	{
		-0.66f,
		-0.2f,
		0.4f,
		0.9f
	};
	const int padID[4] =
	{
		ICON_PAD_STICK_L,
		ICON_PAD_A,
		ICON_PAD_LB,
		ICON_PAD_START
	};
	const int keyID[4] =
	{
		ICON_KEY_ARROW,
		ICON_KEY_Z,
		ICON_KEY_SHIFT,
		ICON_KEY_ESC
	};
	// �X�R�A�p�̕`��B�v���P�B
	DebugText::StartDrawString(2);
	DebugText::DrawString(0.2f, 0.9f, "Score:" + std::to_string(CScore::Ins()->GetScore()));
	DebugText::DrawString(0.2f, 0.8f, "Combo:" + std::to_string(CScore::Ins()->GetCombo()));
	DebugText::EndDrawString();

	// ��������B�ׂ������Ȃ̂ŗv���P�B
	Sprite::SetTexture(m_infoBack);
	Sprite::SetSize(2.0f, 0.28f);
	Sprite::SetOffset(0, -0.94f);
	Sprite::Draw();
	Sprite::SetSize(0.14f, 0.14f);
	for (int i = 0; i < 4; i++)
	{
		Sprite::SetTexture(m_frame % 240 < 120 ? m_icons[padID[i]] : m_icons[keyID[i]]);
		Sprite::SetOffset(iconPosX[i], -0.92f);
		Sprite::Draw();
	}
	Sprite::Reset();

	SetRenderTargets(1, &rtvDefault, dsvDefault);

}

void SceneK07::CallBack(int eventID)
{
	// �C�x���g���Ƃɏ�����ς���B
	if (eventID == EVENT_FINISH_EVENT_CAMERA)
	{
		auto defaultCamera = GetObj<CameraBase>("MainCamera");
		CameraBase::SetPrimary(defaultCamera);
		defaultCamera->SetFovY(90);
		m_isEvent = false;
	}
	if (eventID == EVENT_CLOSE_PAUSE_WINDOW)
	{
		m_pause = false;
	}
}

