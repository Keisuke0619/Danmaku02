#include "ShotKiller.h"

CShotKiller::CShotKiller(DirectX::XMFLOAT3 pos)
	: CObject()
{
	UseCollision(true);
	m_colliderScale = 100;
	m_pos = pos;
	m_tag = "EnemyShotDestroy";
}

void CShotKiller::OnCollision(CObject* _obj)
{
	int a = 0;
}