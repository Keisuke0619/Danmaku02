#include "SceneBase.hpp"
#include "Sprite.h"
#include "CollisionSystem.h"
#include "Object.h"
SceneBase::Objects SceneBase::m_objects;

SceneBase::SceneBase()
	: m_pParent(nullptr)
	, m_pSubScene(nullptr)
{
}
SceneBase::~SceneBase()
{
	for (auto obj : m_objects)
	{
		if (obj.second != nullptr)
		{
			CObjectManager::GetIns()->RemoveList(obj.second);
			delete obj.second;
			obj.second = nullptr;
		}
	}
	m_objects.clear();

	CObjectManager::GetIns()->DestroyAll();
	// サブシーンを削除
	RemoveSubScene();

	// 削除
	Items::iterator it = m_items.begin();
	while (it != m_items.end())
	{
		m_objects.erase(*it);
		++it;
	}
	m_items.clear();
	

	// 親の参照を削除
	if(m_pParent)
		m_pParent->m_pSubScene = nullptr;
}
void SceneBase::_update(float tick)
{
	m_frame++;
	if (m_pSubScene)
		m_pSubScene->_update(tick);
	Update(tick);
}
void SceneBase::_draw()
{
	if (m_pSubScene)
		m_pSubScene->_draw();
	Draw();
}

/// @brief サブシーンの削除
void SceneBase::RemoveSubScene()
{
	// 削除するサブシーンが存在するか
	if (!m_pSubScene) return;

	// 階層内のサブシーンを優先して削除
	m_pSubScene->RemoveSubScene();
	Sprite::Reset();
	// 直下のサブシーンを削除
	m_pSubScene->Uninit();

	delete m_pSubScene;
	m_pSubScene = nullptr;
}

void SceneBase::DestroyObj(const char* name)
{
	m_objects.erase(name);
	m_items.remove(name);
}

void SceneBase::UpdateCollision()
{
	// 当たり判定
	auto collisionList = CCollisionSystem::GetIns()->GetList();
	for (auto itr = collisionList->begin(); itr != collisionList->end(); itr++)
	{
		bool isCollision = false;
#ifdef  _DEBUG
		if (itr->first->GetIsBoxCollision() && itr->second->GetIsBoxCollision())
		{
			CDebugWindow::Print("四角と四角の当たり判定！！\n");
			continue;
		}
#endif //  _DEBUG

		if (itr->first->GetIsBoxCollision())
		{
			isCollision = CollisionCircleBox(itr->second, itr->first);
		}
		else if (itr->second->GetIsBoxCollision())
		{
			isCollision = CollisionCircleBox(itr->first, itr->second);

		}
		else
		{
			isCollision = CollisionCircleCircle(itr->first, itr->second);
		}
		if (isCollision)
		{
			itr->first->OnCollision(itr->second);
			itr->second->OnCollision(itr->first);
		}
	}

}

bool SceneBase::CollisionCircleCircle(CObject* circle1, CObject* circle2)
{
	auto firstPos = circle1->GetPos();
	auto secondPos = circle2->GetPos();
	auto firstScale = circle1->GetColliderScale();
	auto secondScale = circle2->GetColliderScale();
	float xzPowLength = powf(firstPos.x - secondPos.x, 2) + powf(firstPos.z - secondPos.z, 2);
	float colliderPowLength = powf(firstScale * 0.5f + secondScale * 0.5f, 2);
	
	return xzPowLength < colliderPowLength;
}

bool SceneBase::CollisionCircleBox(CObject* circle, CObject* box)
{
	DirectX::XMFLOAT2* vtxVector = box->GetBoxVtxVector();
	DirectX::XMFLOAT2 boxVtx[4];
	DirectX::XMFLOAT2 boxCenter = DirectX::XMFLOAT2(box->GetPos().x, box->GetPos().z);
	DirectX::XMFLOAT2 circleCenter = DirectX::XMFLOAT2(circle->GetPos().x, circle->GetPos().z);
	for (int i = 0; i < 4; i++)
	{
		boxVtx[i] =  DirectXUtil::Mul(vtxVector[i], box->GetColliderScale() * 0.7f + circle->GetColliderScale() * 0.50f);
		DirectXUtil::Increment(&(boxVtx[i]), boxCenter);
	}
	DirectX::XMFLOAT2 tmp[3] =
	{
		boxVtx[0],
		boxVtx[1],
		boxVtx[2]
	};
	if (CheckCross(circleCenter, tmp))
	{
		return true;
	}
	tmp[0] = boxVtx[2];
	tmp[1] = boxVtx[0];
	tmp[2] = boxVtx[3];
	return CheckCross(circleCenter, tmp);
}

bool SceneBase::CheckCross(DirectX::XMFLOAT2 point, DirectX::XMFLOAT2 vtx[3])
{
	DirectX::XMVECTOR vCross[3];
	DirectX::XMVECTOR vPoint = DirectX::XMLoadFloat2(&point);
	DirectX::XMVECTOR vTriStart = DirectX::XMLoadFloat2(vtx);

	for (int i = 0; i < 3; i++)
	{
		auto vTriEnd = DirectX::XMLoadFloat2(&(vtx[(i + 1) % 3]));
		auto vTriEdge = DirectX::XMVectorSubtract(vTriEnd, vTriStart);
		auto vToPoint = DirectX::XMVectorSubtract(vPoint, vTriStart);
		vCross[i] = DirectX::XMVector2Cross(vTriEdge, vToPoint);
		vCross[i] = DirectX::XMVector2Normalize(vCross[i]);
		vTriStart = vTriEnd;
	}
	DirectX::XMVECTOR vDot[3] =
	{
		DirectX::XMVector2Dot(vCross[0], vCross[1]),
		DirectX::XMVector2Dot(vCross[1], vCross[2]),
		DirectX::XMVector2Dot(vCross[2], vCross[0])
	};
	float match[3];
	for (int i = 0; i < 3; i++)
	{
		DirectX::XMStoreFloat(&match[i], vDot[i]);
	}
	return match[0] >= 0.999f && match[1] >= 0.999f && match[2] >= 0.999f;
}
