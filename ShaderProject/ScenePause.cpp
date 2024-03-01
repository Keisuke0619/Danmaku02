#include "ScenePause.h"
#include "Input.h"
#include "Sprite.h"

void CScenePause::Init()
{
	// �I�[�o�[���C�V�[���ɐݒ肵�A�|�[�Y�摜��\��
	m_isOverlay = true;
	m_back = new Texture();
	m_back->Create("Assets/Texture/UI/Pause_Back.png");
}

void CScenePause::Uninit()
{
	delete m_back;
}

void CScenePause::Update(float tick)
{
	// esc�ŊJ����esc�ŕ���֌W�ŁA�P�t���[���ڂɏ�������ƊJ�����Ɠ����ɕ��Ă��܂��B
	// ���Update�ɂĐ��m_frame���C���N�������g���Ă���֌W�� m_frame == 1 ���P�t���[���ڂƂȂ�B
	if (m_frame == 1)
	{
		return;
	}
	if (IsKeyTrigger(VK_ESCAPE))
	{
		// �R�[���o�b�N�֐����Ă�ŏI��
		m_callBack->CallBack(1);
	}
}

void CScenePause::Draw()
{
	Sprite::Reset();
	Sprite::SetTexture(m_back);
	Sprite::SetSize(2, 2);
	Sprite::Draw();
	Sprite::Reset();
}

void CScenePause::SetCallBack(IEventCallBack* callBack)
{
	// �R�[���o�b�N��ݒ�B
	m_callBack = callBack;
}
