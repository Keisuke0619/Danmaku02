#include "FollowCamera.h"

void CFollowCamera::Update()
{
	// �^�[�Q�b�g����������B
	// ��������^�[�Q�b�g�̏ꏊ�Ɉړ����A��������I�t�Z�b�g�̕������ړ��B
	m_pos = m_target->GetPos();
	DirectXUtil::Increment(&m_pos, m_posOffset);
	m_look = m_target->GetPos();
	DirectXUtil::Increment(&m_look, m_lookOffset);
}
