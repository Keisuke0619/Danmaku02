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
	// 仕様上、座標のセットができないので、１フレームで指定座標まで移動するように設定。
	SetMove(DirectX::XMFLOAT2(m_pos.x, m_pos.z), 1);

	// フェーズ関数ポインタを格納。
	m_phase[0] = &CBossStage01::Phase01;
	m_phase[1] = &CBossStage01::Phase02;
	m_phase[2] = &CBossStage01::Phase03;
	m_phase[3] = &CBossStage01::Phase04;
	// 現在のフェーズを０に指定
	m_phaseNum = 0;

	UseCollision(true);
	m_colliderScale = 2;
	m_tag = "Enemy";

	// カメラからの距離で弾を自動削除している。その距離を８０ｍに変更。（ボス戦では引きのカメラになるため）
	CShot::SetDeleteDepth(80);
}
void CBossStage01::Update()
{
	// 体力を参照し、フェーズをチェンジ。
	ChangePhase();
	// 最大フェーズ数以下だったらフェーズ関数を実行
	if (m_phaseNum < m_MaxPhase)
	{
		(this->*m_phase[m_phaseNum])();
	}
	Move();

	// 撃破処理
	if (m_life <= 0)
	{
		Destroy();
		((SceneRoot*)DataPool::GetData("SceneRoot"))->SetNextScene(SceneRoot::SCENE_RESULT, 5.0f);	// ５秒後リザルトシーンに。
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
	// 実際に移動させる。補間で滑らかに動かす。
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
	// 指定フレームかけて指定場所へ移動させるデータを作成。
	m_moveData.begin = DirectX::XMFLOAT2(m_pos.x, m_pos.z);
	m_moveData.end = nextPos;
	m_moveData.countFrame = 0;
	m_moveData.targetFrame = frames;
}

void CBossStage01::RandomMove(int frames)
{
	// 指定フレームをかけてランダム座標に移動。
	// ランダム角度ｘランダム距離の場所に移動。
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
	// 体力の割合でフェーズを変更。
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
	// 最初のムービーの時間は待つ。
	if (m_frame < 180)
	{
		return;
	}
	// 240フレームごとに移動
	if (m_frame % 240 == 0)
	{
		RandomMove(180);
	}
	// 12フレームごとに３方向に球を出す。ちょっとずつ回転。
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
	// 60フレームごとに全方位に球を出す。
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
	// うねる弾幕の角速度の加算の向き。正の数か負の数か。
	static int AddAng = 1;
	// 黄色いうねる弾幕を放つ。
	if (m_frame % 12 == 0 && m_frame < 180)
	{
		const int ways = 6;
		const int allWay = 360 / ways;
		const float addAngleParFrame = 3.0f;
		const float speed = 12;
		for (int i = 0; i < ways; i++)
		{
			auto shot = CShot::Create(nullptr, m_pos, speed, m_frame / 3 + allWay * i, "YELLOW05.png", 0, -addAngleParFrame * AddAng);
			shot->AddShotData(40, speed, ADDSHOT_NULL, 0, addAngleParFrame * AddAng);	// 最後の引数が角速度。ここに静的ローカル変数を使用。
			shot->AddShotData(80, speed, ADDSHOT_NULL, 0, -addAngleParFrame * AddAng);
			shot->AddShotData(120, speed, ADDSHOT_NULL, 0, 0);
		}
	}
	// 移動直前に出す赤い全方位
	if (m_frame % 3 == 0 && 180 <= m_frame && m_frame <= 210)
	{
		const int ways = 16;
		const int allWay = 360 / ways;
		for (int i = 0; i < ways; i++)
		{
			CShot::Create(nullptr, m_pos, 8, m_frame/3 + allWay * i, "RED02.png");
		}
	}
	// 移動
	if (m_frame == 210)
	{
		RandomMove(60);
	}
	// フェーズの先頭まで戻る。うねる方向を逆にする。
	if (m_frame > 300)
	{
		m_frame = 0;
		AddAng *= -1;
	}
}

void CBossStage01::Phase03()
{
	// ちょっと待つ。
	if (m_frame < 180)
	{
		return;
	}
	// 240フレームごとにランダム移動
	if (m_frame % 240 == 0)
	{
		RandomMove(180);
	}
	// ４方向に増えた。ちょっとずつ回る。
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
	// 全方位。
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
