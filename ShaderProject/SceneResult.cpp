#include "SceneResult.h"
#include "Input.h"
#include "SceneRoot.h"
#include "Texture.h"
#include "SoundUtil.h"
#include "ObjectManager.h"
#include "Score.h"
#include <stdio.h>
#include "DebugText.h"

int CSceneResult::m_collisionCount;

void CSceneResult::Init()
{
	m_banner[0] = new Texture();
	m_banner[1] = new Texture();
	m_banner[0]->Create("Assets/Texture/UI/Result_Banner01.png");
	m_banner[1]->Create("Assets/Texture/UI/Result_Banner02.png");

	m_score = CScore::Ins()->GetScore();
	FILE* fp = fopen("Assets/CSV/Rank.dat", "rb");
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
	for (int i = 0; i < RANKING_NUM - 1; i++)
	{
		if (m_rankData[i].score <= m_score)
		{
			m_rank = i;
			memcpy(&(m_rankData[i + 1]), &(m_rankData[i]), sizeof(TRankData) * (RANKING_NUM - i - 1));
			strcpy(m_rankData[i].name, " put name");
			m_rankData[i].score = m_score;
			break;
		}
	}
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
	Sound::FadeIn("Graduation.wav", 1.0f, 0.3f, true);
}

void CSceneResult::Uninit()
{
	FILE* fp = fopen("Assets/CSV/Rank.dat", "wb");
	fwrite(m_rankData, sizeof(TRankData), RANKING_NUM, fp);

	fclose(fp);
}

void CSceneResult::Update(float tick)
{
	(this->*m_updateFunc)();
}

void CSceneResult::Draw()
{
	//Sprite::SetTexture(m_back);
	//Sprite::SetSize(2, 2);
	//Sprite::Draw();

	Sprite::SetTexture(m_drawTex);
	Sprite::SetSize(2.0f, 0.3f);
	Sprite::SetOffset(0, -0.85f);
	Sprite::Draw();


	const float NameX = -0.5f;
	const float ScoreX = 0.2f;
	const float AnchorY = 0.5f;
	const float AddY = -0.25f;
	float addX = 0.0f;
	int rank = 0;


	DebugText::StartDrawString(2.2f);
	for (int i = 0; i < RANKING_NUM; i++)
	{
		if (rank == m_rank)
		{
			addX = -0.1f + sinf(m_frame * 0.1f) * 0.05f;
		}
		else
		{

			addX = 0.0f;
		}
		const std::string rankBackStr[] = { "st", "nd", "rd", "th" };
		DebugText::DrawString(NameX + addX - 0.3f, AnchorY + AddY * rank + m_posY, std::to_string(i + 1) + rankBackStr[i > 3 ? 3 : i]);
		DebugText::DrawString(NameX + addX, AnchorY + AddY * rank + m_posY, m_rankData[i].name);
		DebugText::DrawString(ScoreX + addX, AnchorY + AddY * rank + m_posY, std::to_string(m_rankData[i].score));
		rank++;
	}
	

	DebugText::EndDrawString();

}

void CSceneResult::UpdatePhaseInput()
{
	if (IsKeyTrigger(VK_RETURN))
	{
		m_updateFunc = &CSceneResult::UpdatePhaseWaitExit;
		m_drawTex = m_banner[1];
		return;
	}
	if (m_cursor < 11)
	{
		for (BYTE key = 'A'; key <= 'Z'; key++)
		{
			if (IsKeyTrigger(key))
			{
				if (IsKeyPress(VK_SHIFT) == false)
				{
					key -= 'A' - 'a';
				}
				if (m_cursor == 0)
				{
					ZeroMemory(m_rankData[m_rank].name, sizeof(char) * 12);
				}
				m_rankData[m_rank].name[m_cursor] = key;
				m_cursor++;
				break;
			}
		}
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
	
	if (IsKeyTrigger(VK_BACK))
	{
		m_cursor--;
		if (m_cursor < 0) m_cursor = 0;
		m_rankData[m_rank].name[m_cursor] = ' ';
	}
}

void CSceneResult::UpdatePhaseWaitExit()
{
	if (IsKeyTrigger(VK_RETURN))
	{
		((SceneRoot*)m_pParent)->SetNextScene(SceneRoot::SCENE_TITLE);
		Sound::FadeOut("Graduation.wav", 1.0f);
	}
	const float SpeedY = 0.1f;
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