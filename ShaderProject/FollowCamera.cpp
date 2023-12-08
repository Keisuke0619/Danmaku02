#include "FollowCamera.h"

void CFollowCamera::Update()
{
	m_pos = m_target->GetPos();
	DirectXUtil::Increment(&m_pos, m_posOffset);
	m_look = m_target->GetPos();
	DirectXUtil::Increment(&m_look, m_lookOffset);
}
