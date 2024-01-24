#include "Object.h"
#include "DebugWindow.hpp"
#include "DebugText.h"
CObject::CObject()
{
	m_collisionData.obj = this;
	m_collisionData.isStack = false;
	DirectX::XMStoreFloat4x4(&m_world, DirectX::XMMatrixIdentity());
	CObjectManager::GetIns()->Add(this);
	m_frame = 0;
}

CObject::~CObject()
{
	m_collisionData.Remove();
}

void CObject::Destroy(bool isRoot)
{
	for (auto child : m_childObj)
	{
		child->Destroy(isRoot = false);
	}
	if (m_registedDestroy == false)
	{
		CObjectManager::GetIns()->Destroy(this);
		if (m_parent && isRoot)
		{
			m_parent->m_childObj.remove(this);
		}
		m_registedDestroy = true;
	}
}

void CObject::UseCollision(bool isStack)
{
	m_useCollider = true;
	m_collisionData.isStack = isStack;
}

void CObject::UpdateBase()
{
	// �����蔻������X�g���폜
	if (m_useCollider && m_isStaticPosition == false)
	{
		m_collisionData.Remove();
	}

	// �h���N���X�̃A�b�v�f�[�g���񂵁AFrame�����Z
	Update();
	m_frame++;

	// �ÓI�I�u�W�F�N�g�Ȃ�World�v�Z�A�����蔻��̕t���ւ��͂���Ȃ��̂ŃX�L�b�v�B
	if (m_isStaticPosition) { return; }	

	ReloadWorldMatrix();
	if (m_isBoxCollision)
	{
		ReloadVtxVector();
	}
	// �����蔻��ēo�^
	if (m_useCollider)
	{
		RegistCollision();
	}
}

void CObject::Update()
{
}

void CObject::RegistCollision()
{
	auto elem = CCollisionSystem::GetIns()->Regist(
		&m_collisionData,
		m_pos.x - m_colliderScale,
		m_pos.z - m_colliderScale,
		m_pos.x + m_colliderScale,
		m_pos.z + m_colliderScale
	);
}

void CObject::ReloadWorldMatrix()
{
	// ���[���h�s��v�Z
	auto T = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	auto R = DirectX::XMMatrixRotationY(m_rot.y);
	R *= DirectX::XMMatrixRotationX(m_rot.x);
	R *= DirectX::XMMatrixRotationZ(m_rot.z);
	auto S = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	auto mat = S * R * T;
	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMStoreFloat4x4(&m_world, mat);

}

void CObject::ReloadVtxVector()
{
	for (int i = 0; i < 4; i++)
	{
		m_boxVtxVector[i] = DirectX::XMFLOAT2(cosf(m_rot.y + 6.28f * 0.125f + 6.28f * 0.25f * i), sinf(m_rot.y + 6.28f * 0.125f + 6.28f * 0.25f * i));
	}
}


CObjectManager* CObjectManager::m_ins;

CObjectManager::CObjectManager()
{

}

// �S�I�u�W�F�N�g�̃A�b�v�f�[�g�i�x�[�X�j����
void CObjectManager::Update()
{
	for (auto obj : m_objects)
	{
		if (obj == nullptr) continue;
		obj->UpdateBase();
	}
	DebugText::SetData(DebugText::SLOT_OBJECTS, m_objects.size());
}

// �S�I�u�W�F�N�g�̃h���[���񂷁B�`��}�X�N�ɂ��A�X�L�b�v���邱�Ƃ�����B
void CObjectManager::Draw(Shader* vs, Shader* ps, unsigned drawMask)
{
	for (auto obj : m_objects)
	{
		if (obj == nullptr)continue;
		if ((obj->GetRenderStageMask() & drawMask) == 0) continue;
		obj->Draw(vs,ps);
	}
}
// �폜���X�g�̃I�u�W�F�N�g�����C���̃��X�g��������ĊJ������B
void CObjectManager::RemoveUpdate()
{
	for (auto destroy : m_destroy)
	{
		if (destroy == nullptr) continue;
		m_objects.remove(destroy);
		delete destroy;
		destroy = nullptr;
	}
	m_destroy.clear();
}

void CObjectManager::Add(CObject* obj)
{
	m_objects.push_back(obj);
}

void CObjectManager::Destroy(CObject* obj)
{

	m_destroy.push_back(obj);
}

void CObjectManager::DestroyAll()
{
	auto itr = m_objects.begin();
	while (itr != m_objects.end())
	{
		delete (*itr);
		*itr = nullptr;
		itr = m_objects.erase(itr);
	}
	m_destroy.clear();
}

void CObjectManager::RemoveList(void* obj)
{
	CObject* ptr = (CObject*)obj;
	auto result = std::find(m_objects.begin(), m_objects.end(), ptr);
	if (result != m_objects.end())
	{
		m_objects.remove((CObject*)obj);
	}
}
