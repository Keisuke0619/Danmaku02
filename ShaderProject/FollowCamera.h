#pragma once
#include "CameraBase.h"

class CFollowCamera : public CameraBase
{
public:
	void Update() override;
	void SetTarget(CObject* target) { m_target = target; }
	void SetPosOffset(DirectX::XMFLOAT3 offset) { m_posOffset = offset; }
	void SetLookOffset(DirectX::XMFLOAT3 offset) { m_lookOffset = offset; }
private:
	DirectX::XMFLOAT3 m_posOffset;
	DirectX::XMFLOAT3 m_lookOffset;
	CObject* m_target;
};