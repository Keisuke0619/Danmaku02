#include "Object.h"
#include "DebugWindow.hpp"
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
	if (m_useCollider)
	{
		m_collisionData.Remove();
	}
	Update();
	auto T = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	auto R = DirectX::XMMatrixRotationY(m_rot.y);
	R *= DirectX::XMMatrixRotationX(m_rot.x);
	R *= DirectX::XMMatrixRotationZ(m_rot.z);
	auto S = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	auto mat = S * R * T;
	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMStoreFloat4x4(&m_world, mat);
	if (m_useCollider)
	{
		auto data = CCollisionSystem::GetIns()->Regist(
			&m_collisionData,
			m_pos.x - m_colliderScale * 0.5f,
			m_pos.z - m_colliderScale * 0.5f,
			m_pos.x + m_colliderScale * 0.5f,
			m_pos.z + m_colliderScale * 0.5f
		);
	}
	m_frame++;
}

void CObject::Update()
{
}


CObjectManager* CObjectManager::m_ins;

CObjectManager::CObjectManager()
{

}
void CObjectManager::Update()
{
	for (auto obj : m_objects)
	{
		obj->UpdateBase();
	}
}

void CObjectManager::Draw(Shader* vs, Shader* ps, unsigned drawMask)
{
	for (auto obj : m_objects)
	{
		if (obj == nullptr)continue;
		if ((obj->GetRenderStageMask() & drawMask) == 0) continue;
		obj->Draw(vs,ps);
	}
}

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
