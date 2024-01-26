#pragma once

#include "Object.h"
class CObjectManager
{

private:
	CObjectManager();
	std::list<CObject*> m_objects;
	std::list<CObject*> m_destroy;
public:
	void Update();
	void Draw(Shader* vs, Shader* ps, unsigned drawMask);
	void RemoveUpdate();
	void Add(CObject* obj);
	void Destroy(CObject* obj);
	void DestroyAll();

	// Use in SceneBase ONLY.
	void RemoveList(void* obj);
private:
	static CObjectManager* m_ins;
public:
	static CObjectManager* GetIns() { if (m_ins) return m_ins; m_ins = new CObjectManager(); return m_ins; }
};