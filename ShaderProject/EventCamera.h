#pragma once

#include "CameraBase.h"
#include <memory>
struct TEventCameraLerpPos
{
	DirectX::XMFLOAT3 begin;
	DirectX::XMFLOAT3 end;
};
struct TEventCameraData
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 look;
	int frames;
};

class CEventCamera : CameraBase
{
public:
	CEventCamera(std::string path);

	void Update() override;
	//void SetFinishCallBack(void(T::*callback)());
private:
	//void (T::*m_callback)();
	TEventCameraLerpPos m_posAnchor;
	TEventCameraLerpPos m_targetAnchor;
	std::list<TEventCameraData> m_csv;
	int m_changeFrames;
	float m_progress;

	bool NextAnchor();
};