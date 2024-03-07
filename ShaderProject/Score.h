#pragma once
#include "DebugText.h"

/// <summary>
/// �X�R�A���ۑ��p�N���X
/// </summary>
class CScore
{
public:
	/// <summary>
	/// �X�R�A�����Z���܂��B��b�X�R�A���R���{�������Z�ʂł��B
	/// </summary>
	/// <param name="score">��b�X�R�A</param>
	void AddScore(int score) { m_score += (m_combo + 1) * score; DebugText::SetData(DebugText::SLOT_SCORE, m_score); }
	/// <summary>
	/// �R���{�������Z���܂��B��{�I�Ɉ����͂Ȃ��łn�j�B
	/// </summary>
	/// <param name="add">���Z����ʁB��{�P�łn�j</param>
	void AddCombo(int add = 1) { m_combo += add; }
	/// <summary>
	/// �R���{�����Z�b�g���܂�
	/// </summary>
	void ResetCombo() { m_combo = 0; }
	/// <summary>
	/// �X�R�A���擾���܂�
	/// </summary>
	/// <returns>���݂̃X�R�A</returns>
	int GetScore() { return m_score; }
	/// <summary>
	/// �R���{�����擾���܂�
	/// </summary>
	/// <returns>���݂̃R���{��</returns>
	int GetCombo() { return m_combo; }
	/// <summary>
	/// ���ׂĂ̒l�����������܂��B
	/// </summary>
	void Reset() { m_combo = 0; m_score = 0; }
private:
	int m_score;
	int m_combo;

public:
	/// <summary>
	/// �V���O���g���C���X�^���X�擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static CScore* Ins() { if (m_Ins) return m_Ins; m_Ins = new CScore(); return m_Ins; }

private:
	CScore() { Reset(); }
	static CScore* m_Ins;
};

