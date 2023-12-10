#include "Player.h"
#include "Input.h"
#include "ShotObj.h"

#define PLAYER_SHOT_COLLIDER_SCALE (1.0f)

CPlayer* CPlayer::Player;

CPlayer::CPlayer()
{
	Load("Assets/Model/spot/spot.fbx");
	m_colliderScale = 0.2f;
	UseCollision(true);
	m_pos.y = 1.0f;
	m_tag = "Player";
	if (Player == nullptr) Player = this;
}

void CPlayer::Update()
{
	Shot();
	Move();
}

/// <summary>
/// �ړ�
/// </summary>
void CPlayer::Move()
{
	// �ړ��X�e�[�^�X
	int m_state = 0;
	// �ړ��x�N�g��
	DirectX::XMFLOAT3 addPos = {};
	// �L�[�������ꂽ��Ή������Ɉړ����X�e�[�^�X���u�ړ��v�ɕύX
	if (IsKeyPress(VK_UP)) { addPos.z += 1; m_state = 1; }
	if (IsKeyPress(VK_DOWN)) { addPos.z -= 1; m_state = 1; }
	if (IsKeyPress(VK_LEFT)) { addPos.x -= 1; m_state = 1; }
	if (IsKeyPress(VK_RIGHT)) { addPos.x += 1; m_state = 1; }
	// ��2���@���ł��Ȃ��悤�ɐ��K��
	auto addVec = DirectX::XMLoadFloat3(&addPos);
	addVec = DirectX::XMVector3Normalize(addVec);
	// �ړ����x��������Float3�ɖ߂�
	addVec = DirectX::XMVectorScale(addVec, 0.1f);
	DirectX::XMStoreFloat3(&addPos, addVec);
	// �����ړ����Ă��違�V�t�g��������Ă���Ȃ�ᑬ�ړ��ɁB���K�������x�N�g���ɒ萔��������B
	if (m_state == 1 && IsKeyPress(VK_SHIFT))
	{
		const float dashCoef = 0.7f;
		addPos.x *= dashCoef;
		addPos.y *= dashCoef;
		addPos.z *= dashCoef;
		m_state = 2;
	}
	// ���ۂɈړ�������
	m_pos.x += addPos.x;
	m_pos.y += addPos.y;
	m_pos.z += addPos.z;
	// �����ړ����Ă���Ȃ������ς���
	if (m_state != 0 && m_isShot == false)
	{
		m_rot.y = atan2f(-addPos.x, -addPos.z);
	}
	
}

void CPlayer::Shot()
{
	m_isShot = false;
	if (IsKeyPress('Z') == false) return;
	if (m_frame % 10 == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			auto shot = CShot::Create(this, DirectX::XMFLOAT2(m_pos.x, m_pos.z), 14, 270 - DirectX::XMConvertToDegrees(m_rot.y) + (rand() % 11) - 5, YELLOW, SIZE07);
			shot->FromPlayer();
			shot->SetColliderScale(PLAYER_SHOT_COLLIDER_SCALE);
		}
	}
	if (m_frame % 12 == 0)
	{
		for (int i = -3; i <= 3; i++)
		{
			auto shot = CShot::Create(this, DirectX::XMFLOAT2(m_pos.x, m_pos.z), 12, 270 - DirectX::XMConvertToDegrees(m_rot.y) + 10 * i, WHITE, SIZE07);
			shot->FromPlayer();
			shot->SetColliderScale(PLAYER_SHOT_COLLIDER_SCALE);
		}
	}
	m_isShot = true;
}

void CPlayer::OnCollision(CObject* _obj)
{
	if (_obj->GetTagName() == "ShotFromEnemy")
	{
		CDebugWindow::Print("��������", m_pos);
	}
}
