#include "Object.h"
#include "DebugWindow.hpp"
#include "DebugText.h"
#include "ObjectManager.h"
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
	// 当たり判定をリストより削除
	if (m_useCollider && m_isStaticPosition == false)
	{
		m_collisionData.Remove();
	}

	// 派生クラスのアップデートを回し、Frameを加算
	Update();
	m_frame++;

	// 静的オブジェクトならWorld計算、当たり判定の付け替えはいらないのでスキップ。
	if (m_isStaticPosition) { return; }	

	ReloadWorldMatrix();
	if (m_isBoxCollision)
	{
		ReloadVtxVector();
	}
	// 当たり判定再登録
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
	// ワールド行列計算
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
