#include "Wall.h"
#include "Defines.h"
#include "CameraBase.h"
CWall::CWall(int x, int y, int id, float offsetX, float offsetY)
{
	// 渡された値が負数だったら当たり判定をとらない。
	bool enableCollision = id > 0;
	// マイナス値を消す
	id = abs(id);

	std::string path = "Assets/Model/Wall/" + std::to_string(id) + ".fbx";
	Load(path.c_str(), WALL_SCALE);
	m_pos = DirectX::XMFLOAT3(x * WALL_SCALE - offsetX * WALL_SCALE, 0, y * WALL_SCALE - offsetY * WALL_SCALE);
	m_isStaticPosition = true;	// 静的オブジェクトとして登録。自動でワールド行列を計算しない。
	m_isBoxCollision = true;
	m_colliderScale = WALL_SCALE;
	// 一度だけワールド行列を計算する。
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