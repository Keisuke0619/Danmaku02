#include "Wall.h"
#include "Defines.h"

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
#endif
	UseCollision(true);
	m_tag = "Wall";
}

void CWall::Update()
{
}
