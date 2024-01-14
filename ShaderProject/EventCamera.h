#pragma once

#include "CameraBase.h"
#include <map>
#include <list>
#include "EventCallback.h"

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
	void AddCallBack(IEventCallBack* call, int id = 0);
private:
	TEventCameraLerpPos m_posAnchor;
	TEventCameraLerpPos m_targetAnchor;
	std::list<TEventCameraData> m_csv;
	int m_changeFrames;
	float m_progress;
	std::list<std::pair<IEventCallBack*, int>> m_callbacks;
	bool NextAnchor();
};