#include "CollisionSystem.h"
#include <windows.h>
#include "Object.h"
#include "Geometory.h"
#include "Input.h"
#include "CameraBase.h"
CCollisionSystem* CCollisionSystem::Ins;
CCollisionSystem* CCollisionSystem::GetIns()
{
	if (Ins)return Ins;
	Ins = new CCollisionSystem();
	return Ins;
}

// システムクラス
CCollisionSystem::CCollisionSystem()
{
	m_width = 0;
	m_height = 0;
	m_top = 0;
	m_left = 0;
	m_spaceWidth = 0;
	m_spaceHeight = 0;
	m_lowestLevel = 0;
	m_space = nullptr;
	m_spaceNum = 0;
	m_point[0] = 1;
	for (int i = 1; i <= QUAD_TREE_MAX_LEVEL; i++)
	{
		m_point[i] = m_point[i - 1] * 4;
	}
	m_point[0] = 0;
}

CCollisionSystem::~CCollisionSystem()
{
	for (int i = 0; i < m_spaceNum; i++)
	{
		if (m_space[i])
		{
			delete m_space[i];
			m_space[i] = nullptr;
		}
	}
	delete[] m_space;
	m_space = nullptr;
}

void CCollisionSystem::Create(int level, float left, float top, float right, float bottom)
{
	if (level >= QUAD_TREE_MAX_LEVEL) { return; }

	m_spaceNum = (m_point[level + 1] - 1) / 3;
	m_space = new CSpace*[m_spaceNum];
	ZeroMemory(m_space, sizeof(CSpace*) * m_spaceNum);

	m_left = left;
	m_top = top;
	m_width = right - left;
	m_height = bottom - top;
	m_spaceWidth = m_width / (1 << level);
	m_spaceHeight = m_height / (1 << level);

	m_lowestLevel = level;
}

unsigned CCollisionSystem::Regist(TObjectMember* obj, float left, float top, float right, float bottom)
{
	auto elem = GetMortonNum(left, top, right, bottom);
	if (elem < (unsigned)m_spaceNum)
	{
		if (m_space[elem] == nullptr)
		{
			CreateSpace(elem);
		}
		m_space[elem]->Push(obj);
	}
	return elem;
}

std::list<CCollisionSystem::Pair>* CCollisionSystem::GetList()
{
	m_collisionPair.clear();
	if (m_space == nullptr) { return nullptr; }
	std::list<CObject*> stackObj;
	
	GetChild(stackObj, 0);
	DebugDrawPairLine();
	return &m_collisionPair;
}

void CCollisionSystem::CreateSpace(unsigned elem)
{
	while (!m_space[elem])
	{
		// 指定の要素番号に空間を新規作成
		m_space[elem] = new CSpace;

		// 親空間にジャンプ
		elem = (elem - 1) >> 2;
		if (elem >= (unsigned)m_spaceNum) break;
	}
}

void CCollisionSystem::GetChild(std::list<CObject*>& stackObj, unsigned elem)
{
	std::list<CObject*> objList;
	TObjectMember* obj = nullptr;
	if (m_space[elem])
		obj = m_space[elem]->GetHead();
	unsigned stackNum = 0;
	while (obj != nullptr)
	{
		if (obj->isStack)
		{
			stackObj.push_back(obj->obj);
			stackNum++;
		}
		objList.push_back(obj->obj);
		obj = obj->next;
	}
	for (auto base = stackObj.begin(); base != stackObj.end(); base++)
	{
		for (auto passive = objList.begin(); passive != objList.end(); passive++)
		{
			if (*base == *passive) { continue; }
			if ((*base)->GetTagName() == (*passive)->GetTagName()) { continue; }
			Pair pair = std::make_pair(*base, *passive);
			m_collisionPair.push_back(pair);
		}
	}
	for (int i = 0; i < 4; i++)
	{
		auto nextElem = elem * 4 + 1 + i;
		if (nextElem < m_spaceNum)
		{
			GetChild(stackObj, nextElem);
		}
	}
	for (unsigned int i = 0; i < stackNum; i++)
	{
		stackObj.pop_back();
	}
}

unsigned CCollisionSystem::GetMortonNum(float left, float top, float right, float bottom)
{
	auto min = GetPointElem(left, top);
	auto max = GetPointElem(right, bottom);

	auto def = min ^ max;
	unsigned level = 0;
	for (int i = 0; i < m_lowestLevel; i++)
	{
		auto check = (def >> (i * 2)) & 0x3;
		if (check != 0)
		{
			level = i + 1;
		}
	}
	auto spaceNum = min >> (level * 2);
	auto addNum = (m_point[m_lowestLevel - level] - 1) / 3;
	spaceNum += addNum;

	if (spaceNum > (unsigned)m_spaceNum)
		return 0xffffffff;

	return spaceNum;
}

unsigned CCollisionSystem::BitSeparate32(unsigned n)
{
	n = (n | (n << 8)) & 0x00ff00ff;
	n = (n | (n << 4)) & 0x0f0f0f0f;
	n = (n | (n << 2)) & 0x33333333;
	return (n | (n << 1)) & 0x55555555;
}

unsigned CCollisionSystem::Get2DMortonNumber(unsigned x, unsigned y)
{
	return (BitSeparate32(x) | (BitSeparate32(y) << 1));
}

unsigned CCollisionSystem::GetPointElem(float pos_x, float pos_y)
{
	return Get2DMortonNumber((unsigned)((pos_x - m_left) / m_spaceWidth), (unsigned)((pos_y - m_top) / m_spaceHeight));
}

void CCollisionSystem::DebugDrawPairLine()
{
	if (IsKeyTrigger('P'))
	{
		m_enablePairLine = !m_enablePairLine;
	}
	if (m_enablePairLine == false) { return; }
	for (auto pair : m_collisionPair)
	{
		auto start = pair.first->GetPos();
		auto end = pair.second->GetPos();
		Geometory::AddLine(start, DirectX::XMFLOAT4(1, 0, 0, 1), end, DirectX::XMFLOAT4(0, 0, 1, 1));
	}
	Geometory::SetView(CameraBase::GetPrimary()->GetView());
	Geometory::SetProjection(CameraBase::GetPrimary()->GetProj());
}





//-----  空間クラス  -----
CSpace::CSpace()
{
	m_head = nullptr;
}

CSpace::~CSpace()
{
}

TObjectMember* CSpace::GetHead()
{
	return m_head;
}

void CSpace::Push(TObjectMember* obj)
{
	if (obj == nullptr) { return; }
	if (obj->space == this) { return; }
	if (m_head == nullptr)
	{
		m_head = obj;
	}
	else
	{
		auto preHead = m_head;
		m_head = obj;
		m_head->next = preHead;
		preHead->pre = obj;
	}
	obj->space = this;
}

void CSpace::Remove(TObjectMember* obj)
{
	if (obj == nullptr) { return; }
	if (m_head != obj) { return; }
	m_head = obj->next;
}

void TObjectMember::Remove()
{
	if (space == nullptr) { return; }
	space->Remove(this);
	if (pre != nullptr)
	{
		pre->next = next;
	}
	if (next != nullptr)
	{
		next->pre = pre;
	}
	pre = nullptr;
	next = nullptr;
	space = nullptr;
}
