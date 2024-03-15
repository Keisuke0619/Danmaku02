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
	Load("Assets/Model/blackSpot/spot.fbx");
	m_life = m_MaxLife;
	m_pos = DirectX::XMFLOAT3(0, 3, 265);
	m_scale = DirectX::XMFLOAT3(5, 5, 5);
	// �d�l��A���W�̃Z�b�g���ł��Ȃ��̂ŁA�P�t���[���Ŏw����W�܂ňړ�����悤�ɐݒ�B
	SetMove(DirectX::XMFLOAT2(m_pos.x, m_pos.z), 1);

	// �t�F�[�Y�֐��|�C���^���i�[�B
	m_phase[0] = &CBossStage01::Phase01;
	m_phase[1] = &CBossStage01::Phase02;
	m_phase[2] = &CBossStage01::Phase03;
	m_phase[3] = &CBossStage01::Phase04;
	// ���݂̃t�F�[�Y���O�Ɏw��
	m_phaseNum = 0;

	UseCollision(true);
	m_colliderScale = 2;
	m_tag = "Enemy";

	// �J��������̋����Œe�������폜���Ă���B���̋������W�O���ɕύX�B�i�{�X��ł͈����̃J�����ɂȂ邽�߁j
	CShot::SetDeleteDepth(80);
}
void CBossStage01::Update()
{
	// �̗͂��Q�Ƃ��A�t�F�[�Y���`�F���W�B
	ChangePhase();
	// �ő�t�F�[�Y���ȉ���������t�F�[�Y�֐������s
	if (m_phaseNum < m_MaxPhase)
	{
		(this->*m_phase[m_phaseNum])();
	}
	Move();

	// ���j����
	if (m_life <= 0)
	{
		Destroy();
		((SceneRoot*)DataPool::GetData("SceneRoot"))->SetNextScene(SceneRoot::SCENE_RESULT, 5.0f);	// �T�b�ナ�U���g�V�[���ɁB
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
	// ���ۂɈړ�������B��ԂŊ��炩�ɓ������B
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
	// �w��t���[�������Ďw��ꏊ�ֈړ�������f�[�^���쐬�B
	m_moveData.begin = DirectX::XMFLOAT2(m_pos.x, m_pos.z);
	m_moveData.end = nextPos;
	m_moveData.countFrame = 0;
	m_moveData.targetFrame = frames;
}

void CBossStage01::RandomMove(int frames)
{
	// �w��t���[���������ă����_�����W�Ɉړ��B
	// �����_���p�x�������_�������̏ꏊ�Ɉړ��B
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
	// �̗͂̊����Ńt�F�[�Y��ύX�B
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
	// �ŏ��̃��[�r�[�̎��Ԃ͑҂B
	if (m_frame < 180)
	{
		return;
	}
	// 240�t���[�����ƂɈړ�
	if (m_frame % 240 == 0)
	{
		RandomMove(180);
	}
	// 12�t���[�����ƂɂR�����ɋ����o���B������Ƃ���]�B
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
	// 60�t���[�����ƂɑS���ʂɋ����o���B
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
	// ���˂�e���̊p���x�̉��Z�̌����B���̐������̐����B
	static int AddAng = 1;
	// ���F�����˂�e������B
	if (m_frame % 12 == 0 && m_frame < 180)
	{
		const int ways = 6;
		const int allWay = 360 / ways;
		const float addAngleParFrame = 3.0f;
		const float speed = 12;
		for (int i = 0; i < ways; i++)
		{
			auto shot = CShot::Create(nullptr, m_pos, speed, m_frame / 3 + allWay * i, "YELLOW05.png", 0, -addAngleParFrame * AddAng);
			shot->AddShotData(40, speed, ADDSHOT_NULL, 0, addAngleParFrame * AddAng);	// �Ō�̈������p���x�B�����ɐÓI���[�J���ϐ����g�p�B
			shot->AddShotData(80, speed, ADDSHOT_NULL, 0, -addAngleParFrame * AddAng);
			shot->AddShotData(120, speed, ADDSHOT_NULL, 0, 0);
		}
	}
	// �ړ����O�ɏo���Ԃ��S����
	if (m_frame % 3 == 0 && 180 <= m_frame && m_frame <= 210)
	{
		const int ways = 16;
		const int allWay = 360 / ways;
		for (int i = 0; i < ways; i++)
		{
			CShot::Create(nullptr, m_pos, 8, m_frame/3 + allWay * i, "RED02.png");
		}
	}
	// �ړ�
	if (m_frame == 210)
	{
		RandomMove(60);
	}
	// �t�F�[�Y�̐擪�܂Ŗ߂�B���˂�������t�ɂ���B
	if (m_frame > 300)
	{
		m_frame = 0;
		AddAng *= -1;
	}
}

void CBossStage01::Phase03()
{
	// ������Ƒ҂B
	if (m_frame < 180)
	{
		return;
	}
	// 240�t���[�����ƂɃ����_���ړ�
	if (m_frame % 240 == 0)
	{
		RandomMove(180);
	}
	// �S�����ɑ������B������Ƃ����B
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
	// �S���ʁB
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
	static int WayNum = 0;
	if (m_frame <= 60 && m_frame % 12 == 0)
	{
		auto shot = new CInclusionShot();
		CShot::SetInit(shot, nullptr, DirectX::XMFLOAT2(m_pos.x, m_pos.z), 10.0f, (360 / 5) * WayNum++, "AQUA03.png");
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
