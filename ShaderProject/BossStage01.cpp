#include "BossStage01.h"
#include "Util.h"
#include "Defines.h"
#include "Util.h"
#include "DirectXMathUtil.h"
#include "ShotObj.h"
#include "InclutionShot.h"
#include "DataPool.h"
#include "SceneRoot.h"
#include "Score.h"
CBossStage01::CBossStage01()
{
	Load("Assets/Model/Spot/Spot.fbx");
	m_life = m_MaxLife;
	m_pos = DirectX::XMFLOAT3(0, 3, 265);
	m_scale = DirectX::XMFLOAT3(5, 5, 5);
	SetMove(DirectX::XMFLOAT2(m_pos.x, m_pos.z), 1);
	m_phase[0] = &CBossStage01::Phase01;
	m_phase[1] = &CBossStage01::Phase02;
	m_phase[2] = &CBossStage01::Phase03;
	m_phase[3] = &CBossStage01::Phase04;
	m_phaseNum = 0;
	UseCollision(true);
	m_colliderScale = 2;
	m_tag = "Enemy";
	CShot::SetDeleteDepth(80);
}
void CBossStage01::Update()
{
	ChangePhase();
	if (m_phaseNum < m_MaxPhase)
	{
		(this->*m_phase[m_phaseNum])();
	}
	Move();
	if (m_life <= 0)
	{
		Destroy();
		((SceneRoot*)DataPool::GetData("SceneRoot"))->SetNextScene(SceneRoot::SCENE_RESULT, 5.0f);
		CScore::Ins()->AddScore(1000);
	}
}

void CBossStage01::OnCollision(CObject* _obj)
{
	if (_obj->GetTagName() == "ShotFromPlayer")
	{
		m_life--;
	}
}

void CBossStage01::Move()
{
	if (m_moveData.countFrame < m_moveData.targetFrame)
	{
		m_moveData.countFrame++;
		auto nextPos = Util::Lerp((float)m_moveData.countFrame / m_moveData.targetFrame, m_moveData.begin, m_moveData.end, Util::TYPE_INOUT_SINE);
		m_pos.x = nextPos.x;
		m_pos.z = nextPos.y;
	}
}

void CBossStage01::SetMove(DirectX::XMFLOAT2 nextPos, int frames)
{
	m_moveData.begin = DirectX::XMFLOAT2(m_pos.x, m_pos.z);
	m_moveData.end = nextPos;
	m_moveData.countFrame = 0;
	m_moveData.targetFrame = frames;
}

void CBossStage01::RandomMove(int frames)
{
	const DirectX::XMFLOAT2 center = DirectX::XMFLOAT2(0, 265);
	const int radius = 4 * WALL_SCALE;
	float dis = Util::Rand(0, radius);
	float ang = Util::Rand(0, 6.24f);
	auto point = DirectX::XMFLOAT2(cosf(ang) * dis, sinf(ang) * dis);
	DirectXUtil::Increment(&point, center);
	SetMove(point, frames);
}

void CBossStage01::ChangePhase()
{
	const int phaseRange = m_MaxLife / m_MaxPhase;
	auto prePhase = m_phaseNum;
	m_phaseNum = (m_MaxLife - m_life) / phaseRange;
	if (prePhase != m_phaseNum)
	{
		CScore::Ins()->AddScore(1000);
		CScore::Ins()->AddCombo(10);
		m_frame = 0;
	}
}

void CBossStage01::Phase01()
{
	if (m_frame < 180)
	{
		return;
	}
	if (m_frame % 240 == 0)
	{
		RandomMove(180);
	}
	if (m_frame % 12 == 0)
	{
		const int ways = 3;
		const int addAngle = 360 / ways;
		const std::string color[5] =
		{
			"RED03.png",
			"YELLOW03.png",
			"GREEN03.png",
			"BLUE03.png",
			"PURPLE03.png",
		};
		for (int i = 0; i < ways; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				CShot::Create(nullptr, m_pos, 10 + j * 1.2f, m_frame + addAngle * i + j * 2, color[j]);
			}
		}
	}
	if (m_frame % 60 == 0)
	{
		const int ways = 14;
		const int addAngle = 360 / ways;
		for (int i = 0; i < ways; i++)
		{
			CShot::Create(nullptr, m_pos, 14, addAngle * i, "ORANGE02.png");
		}
	}
}

void CBossStage01::Phase02()
{
	static int AddAng = 1;
	if (m_frame % 12 == 0 && m_frame < 180)
	{
		const int ways = 6;
		const int allWay = 360 / ways;
		const float addAngleParFrame = 3.0f;
		const float speed = 12;
		for (int i = 0; i < ways; i++)
		{
			auto shot = CShot::Create(nullptr, m_pos, speed, m_frame / 3 + allWay * i, "YELLOW05.png", 0, -addAngleParFrame * AddAng);
			shot->AddShotData(40, speed, ADDSHOT_NULL, 0, addAngleParFrame * AddAng);
			shot->AddShotData(80, speed, ADDSHOT_NULL, 0, -addAngleParFrame * AddAng);
			shot->AddShotData(120, speed, ADDSHOT_NULL, 0, 0);
		}
	}
	if (m_frame % 3 == 0 && 180 <= m_frame && m_frame <= 210)
	{
		const int ways = 16;
		const int allWay = 360 / ways;
		for (int i = 0; i < ways; i++)
		{
			CShot::Create(nullptr, m_pos, 8, m_frame/3 + allWay * i, "RED02.png");
		}
	}
	if (m_frame == 210)
	{
		RandomMove(60);
	}
	if (m_frame > 300)
	{
		m_frame = 0;
		AddAng *= -1;
	}
}

void CBossStage01::Phase03()
{
	if (m_frame < 180)
	{
		return;
	}
	if (m_frame % 240 == 0)
	{
		RandomMove(180);
	}
	if (m_frame % 12 == 0)
	{
		const int ways = 4;
		const int addAngle = 360 / ways;
		const std::string color[5] =
		{
			"RED03.png",
			"YELLOW03.png",
			"GREEN03.png",
			"BLUE03.png",
			"PURPLE03.png",
		};
		for (int i = 0; i < ways; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				CShot::Create(nullptr, m_pos, 10 + j * 1.2f, -m_frame * 2 + addAngle * i + j, color[j]);
			}
		}
	}
	if (m_frame % 45 == 0)
	{
		const int ways = 14;
		const int addAngle = 360 / ways;
		for (int i = 0; i < ways; i++)
		{
			CShot::Create(nullptr, m_pos, 12, addAngle * i, "ORANGE02.png");
		}
	}
}

void CBossStage01::Phase04()
{
	static int i = 0;
	if (m_frame <= 60 && m_frame % 12 == 0)
	{
		auto shot = new CInclusionShot();
		CShot::SetInit(shot, nullptr, DirectX::XMFLOAT2(m_pos.x, m_pos.z), 10.0f, (360 / 5) * i++, "AQUA03.png");
	}
	if (m_frame == 210)
	{
		const int ways = 16;
		const int allWay = 360 / ways;
		const int lines = 5;
		for (int i = 0; i < ways; i++)
		{
			for (int j = 0; j < lines; j++)
			{
				CShot::Create(nullptr, m_pos, 8 + j, j*2 + allWay * i, "RED02.png");
			}
		}
		RandomMove(60);
	}
	if (m_frame > 300)
	{
		m_frame = 0;
	}

}
