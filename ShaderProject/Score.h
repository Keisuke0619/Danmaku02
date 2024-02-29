#pragma once
#include "DebugText.h"

/// <summary>
/// スコア情報保存用クラス
/// </summary>
class CScore
{
public:
	/// <summary>
	/// スコアを加算します。基礎スコアｘコンボ数が加算量です。
	/// </summary>
	/// <param name="score">基礎スコア</param>
	void AddScore(int score) { m_score += (m_combo + 1) * score; DebugText::SetData(DebugText::SLOT_SCORE, m_score); }
	/// <summary>
	/// コンボ数を加算します。基本的に引数はなしでＯＫ。
	/// </summary>
	/// <param name="add">加算する量。基本１でＯＫ</param>
	void AddCombo(int add = 1) { m_combo += add; }
	/// <summary>
	/// コンボをリセットします
	/// </summary>
	void ResetCombo() { m_combo = 0; }
	/// <summary>
	/// スコアを取得します
	/// </summary>
	/// <returns>現在のスコア</returns>
	int GetScore() { return m_score; }
	/// <summary>
	/// コンボ数を取得します
	/// </summary>
	/// <returns>現在のコンボ数</returns>
	int GetCombo() { return m_combo; }
	/// <summary>
	/// すべての値を初期化します。
	/// </summary>
	void Reset() { m_combo = 0; m_score = 0; }
private:
	int m_score;
	int m_combo;

public:
	/// <summary>
	/// シングルトンインスタンス取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static CScore* Ins() { if (m_Ins) return m_Ins; m_Ins = new CScore(); return m_Ins; }

private:
	CScore() { Reset(); }
	static CScore* m_Ins;
};

