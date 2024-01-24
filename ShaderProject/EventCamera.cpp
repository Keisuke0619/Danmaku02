#include "EventCamera.h"
#include "CsvLoader.h"
#include "Util.h"

#pragma warning (disable : 4244)
CEventCamera::CEventCamera(std::string path)
	:CameraBase()
{
	auto csv = CSVLoad(path, true);
	for (auto line : csv)
	{
		TEventCameraData data;
		data.pos = DirectX::XMFLOAT3(atof(line[0].c_str()), atof(line[1].c_str()), atof(line[2].c_str()));
		data.look = DirectX::XMFLOAT3(atof(line[3].c_str()), atof(line[4].c_str()), atof(line[5].c_str()));
		data.frames = atoi(line[6].c_str());
		m_csv.push_back(data);
	}
	auto data = m_csv.front();
	m_csv.pop_front();
	m_pos = data.pos;
	m_look = data.look;
	NextAnchor();
}

void CEventCamera::Update()
{
	m_pos = Util::Lerp(m_progress, m_posAnchor.begin, m_posAnchor.end, Util::EEaseType::TYPE_INOUT_SINE);
	m_look = Util::Lerp(m_progress, m_targetAnchor.begin, m_targetAnchor.end, Util::EEaseType::TYPE_INOUT_SINE);
	m_progress += 1.0f / m_changeFrames;
	if (m_progress >= 1.0f)
	{
		auto isEnd = NextAnchor();
		if (isEnd)
		{
			Destroy();
			for (auto call : m_callbacks)
			{
				call.first->CallBack(call.second);
			}
		}
	}
}

void CEventCamera::AddCallBack(IEventCallBack* call, int id)
{
	std::pair<IEventCallBack*, int> data;
	data.first = call;
	data.second = id;
	m_callbacks.push_back(data);
}

bool CEventCamera::NextAnchor()
{
	if (m_csv.empty())
	{
		return true;
	}
	m_posAnchor.begin = m_pos;
	m_targetAnchor.begin = m_look;
	auto data = m_csv.front();
	m_csv.pop_front();
	m_posAnchor.end = data.pos;
	m_targetAnchor.end = data.look;
	m_changeFrames = data.frames;

	m_progress = 0;
	return false;
}
