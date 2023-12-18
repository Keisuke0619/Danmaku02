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
	if (m_frame % 30 == 0)
	{
		auto shot = CShot::Create(this, m_pos, 5, DirectX::XMConvertToDegrees(GetAngleToPlayer()), RED, SIZE03);
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
