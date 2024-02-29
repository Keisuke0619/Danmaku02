#include "CollisionSystem.h"
#include <windows.h>
#include "Object.h"
#include "Geometory.h"
#include "Input.h"
#include "CameraBase.h"
#include "Defines.h"
#include "DebugText.h"

CCollisionSystem* CCollisionSystem::Ins;
CCollisionSystem* CCollisionSystem::GetIns()
{
	if (Ins)return Ins;
	Ins = new CCollisionSystem();
	return Ins;
}

void CCollisionSystem::Delete()
{
	if (Ins == nullptr) return;
	delete Ins;
	Ins = nullptr;
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
	Ins = nullptr;
}

void CCollisionSystem::Create(int level, float left, float top, float right, float bottom)
{
	// 空間を作成。最大深度以下か確認。
	if (level >= QUAD_TREE_MAX_LEVEL) { return; }
	// 空間の数を割り出し、生成。
	m_spaceNum = (m_point[level + 1] - 1) / 3;
	m_space = new CSpace*[m_spaceNum];
	ZeroMemory(m_space, sizeof(CSpace*) * m_spaceNum);
	
	left *= WALL_SCALE;
	top *= WALL_SCALE;
	right *= WALL_SCALE;
	bottom *= WALL_SCALE;

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
	// 所属場所を割り出し、空間に登録。空間が存在しない場合は作成し、登録。
	auto elem = GetMortonNum(left, top, right, bottom);
	if (elem < (unsigned)m_spaceNum)	// 空間が最大数を超えていないか確認する。
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
	
	// ルートより、子の空間を見ていく。その中で当たり判定ペアリストを完成させていく。
	GetChild(stackObj, 0);
	DebugDrawPairLine();

	DebugText::SetData(DebugText::SLOT_COLLISION, m_collisionPair.size());
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
	// 自空間が存在するなら、オブジェクトリストを取得
	if (m_space[elem])
		obj = m_space[elem]->GetHead();
	unsigned stackNum = 0;
	while (obj != nullptr)
	{
		// 与当たり判定側と被当たり判定側がある。与側はスタックしておく（作り方がおかしいかもしれない）
		if (obj->isStack)
		{
			stackObj.push_back(obj->obj);
			stackNum++;
		}
		// ローカルのリストに入れていく。
		objList.push_back(obj->obj);
		obj = obj->next;
	}
	// Base：与当たり判定　Passive：被当たり判定
	for (auto base = stackObj.begin(); base != stackObj.end(); base++)
	{
		for (auto passive = objList.begin(); passive != objList.end(); passive++)
		{
			// 自分とはぶつからない。
			if (*base == *passive) { continue; }
			// 同じタグのものはぶつからない。
			if ((*base)->GetTagName() == (*passive)->GetTagName()) { continue; }
			// ペアを作成し、メンバ変数にPush
			Pair pair = std::make_pair(*base, *passive);
			m_collisionPair.push_back(pair);
		}
	}
	// 自身の子供を参照しに行く
	for (int i = 0; i < 4; i++)
	{
		auto nextElem = elem * 4 + 1 + i;
		if (nextElem < m_spaceNum)
		{
			GetChild(stackObj, nextElem);
		}
	}
	// 終わる前に、与当たり判定リストから削除
	for (unsigned int i = 0; i < stackNum; i++)
	{
		stackObj.pop_back();
	}
}

unsigned CCollisionSystem::GetMortonNum(float left, float top, float right, float bottom)
{
	// 指定矩形より空間IDを割り出す。
	// まず、最小値と最大値の点の空間IDを取得
	auto min = GetPointElem(left, top);
	auto max = GetPointElem(right, bottom);

	// いろいろ計算してすべてが収まる親空間のIDを取得し、Return
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
	// モートンの割り出し
	return (BitSeparate32(x) | (BitSeparate32(y) << 1));
}

unsigned CCollisionSystem::GetPointElem(float pos_x, float pos_y)
{
	// モートンより空間IDの割り出し
	return Get2DMortonNumber((unsigned)((pos_x - m_left) / m_spaceWidth), (unsigned)((pos_y - m_top) / m_spaceHeight));
}

void CCollisionSystem::DebugDrawPairLine()
{
	// 当たり判定ペアの描画。
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
	// Nullオブジェや既に登録されているものが来た場合は終了。
	if (obj == nullptr) { return; }
	if (obj->space == this) { return; }
	// 自空間が空だったら、渡されたオブジェクトを先頭として登録。
	if (m_head == nullptr)
	{
		m_head = obj;
	}
	else
	{
		// 違ったら、先頭を自分にして、今まで先頭だったものを自分のお尻にくっつける
		auto preHead = m_head;
		m_head = obj;
		m_head->next = preHead;
		preHead->pre = obj;
	}
	// 渡されてきたオブジェクトの所属空間を保存。
	obj->space = this;
}

void CSpace::Remove(TObjectMember* obj)
{
	// 頭だったら付け替え。
	if (obj == nullptr) { return; }
	if (m_head != obj) { return; }
	m_head = obj->next;
}

void TObjectMember::Remove()
{
	// 自分の前後をつないで抜ける。
	if (space == nullptr) { return; }
	// 空間から抜ける宣言
	space->Remove(this);
	// 前後つなぎ
	if (pre != nullptr)
	{
		pre->next = next;
	}
	if (next != nullptr)
	{
		next->pre = pre;
	}
	// 初期化
	pre = nullptr;
	next = nullptr;
	space = nullptr;
}
