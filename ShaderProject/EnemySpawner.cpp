#include "EnemySpawner.h"
#include "CsvLoader.h"
#include "Defines.h"
CEnemySpawner::CEnemySpawner(std::string filePath)
{
	/*
		データ構造
			生成データ １行
			↓
			弾データ・移動データ　任意行
		の順番。
		「生成データ」が来ると今までロードしていたデータをリストに書き込み、初期化し読み込みなおすという処理。
	*/

	// 指定のCSVをロード。
	auto csv = CSVLoad(filePath, true);
	// 受け皿作成。
	TSpawnData spawnData;
	bool isFirst = true;

	for (auto line : csv)
	{
		// lineの１番目に何のデータかが書かれている。
		switch (std::stoi(line[0]))
		{
		case 0:	// 生成データなら
			// 最初の以外はリストに書き込みを行う。（最初に書き込んでしまうとからっぽで登録してしまい、エラーになる）
			if (isFirst == false)
			{
				m_spawnDatas.push_back(spawnData);
				spawnData.shots.clear();
				spawnData.moves.clear();
			}
			// もう最初じゃない。
			isFirst = false;
			// 一つずつ読み込み。
			spawnData.modelPath		= "Assets/Model/" + line[1] + ".fbx";
			spawnData.waitFrame		= std::stoi(line[2]);
			spawnData.crones		= std::stoi(line[3]);
			spawnData.pos.x			= std::stof(line[4]) * WALL_SCALE;
			spawnData.pos.y			= std::stof(line[5]) * WALL_SCALE;
			spawnData.destroyFrame	= std::stoi(line[6]);
			spawnData.distance		= std::stof(line[7]) * WALL_SCALE;
			spawnData.hp			= std::stoi(line[8]);
			spawnData.offsetY			= std::stof(line[9]);
			break;
		case 1:	// 弾データなら
		{
			// 弾データ構造体を作成し、一つずつ読み込み。
			TEnemyShotData shot;
			shot.speed		= std::stof(line[1]);
			shot.ways		= std::stoi(line[2]);
			shot.startAngle	= std::stof(line[3]);
			shot.wayAngle	= std::stof(line[4]);
			shot.frameAngle	= std::stof(line[5]);
			shot.waitFrame	= std::stoi(line[6]);
			shot.colorID	= line[7] + ".png";
			// 敵の弾リストに追加
			spawnData.shots.push_back(shot);
		}
			break;
		case 2:
		{
			// 移動データ構造体を作成し、一つずつ読み込み。
			TEnemyMoveData move;
			move.frame			 = std::stoi(line[1]);
			move.posFromSpawn.x	 = std::stof(line[2]) * WALL_SCALE;
			move.posFromSpawn.y	 = std::stof(line[3]) * WALL_SCALE;
			move.toFrame		 = std::stoi(line[4]);
			// 敵の移動リストに追加
			spawnData.moves.push_back(move);
		}
			break;
		}
	}

	// 終了時にも追加しておく。CSVの長さが０の時も考慮し、最初だったら無視する。
	if (isFirst == false)
	{
		m_spawnDatas.push_back(spawnData);
	}

}

void CEnemySpawner::Update(DirectX::XMFLOAT2 pos)
{
	// 敵は近づくと生成される。
	// また、同じ敵が同じ場所から間隔を開けて複数体生成されることもある。
	// そのため、近づいたら「生成開始」し、指定数「生成を続ける」ことになる。
	
	// 「生成開始」場所。近づいたら アクティブリストに追加し、生成開始リストより削除する。
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



	// 「生成を続ける」場所。アクティブリストを参照し、指定数敵を生成する。終了したらリストから削除される。
	itr = m_activeList.begin();
	while (itr != m_activeList.end())
	{
		// 終了したものは削除。
		if (itr->activeFrame >= itr->waitFrame * itr->crones)
		{
			itr = m_activeList.erase(itr);
			continue;
		}
		// 指定間隔で生成
		if (itr->waitFrame == 0 || itr->activeFrame % itr->waitFrame == 0)
		{
			auto enemy = new CEnemy();
			// 弾データを登録。
			for (auto shot : itr->shots)
			{
				enemy->PushData(shot);
			}
			// 移動データを登録。
			for (auto move : itr->moves)
			{
				enemy->PushData(move);
			}
			enemy->SetOriginPos(itr->pos, itr->offsetY);
			enemy->SetSpawnData(itr->modelPath.c_str(), itr->destroyFrame, itr->hp);
		}
		// 該当「生成を続ける」データのフレームを加算。
		itr->activeFrame++;

		itr++;
	}
}
