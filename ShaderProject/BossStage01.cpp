#include "BossStage01.h"
#include "Easing.h"
CBossStage01::CBossStage01()
{
	Load("Assets/Model/Spot/Spot.fbx");
	m_life = m_MaxLife;
	m_pos = DirectX::XMFLOAT3(0, 1, 270);
	SetMove(DirectX::XMFLOAT2(m_pos.x, m_pos.z), 1);
	m_phase[0] = &CBossStage01::Phase01;
	m_phase[1] = &CBossStage01::Phase02;
	m_phase[2] = &CBossStage01::Phase03;
	m_phase[3] = &CBossStage01::Phase04;
	m_phaseNum = 0;
	UseCollision(true);
	m_colliderScale = 2;
	m_tag = "Enemy";
}
void CBossStage01::Update()
{
	ChangePhase();
	(this->*m_phase[m_phaseNum])();
	Move();
	if (m_life <= 0)
	{
		Destroy();
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

void CBossStage01::ChangePhase()
{
	const int phaseRange = m_MaxLife / m_MaxPhase;

	m_phaseNum = m_MaxPhase - m_life / phaseRange;
}

void CBossStage01::Phase01()
{
	CDebugWindow::Print("Phase1\n");
}

void CBossStage01::Phase02()
{
	CDebugWindow::Print("Phase2\n");
}

void CBossStage01::Phase03()
{
	CDebugWindow::Print("Phase3\n");
}

void CBossStage01::Phase04()
{
	CDebugWindow::Print("Phase4\n");
}
