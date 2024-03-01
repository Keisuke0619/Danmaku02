#include "Wall.h"
#include "Defines.h"
#include "CameraBase.h"
CWall::CWall(int x, int y, int id, float offsetX, float offsetY)
{
	// �n���ꂽ�l�������������瓖���蔻����Ƃ�Ȃ��B
	bool enableCollision = id > 0;
	// �}�C�i�X�l������
	id = abs(id);

	std::string path = "Assets/Model/Wall/" + std::to_string(id) + ".fbx";
	Load(path.c_str(), WALL_SCALE);
	m_pos = DirectX::XMFLOAT3(x * WALL_SCALE - offsetX * WALL_SCALE, 0, y * WALL_SCALE - offsetY * WALL_SCALE);
	m_isStaticPosition = true;	// �ÓI�I�u�W�F�N�g�Ƃ��ēo�^�B�����Ń��[���h�s����v�Z���Ȃ��B
	m_isBoxCollision = true;
	m_colliderScale = WALL_SCALE;
	// ��x�������[���h�s����v�Z����B
	ReloadWorldMatrix();
	ReloadVtxVector();
	if(enableCollision) RegistCollision();
#ifdef  _DEBUG
	UseCollision(false);
#else //  _DEBUG
	UseCollision(true);
#endif
	m_tag = "Wall";
}

void CWall::Update()
{
}