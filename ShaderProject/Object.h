#pragma once

#include <DirectXMath.h>
#include <string>
#include <list>
#include "CollisionSystem.h"
enum EDrawMask
{
	RENDER_BUFFER = 1 << 0,
	RENDER_ALPHA = 1 << 1,

};


class CObject
{
protected:
	DirectX::XMFLOAT3 m_pos		= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 m_rot		= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 m_scale	= DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT4X4 m_world;
	std::string m_tag = "NO_TAG";
	unsigned m_renderStageMask = 1;
	// “–‚½‚è”»’è
	TObjectMember m_collisionData = {};
	float m_colliderScale = 1.0f;
	bool m_isStack = false;
	bool m_useCollider = false;
public:
	CObject();
	void SetColliderScale(float scale) { m_colliderScale = scale; }
	void Destroy();



	virtual void UpdateBase() final;
	virtual void Draw(int texSlot = 0) {}
	virtual void OnCollision(CObject* _obj) {}
protected:
	virtual void Update();

};

class CObjectManager
{
private:
	static CObjectManager* m_ins;
public:
	static CObjectManager* GetIns() { if (m_ins) return m_ins; m_ins = new CObjectManager(); return m_ins; }

private:
	CObjectManager();
	std::list<CObject*> m_objects;
	std::list<CObject*> m_destroy;
public:
	void Update();
	void Draw();
	void RemoveUpdate();
	void Add(CObject* obj);
	void Destroy(CObject* obj);
};