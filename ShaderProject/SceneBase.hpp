#ifndef __SCENE_BASE_HPP__
#define __SCENE_BASE_HPP__

#include <memory>
#include <map>
#include <string>
#include <list>
#include <Windows.h>
#include "Object.h"

// @breif シーン基本クラス
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

	// シーン操作関数
	template<class T> T* AddSubScene();
	void RemoveSubScene();

	// オブジェクト操作関数
	template<class T> T* CreateObj(const char* name);
	void DestroyObj(const char* name);
	template<class T> T* GetObj(const char* name);

	// 継承シーンの一通りの処理
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
/// サブシーンの追加
/// </summary>
/// <typeparam name="T">サブシーンの型</typeparam>
/// <returns>生成したサブシーン</returns>
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
/// オブジェクトの生成
/// </summary>
/// <typeparam name="T">オブジェクトの型</typeparam>
/// <param name="name">オブジェクトの名称</param>
/// <returns>生成したオブジェクト</returns>
template<class T> T* SceneBase::CreateObj(const char* name)
{
#if 0
	// デバッグ中のみ、名称ダブりがないかチェック
	Objects::iterator it = m_objects.find(name);
	if (it != m_objects.end()) {
		static char buf[256];
		sprintf_s(buf, sizeof(buf), "Failed to create object. %s", name);
		MessageBox(NULL, buf, "Error", MB_OK);
		return nullptr;
	}
#endif // _DEBUG

	// オブジェクト生成
	T* ptr = new T;
	m_objects.insert(std::pair<std::string, void*>(name, ptr));
	m_items.push_back(name);
	return ptr;
}

/// <summary>
/// オブジェクトの取得
/// </summary>
/// <typeparam name="T">オブジェクトの型</typeparam>
/// <param name="name">オブジェクトの名称</param>
/// <returns>取得したオブジェクト</returns>
template<class T> T* SceneBase::GetObj(const char* name)
{
	// オブジェクトの探索
	Objects::iterator it = m_objects.find(name);
	if (it == m_objects.end()) return nullptr;

	// 型変換
	T* ptr = (T*)it->second;
	return ptr;
}

#endif // __SCENE_BASE_HPP__