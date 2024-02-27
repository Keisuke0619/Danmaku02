#pragma once
#include "DebugText.h"
class CScore
{
public:
	void AddScore(int score) { m_score += (m_combo + 1) * score; DebugText::SetData(DebugText::SLOT_SCORE, m_score); }
	void AddCombo(int add = 1) { m_combo += add; }
	int GetScore() { return m_score; }
	void Reset() { m_combo = 0; m_score = 0; }
private:
	int m_score;
	int m_combo;

public:
	static CScore* Ins() { if (m_Ins) return m_Ins; m_Ins = new CScore(); return m_Ins; }

private:
	CScore() { Reset(); }
	static CScore* m_Ins;
};

