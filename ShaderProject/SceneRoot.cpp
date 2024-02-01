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

//--- 構造体
// @brief シーン情報保存
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

void SceneRoot::SetNextScene(ESceneID id)
{
	m_nextSceneID = id;
}

void SceneRoot::SceneChange()
{
	RemoveSubScene();
	switch (m_nextSceneID)
	{
	case SCENE_GAME: AddSubScene<SceneK07>(); break;
	case SCENE_TITLE: AddSubScene<CSceneTitle>(); break;
	}

	m_nextSceneID = SCENE_NONE;
}

const char* SettingFileName = "Assets/setting.dat";

void SceneRoot::Init()
{
	// 保存データの読み込み
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

	// カメラの作成
	CameraBase* pCamera = CreateObj<CameraDCC>("Camera");
	pCamera->SetPos(setting.camPos);
	pCamera->SetLook(setting.camLook);
	pCamera->SetUp(setting.camUp);

	// ライトの作成
	MoveLight* pLight = CreateObj<MoveLight>("Light");
	pLight->SetRot(setting.lightRadXZ, setting.lightRadY);
	pLight->SetHSV(setting.lightH, setting.lightSV);
	pLight->UpdateParam();

	// シーンの作成
	m_index = setting.index;
	SetNextScene(SCENE_GAME);

	
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
	if (IsKeyTrigger('N'))
	{
		SetNextScene(SCENE_GAME);
	}
	if (m_nextSceneID != SCENE_NONE)
	{
		SceneChange();
	}
}
void SceneRoot::Draw()
{
	LightBase* pLight = GetObj<LightBase>("Light");

	// オブジェクト描画
	pLight->Draw();

	Geometory::DrawLines();
}
