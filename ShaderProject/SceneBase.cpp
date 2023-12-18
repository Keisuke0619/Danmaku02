#include "SceneBase.hpp"
#include "Sprite.h"
#include "CollisionSystem.h"
SceneBase::Objects SceneBase::m_objects;

SceneBase::SceneBase()
	: m_pParent(nullptr)
	, m_pSubScene(nullptr)
{
}
SceneBase::~SceneBase()
{
	// �T�u�V�[�����폜
	RemoveSubScene();

	// �폜
	Items::iterator it = m_items.begin();
	while (it != m_items.end())
	{
		m_objects.erase(*it);
		++it;
	}
	m_items.clear();

	// �e�̎Q�Ƃ��폜
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

/// @brief �T�u�V�[���̍폜
void SceneBase::RemoveSubScene()
{
	// �폜����T�u�V�[�������݂��邩
	if (!m_pSubScene) return;

	// �K�w���̃T�u�V�[����D�悵�č폜
	m_pSubScene->RemoveSubScene();
	Sprite::Reset();
	// �����̃T�u�V�[�����폜
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
	// �����蔻��
	auto collisionList = CCollisionSystem::GetIns()->GetList();
	for (auto itr = collisionList->begin(); itr != collisionList->end(); itr++)
	{
		bool isCollision = false;
#ifdef  _DEBUG
		if (itr->first->GetIsBoxCollision() && itr->second->GetIsBoxCollision())
		{
			CDebugWindow::Print("�l�p�Ǝl�p�̓����蔻��I�I\n");
			continue;
		}
#endif //  _DEBUG

		//if (itr->first->GetIsBoxCollision())
		//{
		//	CollisionCircleBox(itr->second, itr->first);
		//}
		//else if (itr->second->GetIsBoxCollision())
		//{
		//	CollisionCircleBox(itr->first, itr->second);

		//}
		//else
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
	DirectX::XMVECTOR vVtx[4];
	DirectX::XMFLOAT2 boxCenter = DirectX::XMFLOAT2(box->GetPos().x, box->GetPos().z);
	for (int i = 0; i < 4; i++)
	{
		boxVtx[i] =  DirectXUtil::Mul(vtxVector[i], box->GetColliderScale() * 0.5f + circle->GetColliderScale() * 0.5f);
		vVtx[i] = DirectX::XMLoadFloat2(&(boxVtx[i]));
	}
	for (int i = 0; i < 4; i++)
	{
		//DirectX::XMVector2Cross
	}
}
