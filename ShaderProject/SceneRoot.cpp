#include "SceneRoot.h"
#include <stdio.h>
#include "CameraDCC.h"
#include "MoveLight.h"
#include "Model.h"
#include "Input.h"
#include "Geometory.h"

#include "DebugText.h"

#include "SceneK07.h"
#include "SceneTitle.h"
#include "SceneResult.h"

#include "DataPool.h"
//--- �\����
// @brief �V�[�����ۑ�
struct ViewSetting
{
	DirectX::XMFLOAT3 camPos;
	DirectX::XMFLOAT3 camLook;
	DirectX::XMFLOAT3 camUp;
	float lightRadXZ;
	float lightRadY;
	float lightH;
	float lightSV;
	int index;
};


void SceneRoot::SetNextScene(ESceneID id, float delay)
{
	m_nextSceneID = id;
	m_sceneDelay = m_time + delay;
}

void SceneRoot::SceneChange()
{
	if (m_sceneDelay < 0 || m_sceneDelay > m_time) return;
	m_sceneDelay = -1;
	RemoveSubScene();
	switch (m_nextSceneID)
	{
	case SCENE_GAME: AddSubScene<SceneK07>(); break;
	case SCENE_TITLE: AddSubScene<CSceneTitle>(); break;
	case SCENE_RESULT: AddSubScene<CSceneResult>(); break;
	}

	m_nextSceneID = SCENE_NONE;
	DataPool::AddData("SceneRoot", this);
}

const char* SettingFileName = "Assets/setting.dat";

void SceneRoot::Init()
{
	// �ۑ��f�[�^�̓ǂݍ���
	ViewSetting setting =
	{
		DirectX::XMFLOAT3(0.0f, 1.0f, -5.0f),
		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
		0.0f, DirectX::XM_PIDIV4,
		0.0f, 1.0f,
		0
	};
	FILE* fp;
	fopen_s(&fp, SettingFileName, "rb");
	if (fp)
	{
		fread(&setting, sizeof(ViewSetting), 1, fp);
		fclose(fp);
	}

	// �J�����̍쐬
	CameraBase* pCamera = CreateObj<CameraDCC>("Camera");
	pCamera->SetPos(setting.camPos);
	pCamera->SetLook(setting.camLook);
	pCamera->SetUp(setting.camUp);

	// ���C�g�̍쐬
	MoveLight* pLight = CreateObj<MoveLight>("Light");
	pLight->SetRot(setting.lightRadXZ, setting.lightRadY);
	pLight->SetHSV(setting.lightH, setting.lightSV);
	pLight->UpdateParam();

	// �V�[���̍쐬
	m_index = setting.index;
	SetNextScene(SCENE_TITLE);

	DataPool::AddData("SceneRoot", this);
	m_time = 0;
	m_sceneDelay = 0;
}

void SceneRoot::Uninit()
{
	CameraBase* pCamera = GetObj<CameraBase>("Camera");
	MoveLight* pLight = GetObj<MoveLight>("Light");
	ViewSetting setting =
	{
		pCamera->GetPos(),
		pCamera->GetLook(),
		pCamera->GetUp(),
		pLight->GetRotXZ(), pLight->GetRotY(),
		pLight->GetH(), pLight->GetSV(),
		m_index
	};
	FILE* fp;
	fopen_s(&fp, SettingFileName, "wb");
	if (fp)
	{
		fwrite(&setting, sizeof(ViewSetting), 1, fp);
		fclose(fp);
	}
}

void SceneRoot::Update(float tick)
{
	DebugText::SetData(DebugText::SLOT_FPS, 1.0f / tick);
	CameraBase* pCamera = GetObj<CameraBase>("Camera");
	LightBase* pLight = GetObj<LightBase>("Light");
	pCamera->Update();
	pLight->Update();
	if (m_nextSceneID != SCENE_NONE)
	{
		SceneChange();
	}
	m_time += tick;
}
void SceneRoot::Draw()
{
	LightBase* pLight = GetObj<LightBase>("Light");

	Geometory::DrawLines();
}
