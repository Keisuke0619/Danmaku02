#include "EnemySpawner.h"
#include "CsvLoader.h"
#include "Defines.h"
CEnemySpawner::CEnemySpawner(std::string filePath)
{
	/*
		�f�[�^�\��
			�����f�[�^ �P�s
			��
			�e�f�[�^�E�ړ��f�[�^�@�C�Ӎs
		�̏��ԁB
		�u�����f�[�^�v������ƍ��܂Ń��[�h���Ă����f�[�^�����X�g�ɏ������݁A���������ǂݍ��݂Ȃ����Ƃ��������B
	*/

	// �w���CSV�����[�h�B
	auto csv = CSVLoad(filePath, true);
	// �󂯎M�쐬�B
	TSpawnData spawnData;
	bool isFirst = true;

	for (auto line : csv)
	{
		// line�̂P�Ԗڂɉ��̃f�[�^����������Ă���B
		switch (std::stoi(line[0]))
		{
		case 0:	// �����f�[�^�Ȃ�
			// �ŏ��̈ȊO�̓��X�g�ɏ������݂��s���B�i�ŏ��ɏ�������ł��܂��Ƃ�����ۂœo�^���Ă��܂��A�G���[�ɂȂ�j
			if (isFirst == false)
			{
				m_spawnDatas.push_back(spawnData);
				spawnData.shots.clear();
				spawnData.moves.clear();
			}
			// �����ŏ�����Ȃ��B
			isFirst = false;
			// ����ǂݍ��݁B
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
		case 1:	// �e�f�[�^�Ȃ�
		{
			// �e�f�[�^�\���̂��쐬���A����ǂݍ��݁B
			TEnemyShotData shot;
			shot.speed		= std::stof(line[1]);
			shot.ways		= std::stoi(line[2]);
			shot.startAngle	= std::stof(line[3]);
			shot.wayAngle	= std::stof(line[4]);
			shot.frameAngle	= std::stof(line[5]);
			shot.waitFrame	= std::stoi(line[6]);
			shot.colorID	= line[7] + ".png";
			// �G�̒e���X�g�ɒǉ�
			spawnData.shots.push_back(shot);
		}
			break;
		case 2:
		{
			// �ړ��f�[�^�\���̂��쐬���A����ǂݍ��݁B
			TEnemyMoveData move;
			move.frame			 = std::stoi(line[1]);
			move.posFromSpawn.x	 = std::stof(line[2]) * WALL_SCALE;
			move.posFromSpawn.y	 = std::stof(line[3]) * WALL_SCALE;
			move.toFrame		 = std::stoi(line[4]);
			// �G�̈ړ����X�g�ɒǉ�
			spawnData.moves.push_back(move);
		}
			break;
		}
	}

	// �I�����ɂ��ǉ����Ă����BCSV�̒������O�̎����l�����A�ŏ��������疳������B
	if (isFirst == false)
	{
		m_spawnDatas.push_back(spawnData);
	}

}

void CEnemySpawner::Update(DirectX::XMFLOAT2 pos)
{
	// �G�͋߂Â��Ɛ��������B
	// �܂��A�����G�������ꏊ����Ԋu���J���ĕ����̐�������邱�Ƃ�����B
	// ���̂��߁A�߂Â�����u�����J�n�v���A�w�萔�u�����𑱂���v���ƂɂȂ�B
	
	// �u�����J�n�v�ꏊ�B�߂Â����� �A�N�e�B�u���X�g�ɒǉ����A�����J�n���X�g���폜����B
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



	// �u�����𑱂���v�ꏊ�B�A�N�e�B�u���X�g���Q�Ƃ��A�w�萔�G�𐶐�����B�I�������烊�X�g����폜�����B
	itr = m_activeList.begin();
	while (itr != m_activeList.end())
	{
		// �I���������͍̂폜�B
		if (itr->activeFrame >= itr->waitFrame * itr->crones)
		{
			itr = m_activeList.erase(itr);
			continue;
		}
		// �w��Ԋu�Ő���
		if (itr->waitFrame == 0 || itr->activeFrame % itr->waitFrame == 0)
		{
			auto enemy = new CEnemy();
			// �e�f�[�^��o�^�B
			for (auto shot : itr->shots)
			{
				enemy->PushData(shot);
			}
			// �ړ��f�[�^��o�^�B
			for (auto move : itr->moves)
			{
				enemy->PushData(move);
			}
			enemy->SetOriginPos(itr->pos, itr->offsetY);
			enemy->SetSpawnData(itr->modelPath.c_str(), itr->destroyFrame, itr->hp);
		}
		// �Y���u�����𑱂���v�f�[�^�̃t���[�������Z�B
		itr->activeFrame++;

		itr++;
	}
}
