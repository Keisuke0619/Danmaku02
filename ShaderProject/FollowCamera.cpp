#include "FollowCamera.h"

void CFollowCamera::Update()
{
	// ターゲットを見続ける。
	// いったんターゲットの場所に移動し、そこからオフセットの分だけ移動。
	m_pos = m_target->GetPos();
	DirectXUtil::Increment(&m_pos, m_posOffset);
	m_look = m_target->GetPos();
	DirectXUtil::Increment(&m_look, m_lookOffset);
}
