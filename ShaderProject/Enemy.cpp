#include "Enemy.h"
#include "Player.h"
#include "ShotObj.h"
#include "Defines.h"
#include "Input.h"
#include "Effect.h"
CEnemy::CEnemy()
{
	UseCollision(true);
	m_tag = "Enemy";
	m_life = 30;
	m_colliderScale = 2;
}
void CEnemy::Update()
{
	if (m_frame == 0)
	{
		Efk::Play(u"Assets/Effect/Spawn.efkefc", m_pos.x, m_pos.y, m_pos.z);
	}
	CSVRoutine();
	if (m_life <= 0) { Destroy(); }
}

void CEnemy::SetSpawnData(const char* modelPath, int autoDestroyFrame, int hp)
{
	Load(modelPath);
	m_destroyFrame = autoDestroyFrame;
	m_life = hp;
}

void CEnemy::SetOriginPos(DirectX::XMFLOAT2 pos, float height)
{
	m_pos.x = pos.x;
	m_pos.y = height;
	m_pos.z = pos.y;
	m_spawnPos = pos;
}

void CEnemy::PushData(TEnemyMoveData data)
{
	m_moveDatas.push_back(data);

	m_moveDatas.sort(
		[](TEnemyMoveData l, TEnemyMoveData r)->bool
		{
			return l.frame < r.frame;
		});
}

void CEnemy::PushData(TEnemyShotData data)
{
	m_shotDatas.push_back(data);
}

void CEnemy::OnCollision(CObject* _obj)
{
	if (_obj->GetTagName() == "ShotFromPlayer")
	{
		m_life--;
	}
}

float CEnemy::GetAngleToPlayer()
{
	auto pl = CPlayer::GetPrimary();
	if (pl == nullptr)
	{
		return 0;
	}
	return atan2f(pl->GetPos().z - m_pos.z, pl->GetPos().x - m_pos.x);
}

void CEnemy::CSVRoutine()
{
	// 自動削除処理
	if (m_frame >= m_destroyFrame)
	{
		Destroy();
		return;
	}

	// 読み込まれたShotリストを順に再生
	for (auto shot : m_shotDatas)
	{
		CSVShot(shot);
	}

	// 以下移動処理
	bool isDelete = false;
	// 移動リストは先頭を読み取る。
	auto itr = m_moveDatas.begin();
	// 先頭が存在すれば、移動処理実行。
	if (itr != m_moveDatas.end())
	{
		isDelete = CSVMove(*itr);
		// 移動終了で該当データ削除。
		if (isDelete)
		{
			m_moveDatas.erase(itr);
		}
	}
	// 終了フレーム＝移動開始フレームの場合があるので、移動終了時はもう一度呼ぶ。
	if (isDelete)
	{
		itr = m_moveDatas.begin();
		// 先頭が存在すれば、移動処理実行。
		if (itr != m_moveDatas.end())
		{
			isDelete = CSVMove(*itr);
			// 移動終了で該当データ削除。
			if (isDelete)
			{
				m_moveDatas.erase(itr);
			}
		}
	}
	m_pos.x += m_moveVector.x;
	m_pos.z += m_moveVector.y;

}

void CEnemy::CSVShot(TEnemyShotData data)
{
	if (m_frame % data.waitFrame != 0) { return; }

	auto angle = data.startAngle;
	if (angle == 1000) { angle = DirectX::XMConvertToDegrees(GetAngleToPlayer()); }
	if (angle == 2000) { angle = (rand() % 3600) * 0.1f; }

	for(int i = -(data.ways / 2); i <= (data.ways / 2); i++)
		CShot::Create(nullptr, m_pos, data.speed, angle + (i * data.wayAngle) + (m_frame * data.frameAngle), data.colorID);
}

bool CEnemy::CSVMove(TEnemyMoveData data)
{
	if (m_frame - (data.frame + data.toFrame) == 0)
	{
		m_moveVector = DirectX::XMFLOAT2(0, 0);
	}
	if (m_frame - data.frame == 0)
	{
		// スピード設定
		auto posf2 = DirectX::XMFLOAT2(m_pos.x, m_pos.z);
		auto vFrom = DirectX::XMLoadFloat2(&posf2);
		auto toPos = DirectXUtil::Add(data.posFromSpawn, m_spawnPos);
		auto vTo = DirectX::XMLoadFloat2(&toPos);
		vTo = DirectX::XMVectorSubtract(vTo, vFrom);

		DirectX::XMStoreFloat2(&toPos, vTo);

		float len;
		auto vLen = DirectX::XMVector2Length(vTo);
		DirectX::XMStoreFloat(&len, vLen);
		m_speed = len / data.toFrame;
		vTo = DirectX::XMVector2Normalize(vTo);
		vTo = DirectX::XMVectorScale(vTo, m_speed);
		DirectX::XMStoreFloat2(&m_moveVector, vTo);
	}
	return m_frame >= data.frame + data.toFrame;
}
