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
	// 説明用バナー読み込み
	m_banner[0] = new Texture();
	m_banner[0]->Create("Assets/Texture/UI/Result_Banner01.png");
	m_banner[1] = new Texture();
	m_banner[1]->Create("Assets/Texture/UI/Result_Banner02.png");

	// スコア取得
	m_score = CScore::Ins()->GetScore();
	// ランキングデータを開く
	FILE* fp = fopen("Assets/CSV/Rank.dat", "rb");
	// ランキングデータが存在するなら読み込んでクローズ。ないなら初期データ生成
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
	// ランキングのデータと今回のスコアを比較して今回の順位を取得。同時に自分以下のデータを一つ下にずらし、スペースを作る。
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
	// ランキング圏内なら、名前入力関数をセット、圏外なら終了まち関数をセット。（順位の初期値は-1のため、ゼロ以上でランキング圏内となる）
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
	// 音楽再生
	Sound::FadeIn("Graduation.wav", 1.0f, 0.3f, true);
	// 順位を基準に表示場所を指定。ちょうど現在のランクが映る場所になる。
	m_posY = m_rank * 0.25f;

	// 最終的に書き出すバッファを取得
	auto rtvDefault = GetObj<RenderTarget>("RTV");
	// 深度バッファは使用しないので指定しない。
	SetRenderTargets(1, &rtvDefault, nullptr);

}

void CSceneResult::Uninit()
{
	// セーブ
	FILE* fp = fopen("Assets/CSV/Rank.dat", "wb");
	fwrite(m_rankData, sizeof(TRankData), RANKING_NUM, fp);
	fclose(fp);
}

void CSceneResult::Update(float tick)
{
	// 名前入力 or 終了待ち関数を呼ぶ。
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
	// ランキングデータの項目ごとに　順位　名前　スコア　を表示させる。
	for (int i = 0; i < RANKING_NUM; i++)
	{
		// 自分の項目はsinで左右に揺らす。
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

	// 下の案内バナー描画
	Sprite::SetTexture(m_drawTex);
	Sprite::SetSize(2.0f, 0.3f);
	Sprite::SetOffset(0, -0.85f);
	Sprite::Draw();
}

void CSceneResult::UpdatePhaseInput()
{
	// 名前入力完了。呼び出し関数を終了待ち関数に変更。下の案内バナーを変更し終了。
	if (IsKeyTrigger(VK_RETURN))
	{
		m_updateFunc = &CSceneResult::UpdatePhaseWaitExit;
		m_drawTex = m_banner[1];
		return;
	}
	// 現在のカーソル位置が11未満なら。（オーバーフローしないように）
	if (m_cursor < 11)
	{
		// 英字入力
		for (BYTE key = 'A'; key <= 'Z'; key++)
		{
			if (IsKeyTrigger(key))
			{
				// 小文字入力
				if (IsKeyPress(VK_SHIFT) == false)
				{
					key -= 'A' - 'a';
				}
				// 先頭だったら文字列を全削除。（プリセット削除用。空の状態から削除しても別段問題ない。）
				if (m_cursor == 0)
				{
					ZeroMemory(m_rankData[m_rank].name, sizeof(char) * 12);
				}
				// 指定個所の文字を変更。
				m_rankData[m_rank].name[m_cursor] = key;
				m_cursor++;
				break;
			}
		}
		// 数字。処理は英字と大体同じ。大文字小文字処理を抜いたもの。
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
		// 空白処理。
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
	// 削除処理。カーソル部分を空白に変更し、カーソルを一つ戻す。
	if (IsKeyTrigger(VK_BACK))
	{
		m_cursor--;
		if (m_cursor < 0) m_cursor = 0;
		m_rankData[m_rank].name[m_cursor] = ' ';
	}
}

void CSceneResult::UpdatePhaseWaitExit()
{
	// リターンでタイトルに戻る。
	if (IsKeyTrigger(VK_RETURN))
	{
		((SceneRoot*)m_pParent)->SetNextScene(SceneRoot::SCENE_TITLE);
		Sound::FadeOut("Graduation.wav", 1.0f);
	}
	const float SpeedY = 0.1f;
	// ランキングスクロール処理。上下それぞれ移動が可能で、上端下端でストップするようになっている。
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