#include "Torch.h"
#include "Defines.h"
#include "CsvLoader.h"
#include "Pipeline.h"
#include <list>
#include "DirectXMathUtil.h"
#include "CameraBase.h"
void CTorch::SetTorch(std::string csvPath)
{
	auto csv = CSVLoad(csvPath, true);
	std::list<std::vector<std::string>> sortList;
	for (auto line : csv)
	{
		sortList.push_back(line);
	}
	sortList.sort(
		[](std::vector<std::string> l, std::vector<std::string> r)->bool
		{
			return l[1] < r[1];
		}
	);
	for (auto line : csv)
	{
		auto obj = new CTorch(
			std::atoi(line[0].c_str()), 
			std::atoi(line[1].c_str()), 
			std::atoi(line[2].c_str()),
			DirectX::XMFLOAT3(
				std::atof(line[3].c_str()),
				std::atof(line[4].c_str()),
				std::atof(line[5].c_str())
			),
			std::atof(line[6].c_str())
		);
	}
}
CTorch::CTorch(int x, int y, int direction, DirectX::XMFLOAT3 col, float range)
{
	const DirectX::XMFLOAT3 constOffset = {-1.6f, 3.7f, 1.6f};
	Load("Assets/Model/Torch/Torch.fbx", WALL_SCALE);
	m_tag = "Torch";
	m_pos = DirectX::XMFLOAT3(x * WALL_SCALE, 0, y * WALL_SCALE);
	m_isStaticPosition = true;
	m_rot.y = 3.141592f * 0.5f * direction;
	auto offset = constOffset;
	offset.x *= cosf(m_rot.y);
	offset.z *= sinf(m_rot.y);
	m_lightPos = DirectXUtil::Add(m_pos, offset);
	PipelinePushPointLight(m_lightPos, col, range);
	ReloadWorldMatrix();
		m_efk = Efk::Play(u"Assets/Effect/Torch.efkefc", m_lightPos.x, m_lightPos.y, m_lightPos.z);

}

void CTorch::Update()
{
	//if ((m_pos.z < CameraBase::GetPrimary()->GetPos().z || CameraBase::GetPrimary()->GetPos().z < m_pos.z - 100) && m_efk != -1)
	//{
	//	m_efk = Efk::Stop(m_efk);
	//}
	//else if (m_efk == -1)
	//{
	//	m_efk = Efk::Play(u"Assets/Effect/Torch.efkefc", m_lightPos.x, m_lightPos.y, m_lightPos.z);
	//}
}
