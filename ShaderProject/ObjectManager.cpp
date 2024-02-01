#include "ObjectManager.h"
#include "DebugText.h"


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
	DebugText::SetData(DebugText::SLOT_OBJECTS, m_objects.size());
}

// 全オブジェクトのドローを回す。描画マスクにより、スキップすることもある。
void CObjectManager::Draw(Shader* vs, Shader* ps, unsigned drawMask)
{
	for (auto obj : m_objects)
	{
		if (obj == nullptr)continue;
		if ((obj->GetRenderStageMask() & drawMask) == 0) continue;
		obj->Draw(vs, ps);
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
