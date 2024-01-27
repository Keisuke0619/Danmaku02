#include "Wall.h"
#include "Defines.h"
#include "CameraBase.h"
CWall::CWall(int x, int y, int id, float offsetX, float offsetY)
{
	bool enableCollision = id > 0;

	id = abs(id);
	std::string path = "Assets/Model/Wall/" + std::to_string(id) + ".fbx";
	Load(path.c_str(), WALL_SCALE);
	m_pos = DirectX::XMFLOAT3(x * WALL_SCALE - offsetX * WALL_SCALE, 0, y * WALL_SCALE - offsetY * WALL_SCALE);
	m_isStaticPosition = true;
	m_isBoxCollision = true;
	m_colliderScale = WALL_SCALE;
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

void CWall::Draw(Shader* vs, Shader* ps)
{
	auto anchor = CameraBase::GetPrimary()->GetPos().z;
	if (m_pos.z < anchor - 5 || anchor + 120 < m_pos.z) return;
	Model::Draw(vs, ps);
}
