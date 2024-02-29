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

// �V�X�e���N���X
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
	// ��Ԃ��쐬�B�ő�[�x�ȉ����m�F�B
	if (level >= QUAD_TREE_MAX_LEVEL) { return; }
	// ��Ԃ̐�������o���A�����B
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
	// �����ꏊ������o���A��Ԃɓo�^�B��Ԃ����݂��Ȃ��ꍇ�͍쐬���A�o�^�B
	auto elem = GetMortonNum(left, top, right, bottom);
	if (elem < (unsigned)m_spaceNum)	// ��Ԃ��ő吔�𒴂��Ă��Ȃ����m�F����B
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
	
	// ���[�g���A�q�̋�Ԃ����Ă����B���̒��œ����蔻��y�A���X�g�����������Ă����B
	GetChild(stackObj, 0);
	DebugDrawPairLine();

	DebugText::SetData(DebugText::SLOT_COLLISION, m_collisionPair.size());
	return &m_collisionPair;
}

void CCollisionSystem::CreateSpace(unsigned elem)
{
	while (!m_space[elem])
	{
		// �w��̗v�f�ԍ��ɋ�Ԃ�V�K�쐬
		m_space[elem] = new CSpace;

		// �e��ԂɃW�����v
		elem = (elem - 1) >> 2;
		if (elem >= (unsigned)m_spaceNum) break;
	}
}

void CCollisionSystem::GetChild(std::list<CObject*>& stackObj, unsigned elem)
{
	std::list<CObject*> objList;
	TObjectMember* obj = nullptr;
	// ����Ԃ����݂���Ȃ�A�I�u�W�F�N�g���X�g���擾
	if (m_space[elem])
		obj = m_space[elem]->GetHead();
	unsigned stackNum = 0;
	while (obj != nullptr)
	{
		// �^�����蔻�葤�Ɣ퓖���蔻�葤������B�^���̓X�^�b�N���Ă����i����������������������Ȃ��j
		if (obj->isStack)
		{
			stackObj.push_back(obj->obj);
			stackNum++;
		}
		// ���[�J���̃��X�g�ɓ���Ă����B
		objList.push_back(obj->obj);
		obj = obj->next;
	}
	// Base�F�^�����蔻��@Passive�F�퓖���蔻��
	for (auto base = stackObj.begin(); base != stackObj.end(); base++)
	{
		for (auto passive = objList.begin(); passive != objList.end(); passive++)
		{
			// �����Ƃ͂Ԃ���Ȃ��B
			if (*base == *passive) { continue; }
			// �����^�O�̂��̂͂Ԃ���Ȃ��B
			if ((*base)->GetTagName() == (*passive)->GetTagName()) { continue; }
			// �y�A���쐬���A�����o�ϐ���Push
			Pair pair = std::make_pair(*base, *passive);
			m_collisionPair.push_back(pair);
		}
	}
	// ���g�̎q�����Q�Ƃ��ɍs��
	for (int i = 0; i < 4; i++)
	{
		auto nextElem = elem * 4 + 1 + i;
		if (nextElem < m_spaceNum)
		{
			GetChild(stackObj, nextElem);
		}
	}
	// �I���O�ɁA�^�����蔻�胊�X�g����폜
	for (unsigned int i = 0; i < stackNum; i++)
	{
		stackObj.pop_back();
	}
}

unsigned CCollisionSystem::GetMortonNum(float left, float top, float right, float bottom)
{
	// �w���`�����ID������o���B
	// �܂��A�ŏ��l�ƍő�l�̓_�̋��ID���擾
	auto min = GetPointElem(left, top);
	auto max = GetPointElem(right, bottom);

	// ���낢��v�Z���Ă��ׂĂ����܂�e��Ԃ�ID���擾���AReturn
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
	// ���[�g���̊���o��
	return (BitSeparate32(x) | (BitSeparate32(y) << 1));
}

unsigned CCollisionSystem::GetPointElem(float pos_x, float pos_y)
{
	// ���[�g�������ID�̊���o��
	return Get2DMortonNumber((unsigned)((pos_x - m_left) / m_spaceWidth), (unsigned)((pos_y - m_top) / m_spaceHeight));
}

void CCollisionSystem::DebugDrawPairLine()
{
	// �����蔻��y�A�̕`��B
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





//-----  ��ԃN���X  -----
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
	// Null�I�u�W�F����ɓo�^����Ă�����̂������ꍇ�͏I���B
	if (obj == nullptr) { return; }
	if (obj->space == this) { return; }
	// ����Ԃ��󂾂�����A�n���ꂽ�I�u�W�F�N�g��擪�Ƃ��ēo�^�B
	if (m_head == nullptr)
	{
		m_head = obj;
	}
	else
	{
		// �������A�擪�������ɂ��āA���܂Ő擪���������̂������̂��K�ɂ�������
		auto preHead = m_head;
		m_head = obj;
		m_head->next = preHead;
		preHead->pre = obj;
	}
	// �n����Ă����I�u�W�F�N�g�̏�����Ԃ�ۑ��B
	obj->space = this;
}

void CSpace::Remove(TObjectMember* obj)
{
	// ����������t���ւ��B
	if (obj == nullptr) { return; }
	if (m_head != obj) { return; }
	m_head = obj->next;
}

void TObjectMember::Remove()
{
	// �����̑O����Ȃ��Ŕ�����B
	if (space == nullptr) { return; }
	// ��Ԃ��甲����錾
	space->Remove(this);
	// �O��Ȃ�
	if (pre != nullptr)
	{
		pre->next = next;
	}
	if (next != nullptr)
	{
		next->pre = pre;
	}
	// ������
	pre = nullptr;
	next = nullptr;
	space = nullptr;
}
