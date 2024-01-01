#pragma once

#include <DirectXMath.h>
#include <string>
#include <list>
#include "CollisionSystem.h"
#include "Shader.h"
#include "DirectXMathUtil.h"
#include "DebugWindow.hpp"
enum EDrawMask
{
	RENDER_MODEL = 1 << 0,
	RENDER_ALPHA = 1 << 1,

};


class CObject
{
protected:
	DirectX::XMFLOAT3 m_pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 m_rot = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 m_scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT4X4 m_world;
	std::string m_tag = "NO_TAG";
	unsigned m_renderStageMask = 1;
	// �����蔻��
	TObjectMember m_collisionData = {};
	float m_colliderScale = 0.2f;
	bool m_isStack = false;
	bool m_useCollider = false;
	int m_frame;
	bool m_isStaticPosition = false;	// �o�^�폜�𖈃t���[���s��Ȃ������蔻��
	bool m_isBoxCollision = false;
	DirectX::XMFLOAT2 m_boxVtxVector[4] = {};
public:
	CObject();
	virtual ~CObject();

	// ���[�e�B���e�B�֐��n


	void SetColliderScale(float scale) { m_colliderScale = scale; }
	void Destroy(bool isRoot = true);



	DirectX::XMFLOAT3	GetPos() { return m_pos; }
	DirectX::XMFLOAT3	GetRotation() { return m_rot; }
	DirectX::XMFLOAT3	GetScale() { return m_scale; }
	std::string			GetTagName() { return m_tag; }
	unsigned			GetRenderStageMask() { return m_renderStageMask; }
	float				GetColliderScale() { return m_colliderScale; }
	DirectX::XMFLOAT4X4 GetWorld() { return m_world; }
	bool				GetIsBoxCollision() { return m_isBoxCollision; }
	DirectX::XMFLOAT2*	GetBoxVtxVector() { return m_boxVtxVector; }

	//					�Z�b�^�[

	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="pos">�V�������W</param>
	void SetPos(DirectX::XMFLOAT3 pos) { m_pos = pos; }
	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	/// <param name="rot">�e���ɂ������]�ʁi�ʓx�@�j</param>
	void SetRot(DirectX::XMFLOAT3 rot) { m_rot = rot; }
	/// <summary>
	/// �g�嗦��ݒ�
	/// </summary>
	/// <param name="scale">�e���ɂ�����g�嗦</param>
	void SetScale(DirectX::XMFLOAT3 scale) { m_scale = scale; }

	//					���̑�

	/// <summary>
	/// �����蔻����g�p����ꍇ�͂��̊֐����Ăяo���B
	/// </summary>
	/// <param name="isStack">�^�����蔻�葤�ɂȂ邩�B</param>
	void UseCollision(bool isStack = false);
	void AddChild(CObject* childObj) { m_childObj.push_back(childObj); childObj->m_parent = this; }
	// ���z�֐��n
	virtual void UpdateBase() final;
	virtual void Draw(Shader* vs, Shader* ps) {}
	virtual void OnCollision(CObject* _obj) {}
protected:
	virtual void Update();
	void RegistCollision();
	void ReloadWorldMatrix();
	void ReloadVtxVector();
private:
	std::list<CObject*> m_childObj;
	bool m_registedDestroy = false;
	CObject* m_parent = nullptr;
};

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