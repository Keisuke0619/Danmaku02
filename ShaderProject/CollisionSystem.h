#pragma once

#include <list>
#define	QUAD_TREE_MAX_LEVEL (9)

class CSpace;
class CObject;
class TObjectMember
{
public:
	CObject* obj;
	bool isStack;	// �^�����蔻��g
	TObjectMember* pre;
	TObjectMember* next;
	CSpace* space;
	void Remove();
};

// �l���؃}�l�[�W��
class CCollisionSystem
{
public:
	static CCollisionSystem* GetIns();
	static void Delete();
private:
	static CCollisionSystem* Ins;
	using Pair = std::pair<CObject*, CObject*>;
	CSpace** m_space;
	float m_width;
	float m_height;
	float m_left;
	float m_top;
	float m_spaceWidth;
	float m_spaceHeight;
	int m_spaceNum;
	int m_lowestLevel;
	int m_point[QUAD_TREE_MAX_LEVEL + 1];
	std::list<Pair> m_collisionPair;
private:
	CCollisionSystem();
public:
	~CCollisionSystem();
	void Create(int level, float left, float top, float right, float bottom);
	unsigned Regist(TObjectMember* obj, float left, float top, float right, float bottom);
	std::list<Pair>* GetList();
private:
	void CreateSpace(unsigned elem);
	void GetChild(std::list<CObject*>& stackObj, unsigned elem);
	unsigned GetMortonNum(float left, float top, float right, float bottom);
	unsigned BitSeparate32(unsigned n);
	unsigned Get2DMortonNumber(unsigned x, unsigned y);
	unsigned GetPointElem(float pos_x, float pos_y);

	bool m_enablePairLine = false;
	void DebugDrawPairLine();
};

// ��ԃN���X
class CSpace
{
private:
	TObjectMember*m_head;
public:
	CSpace();
	virtual ~CSpace();
	TObjectMember* GetHead();
	/// <summary>
	/// ��ԂɃI�u�W�F�N�g��o�^
	/// </summary>
	/// <param name="obj">�o�^����I�u�W�F�N�g�̃f�[�^</param>
	void Push(TObjectMember* obj);
	/// <summary>
	/// �폜���ɐ擪���m�F
	/// </summary>
	/// <param name="obj">�폜����I�u�W�F�N�g�̃|�C���^</param>
	void Remove(TObjectMember* obj);
};