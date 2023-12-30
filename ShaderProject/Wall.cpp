#include "Wall.h"
#include "Defines.h"

CWall::CWall(int x, int y, int id, float offsetX, float offsetY)
{
	std::string path = "Assets/Model/Wall/" + std::to_string(id) + ".fbx";
	Load(path.c_str(), WALL_SCALE);
	m_pos = DirectX::XMFLOAT3(x * WALL_SCALE - offsetX * WALL_SCALE, 0, y * WALL_SCALE - offsetY * WALL_SCALE);
	m_isStaticPosition = true;
	m_isBoxCollision = true;
	m_colliderScale = WALL_SCALE;
	ReloadWorldMatrix();
	ReloadVtxVector();
	RegistCollision();
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
