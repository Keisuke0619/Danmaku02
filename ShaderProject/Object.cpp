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

	// ワールド行列計算
	auto T = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	auto R = DirectX::XMMatrixRotationY(m_rot.y);
	R *= DirectX::XMMatrixRotationX(m_rot.x);
	R *= DirectX::XMMatrixRotationZ(m_rot.z);
	auto S = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	auto mat = S * R * T;
	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMStoreFloat4x4(&m_world, mat);

	// 当たり判定再登録
	if (m_useCollider)
	{
		CCollisionSystem::GetIns()->Regist(
			&m_collisionData,
			m_pos.x - m_colliderScale * 0.5f,
			m_pos.z - m_colliderScale * 0.5f,
			m_pos.x + m_colliderScale * 0.5f,
			m_pos.z + m_colliderScale * 0.5f
		);
	}
}

void CObject::Update()
{
}


CObjectManager* CObjectManager::m_ins;

CObjectManager::CObjectManager()
{

}

// 全オブジェクトのアップデート（ベース）を回す
void CObjectManager::Update()
{
	for (auto obj : m_objects)
	{
		if (obj == nullptr) continue;
		obj->UpdateBase();
	}
}

// 全オブジェクトのドローを回す。描画マスクにより、スキップすることもある。
void CObjectManager::Draw(Shader* vs, Shader* ps, unsigned drawMask)
{
	for (auto obj : m_objects)
	{
		if (obj == nullptr)continue;
		if ((obj->GetRenderStageMask() & drawMask) == 0) continue;
		obj->Draw(vs,ps);
	}
}
// 削除リストのオブジェクトをメインのリストから消して開放する。
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
