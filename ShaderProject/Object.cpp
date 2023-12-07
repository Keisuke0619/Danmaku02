#include "Object.h"

CObject::CObject()
{
	m_collisionData.obj = this;
	m_collisionData.isStack = false;
	DirectX::XMStoreFloat4x4(&m_world, DirectX::XMMatrixIdentity());
	CObjectManager::GetIns()->Add(this);
}

void CObject::Destroy()
{
	CObjectManager::GetIns()->Destroy(this);
}

void CObject::UpdateBase()
{
	Update();
	auto T = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	auto R = DirectX::XMMatrixRotationY(m_rot.y);
	R *= DirectX::XMMatrixRotationX(m_rot.x);
	R *= DirectX::XMMatrixRotationZ(m_rot.z);
	auto S = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	auto mat = S * R * T;
	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMStoreFloat4x4(&m_world, mat);
}

void CObject::Update()
{
}


CObjectManager* CObjectManager::m_ins;

void CObjectManager::Update()
{
	for (auto obj : m_objects)
	{
		obj->UpdateBase();
	}
}

void CObjectManager::Draw()
{
	for (auto obj : m_objects)
	{
		obj->Draw();
	}
}

void CObjectManager::RemoveUpdate()
{
	for (auto destroy : m_destroy)
	{
		m_objects.remove(destroy);
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
