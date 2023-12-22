#include "Enemy.h"
#include "Player.h"
#include "ShotObj.h"

CEnemy::CEnemy()
{
	Load("Assets/Model/spot/spot.fbx");
	UseCollision(true);
	m_tag = "Enemy";
	m_life = 100;
	m_colliderScale = 2;
}
void CEnemy::Update()
{
	for (auto shot : m_shotDatas)
	{
		CSVShot(shot);
	}
	if (m_life <= 0) { Destroy(); }
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

void CEnemy::CSVShot(TEnemyShotData data)
{
	if (m_frame % data.waitFrame != 0) { return; }
	for(int i = -(data.ways / 2); i <= (data.ways / 2); i++)
	CShot::Create(this, m_pos, data.speed, data.startAngle + (i * data.wayAngle) + (m_frame * data.frameAngle), data.colorID);
}