#ifndef __CAMERA_BASE_H__
#define __CAMERA_BASE_H__

#include <DirectXMath.h>
#include <Object.h>
class CameraBase : public CObject
{
public:
	static CameraBase* GetPrimary();
	static void SetPrimary(CameraBase* primary) { m_primary = primary; }
protected:
	static CameraBase* m_primary;
	static CameraBase* m_pre;
public:
	CameraBase();
	virtual ~CameraBase();

#ifdef _DEBUG
	void Draw();
#else
	void Draw() {}
#endif

	virtual void Update() = 0;

	DirectX::XMFLOAT4X4 GetView(bool transpose = true);
	DirectX::XMFLOAT4X4 GetProj(bool transpose = true);

	DirectX::XMFLOAT3 GetPos();
	DirectX::XMFLOAT3 GetLook();
	DirectX::XMFLOAT3 GetUp();

	void SetPos(DirectX::XMFLOAT3 pos);
	void SetLook(DirectX::XMFLOAT3 look);
	void SetUp(DirectX::XMFLOAT3 up);
	void SetFovY(float degFovY);
protected:
	DirectX::XMFLOAT3 m_look, m_up;
	float m_fovy, m_aspect, m_near, m_far;
};

#endif // __CAMERA_BASE_H__