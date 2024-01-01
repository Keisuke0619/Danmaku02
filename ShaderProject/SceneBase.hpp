#ifndef __SCENE_BASE_HPP__
#define __SCENE_BASE_HPP__

#include <memory>
#include <map>
#include <string>
#include <list>
#include <Windows.h>
#include "Object.h"

// @breif �V�[����{�N���X
class SceneBase
{
private:
	using Objects = std::map<std::string, void*>;
	using Items = std::list<std::string >;
public:
	SceneBase();
	virtual ~SceneBase();
	void _update(float tick);
	void _draw();

	// �V�[������֐�
	template<class T> T* AddSubScene();
	void RemoveSubScene();

	// �I�u�W�F�N�g����֐�
	template<class T> T* CreateObj(const char* name);
	void DestroyObj(const char* name);
	template<class T> T* GetObj(const char* name);

	// �p���V�[���̈�ʂ�̏���
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update(float tick) = 0;
	virtual void Draw() = 0;

	void UpdateCollision();
private:
	static Objects m_objects;
protected:
	SceneBase* m_pParent;
	SceneBase* m_pSubScene;
	Items m_items;
	int m_frame = 0;


private:
	bool CollisionCircleCircle(CObject* circle1, CObject* circle2);
	bool CollisionCircleBox(CObject* circle, CObject* box);
	bool CheckCross(DirectX::XMFLOAT2 point, DirectX::XMFLOAT2 vtx[3]);
};

/// <summary>
/// �T�u�V�[���̒ǉ�
/// </summary>
/// <typeparam name="T">�T�u�V�[���̌^</typeparam>
/// <returns>���������T�u�V�[��</returns>
template<class T> T* SceneBase::AddSubScene()
{
	RemoveSubScene();
	T* pScene = new T;
	m_pSubScene = pScene;
	pScene->m_pParent = this;
	pScene->Init();
	return pScene;
}

/// <summary>
/// �I�u�W�F�N�g�̐���
/// </summary>
/// <typeparam name="T">�I�u�W�F�N�g�̌^</typeparam>
/// <param name="name">�I�u�W�F�N�g�̖���</param>
/// <returns>���������I�u�W�F�N�g</returns>
template<class T> T* SceneBase::CreateObj(const char* name)
{
#if 0
	// �f�o�b�O���̂݁A���̃_�u�肪�Ȃ����`�F�b�N
	Objects::iterator it = m_objects.find(name);
	if (it != m_objects.end()) {
		static char buf[256];
		sprintf_s(buf, sizeof(buf), "Failed to create object. %s", name);
		MessageBox(NULL, buf, "Error", MB_OK);
		return nullptr;
	}
#endif // _DEBUG

	// �I�u�W�F�N�g����
	T* ptr = new T;
	m_objects.insert(std::pair<std::string, void*>(name, ptr));
	m_items.push_back(name);
	return ptr;
}

/// <summary>
/// �I�u�W�F�N�g�̎擾
/// </summary>
/// <typeparam name="T">�I�u�W�F�N�g�̌^</typeparam>
/// <param name="name">�I�u�W�F�N�g�̖���</param>
/// <returns>�擾�����I�u�W�F�N�g</returns>
template<class T> T* SceneBase::GetObj(const char* name)
{
	// �I�u�W�F�N�g�̒T��
	Objects::iterator it = m_objects.find(name);
	if (it == m_objects.end()) return nullptr;

	// �^�ϊ�
	T* ptr = (T*)it->second;
	return ptr;
}

#endif // __SCENE_BASE_HPP__