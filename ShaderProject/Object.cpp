#include "Object.h"

CObject::CObject()
{
	m_collisionData.obj = this;
	m_collisionData.isStack = false;
	DirectX::XMStoreFloat4x4(&m_world, DirectX::XMMatrixIdentity());
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
