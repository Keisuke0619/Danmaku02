#pragma once
#include <string>
#include "Enemy.h"
struct TSpawnData
{
	std::string modelPath;
	int waitFrame;
	int crones;
	DirectX::XMFLOAT2 pos;
	int destroyFrame;
	float distance;
	int hp;
	std::list<TEnemyMoveData> moves;
	std::list<TEnemyShotData> shots;
	int activeFrame = 0;
};
class CEnemySpawner
{
public:
	CEnemySpawner(std::string filePath);

	void Update(DirectX::XMFLOAT2 pos);

private:
	std::list<TSpawnData> m_spawnDatas;
	std::list<TSpawnData> m_activeList;
};