#include "SceneResult.h"
#include "Input.h"
#include "SceneRoot.h"
#include "Texture.h"
#include "SoundUtil.h"
#include "ObjectManager.h"
#include "Score.h"
#include <stdio.h>
#include "DebugText.h"
#include "DebugWindow.hpp"
int CSceneResult::m_collisionCount;

void CSceneResult::Init()
{
	// �����p�o�i�[�ǂݍ���
	m_banner[0] = new Texture();
	m_banner[0]->Create("Assets/Texture/UI/Result_Banner01.png");
	m_banner[1] = new Texture();
	m_banner[1]->Create("Assets/Texture/UI/Result_Banner02.png");

	// �X�R�A�擾
	m_score = CScore::Ins()->GetScore();
	// �����L���O�f�[�^���J��
	FILE* fp = fopen("Assets/CSV/Rank.dat", "rb");
	// �����L���O�f�[�^�����݂���Ȃ�ǂݍ���ŃN���[�Y�B�Ȃ��Ȃ珉���f�[�^����
	if (fp)
	{
		fread(m_rankData, sizeof(TRankData), RANKING_NUM, fp);
		fclose(fp);
	}
	else
	{
		for (int i = 0; i < RANKING_NUM; i++)
		{
			m_rankData[i].score = RANKING_NUM - i;
		}
	}
	// �����L���O�̃f�[�^�ƍ���̃X�R�A���r���č���̏��ʂ��擾�B�����Ɏ����ȉ��̃f�[�^������ɂ��炵�A�X�y�[�X�����B
	for (int i = 0; i < RANKING_NUM - 1; i++)
	{
		if (m_rankData[i].score < m_score)
		{
			m_rank = i;
			memcpy(&(m_rankData[i + 1]), &(m_rankData[i]), sizeof(TRankData) * (RANKING_NUM - i - 1));
			strcpy(m_rankData[i].name, " put name");
			m_rankData[i].score = m_score;
			break;
		}
	}
	// �����L���O�����Ȃ�A���O���͊֐����Z�b�g�A���O�Ȃ�I���܂��֐����Z�b�g�B�i���ʂ̏����l��-1�̂��߁A�[���ȏ�Ń����L���O�����ƂȂ�j
	if (m_rank < 0)
	{
		m_updateFunc = &CSceneResult::UpdatePhaseWaitExit;
		m_drawTex = m_banner[1];
	}
	else
	{
		m_updateFunc = &CSceneResult::UpdatePhaseInput;
		m_drawTex = m_banner[0];
	}
	// ���y�Đ�
	Sound::FadeIn("Graduation.wav", 1.0f, 0.3f, true);
	// ���ʂ���ɕ\���ꏊ���w��B���傤�ǌ��݂̃����N���f��ꏊ�ɂȂ�B
	m_posY = m_rank * 0.25f;

	// �ŏI�I�ɏ����o���o�b�t�@���擾
	auto rtvDefault = GetObj<RenderTarget>("RTV");
	// �[�x�o�b�t�@�͎g�p���Ȃ��̂Ŏw�肵�Ȃ��B
	SetRenderTargets(1, &rtvDefault, nullptr);

}

void CSceneResult::Uninit()
{
	// �Z�[�u
	FILE* fp = fopen("Assets/CSV/Rank.dat", "wb");
	fwrite(m_rankData, sizeof(TRankData), RANKING_NUM, fp);
	fclose(fp);
}

void CSceneResult::Update(float tick)
{
	// ���O���� or �I���҂��֐����ĂԁB
	(this->*m_updateFunc)();
}

void CSceneResult::Draw()
{	
	const float NameX = -0.5f;
	const float ScoreX = 0.2f;
	const float AnchorY = 0.5f;
	const float AddY = -0.25f;
	const std::string rankBackStr[] = { "st", "nd", "rd", "th" };
	float addX = 0.0f;
	int rank = 0;

	DebugText::StartDrawString(2.2f);
	// �����L���O�f�[�^�̍��ڂ��ƂɁ@���ʁ@���O�@�X�R�A�@��\��������B
	for (int i = 0; i < RANKING_NUM; i++)
	{
		// �����̍��ڂ�sin�ō��E�ɗh�炷�B
		if (rank == m_rank)
		{
			addX = -0.1f + sinf(m_frame * 0.1f) * 0.05f;
		}
		else
		{
			addX = 0.0f;
		}
		
		DebugText::DrawString(NameX + addX - 0.3f, AnchorY + AddY * rank + m_posY, std::to_string(i + 1) + rankBackStr[i > 3 ? 3 : i]);
		DebugText::DrawString(NameX + addX, AnchorY + AddY * rank + m_posY, m_rankData[i].name);
		DebugText::DrawString(ScoreX + addX, AnchorY + AddY * rank + m_posY, std::to_string(m_rankData[i].score));
		rank++;
	}
	DebugText::EndDrawString();

	// ���̈ē��o�i�[�`��
	Sprite::SetTexture(m_drawTex);
	Sprite::SetSize(2.0f, 0.3f);
	Sprite::SetOffset(0, -0.85f);
	Sprite::Draw();
}

void CSceneResult::UpdatePhaseInput()
{
	// ���O���͊����B�Ăяo���֐����I���҂��֐��ɕύX�B���̈ē��o�i�[��ύX���I���B
	if (IsKeyTrigger(VK_RETURN))
	{
		m_updateFunc = &CSceneResult::UpdatePhaseWaitExit;
		m_drawTex = m_banner[1];
		return;
	}
	// ���݂̃J�[�\���ʒu��11�����Ȃ�B�i�I�[�o�[�t���[���Ȃ��悤�Ɂj
	if (m_cursor < 11)
	{
		// �p������
		for (BYTE key = 'A'; key <= 'Z'; key++)
		{
			if (IsKeyTrigger(key))
			{
				// ����������
				if (IsKeyPress(VK_SHIFT) == false)
				{
					key -= 'A' - 'a';
				}
				// �擪�������當�����S�폜�B�i�v���Z�b�g�폜�p�B��̏�Ԃ���폜���Ă��ʒi���Ȃ��B�j
				if (m_cursor == 0)
				{
					ZeroMemory(m_rankData[m_rank].name, sizeof(char) * 12);
				}
				// �w����̕�����ύX�B
				m_rankData[m_rank].name[m_cursor] = key;
				m_cursor++;
				break;
			}
		}
		// �����B�����͉p���Ƒ�̓����B�啶�������������𔲂������́B
		for (BYTE key = '0'; key <= '9'; key++)
		{
			if (IsKeyTrigger(key))
			{
				if (m_cursor == 0)
				{
					ZeroMemory(m_rankData[m_rank].name, sizeof(char) * 12);
				}
				m_rankData[m_rank].name[m_cursor] = key;
				m_cursor++;
				break;
			}
		}
		// �󔒏����B
		if (IsKeyTrigger(VK_SPACE))
		{
			if (m_cursor == 0)
			{
				ZeroMemory(m_rankData[m_rank].name, sizeof(char) * 12);
			}
			m_rankData[m_rank].name[m_cursor] = ' ';
			m_cursor++;
		}
	}
	// �폜�����B�J�[�\���������󔒂ɕύX���A�J�[�\������߂��B
	if (IsKeyTrigger(VK_BACK))
	{
		m_cursor--;
		if (m_cursor < 0) m_cursor = 0;
		m_rankData[m_rank].name[m_cursor] = ' ';
	}
}

void CSceneResult::UpdatePhaseWaitExit()
{
	// ���^�[���Ń^�C�g���ɖ߂�B
	if (IsKeyTrigger(VK_RETURN))
	{
		((SceneRoot*)m_pParent)->SetNextScene(SceneRoot::SCENE_TITLE);
		Sound::FadeOut("Graduation.wav", 1.0f);
	}
	const float SpeedY = 0.1f;
	// �����L���O�X�N���[�������B�㉺���ꂼ��ړ����\�ŁA��[���[�ŃX�g�b�v����悤�ɂȂ��Ă���B
	if (IsKeyPress(VK_DOWN))
	{
		m_posY += SpeedY;
		if (m_posY > 0.25f * RANKING_NUM) m_posY = 0.25f * RANKING_NUM;
	}
	if (IsKeyPress(VK_UP))
	{
		m_posY -= SpeedY;
		if (m_posY < 0) m_posY = 0;
	}
}