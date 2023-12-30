#include "EnemySpawner.h"
#include "CsvLoader.h"
#include "Defines.h"
CEnemySpawner::CEnemySpawner(std::string filePath)
{
	auto csv = CSVLoad(filePath, true);
	TSpawnData spawnData;
	bool isFirst = true;
	for (auto line : csv)
	{
		switch (std::stoi(line[0]))
		{
		case 0:
			if (isFirst == false)
			{
				m_spawnDatas.push_back(spawnData);
				spawnData.shots.clear();
				spawnData.moves.clear();
			}
			isFirst = false;
			spawnData.modelPath		= "Assets/Model/" + line[1] + ".fbx";
			spawnData.waitFrame		= std::stoi(line[2]);
			spawnData.crones		= std::stoi(line[3]);
			spawnData.pos.x			= std::stof(line[4]) * WALL_SCALE;
			spawnData.pos.y			= std::stof(line[5]) * WALL_SCALE;
			spawnData.destroyFrame	= std::stoi(line[6]);
			spawnData.distance		= std::stof(line[7]) * WALL_SCALE;
			break;
		case 1:
		{
			TEnemyShotData shot;
			shot.speed		= std::stof(line[1]);
			shot.ways		= std::stoi(line[2]);
			shot.startAngle	= std::stof(line[3]);
			shot.wayAngle	= std::stof(line[4]);
			shot.frameAngle	= std::stof(line[5]);
			shot.waitFrame	= std::stoi(line[6]);
			shot.colorID	= line[7] + ".png";
			spawnData.shots.push_back(shot);
		}
			break;
		case 2:
		{
			TEnemyMoveData move;
			move.frame			 = std::stoi(line[1]);
			move.posFromSpawn.x	 = std::stof(line[2]) * WALL_SCALE;
			move.posFromSpawn.y	 = std::stof(line[3]) * WALL_SCALE;
			move.toFrame		 = std::stoi(line[4]);
			spawnData.moves.push_back(move);
		}
			break;
		}
	}
	if (isFirst == false)
	{
		m_spawnDatas.push_back(spawnData);
	}

}

void CEnemySpawner::Update(DirectX::XMFLOAT2 pos)
{
	auto itr = m_spawnDatas.begin();
	while (itr != m_spawnDatas.end())
	{
		if (DirectXUtil::Dist(pos, itr->pos) < powf(itr->distance, 2))
		{
			itr->activeFrame = 0;
			m_activeList.push_back(*itr);
			itr = m_spawnDatas.erase(itr);
			continue;
		}
		itr++;
	}
	itr = m_activeList.begin();
	while (itr != m_activeList.end())
	{
		if (itr->activeFrame >= itr->waitFrame * itr->crones)
		{
			itr = m_activeList.erase(itr);
			continue;
		}
		if (itr->waitFrame == 0 || itr->activeFrame % itr->waitFrame == 0)
		{
			auto enemy = new CEnemy();
			for (auto shot : itr->shots)
			{
				enemy->PushData(shot);
			}
			for (auto move : itr->moves)
			{
				enemy->PushData(move);
			}
			enemy->SetOriginPos(itr->pos);
			enemy->SetSpawnData(itr->modelPath.c_str(), itr->destroyFrame);
		}
		itr->activeFrame++;

		itr++;
	}
}
