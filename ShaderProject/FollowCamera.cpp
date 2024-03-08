#include "FollowCamera.h"

CFollowCamera::CFollowCamera()
	:CameraBase()
{
	DataPool::AddData("FollowCamera", this);
}

void CFollowCamera::Update()
{
	// ターゲットを見続ける。
	// いったんターゲットの場所に移動し、そこからオフセットの分だけ移動。
	m_pos = m_target->GetPos();
	DirectXUtil::Increment(&m_pos, m_posOffset);
	m_look = m_target->GetPos();
	if (m_shakeFrame > 0)
	{
		m_look.x += (rand() % 1000) * 0.001f - 0.5f;
		m_look.y += (rand() % 1000) * 0.001f - 0.5f;
		m_shakeFrame--;
	}
	DirectXUtil::Increment(&m_look, m_lookOffset);
}

void CFollowCamera::Shake()
{
	m_shakeFrame = 3;
}
