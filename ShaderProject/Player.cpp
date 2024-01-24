#include "Player.h"
#include "Input.h"
#include "ShotObj.h"
#include "Wall.h"
#include "CameraBase.h"
#include "DebugText.h"
#include "DataPool.h"

#define PLAYER_SHOT_COLLIDER_SCALE (1.0f)

CPlayer* CPlayer::Player;

CPlayer::CPlayer()
{
	Load("Assets/Model/spot/spot.fbx");
	m_colliderScale = 1.0f;
	UseCollision(true);
	m_pos.y = 1.0f;
	m_tag = "Player";
	if (Player == nullptr) Player = this;

	DataPool::AddData("Int_PlayerCollision", &m_testCollisionNum);
}

CPlayer::~CPlayer()
{
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
	m_prePos = m_pos;
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
	
	if (IsKeyPress('U'))
	{
		m_pos.x = 0;
		m_pos.z = 200;
	}


	DebugText::SetData(DebugText::SLOT_PLAYER_POS_X, m_pos.x);
	DebugText::SetData(DebugText::SLOT_PLAYER_POS_Y, m_pos.z);
}

void CPlayer::Shot()
{
	m_isShot = false;
	if (IsKeyPress('Z') == false) return;
	if (m_frame % 10 == 0)
	{
		for (int i = 0; i < 2; i++)
		{
			auto shot = CShot::Create(this, DirectX::XMFLOAT2(m_pos.x, m_pos.z), 18, 270 - DirectX::XMConvertToDegrees(m_rot.y) + (rand() % 11) - 5, YELLOW, SIZE07);
			shot->FromPlayer();
			shot->SetColliderScale(PLAYER_SHOT_COLLIDER_SCALE);
		}
	}
	if (m_frame % 12 == 0)
	{
		for (int i = -5; i <= 5; i++)
		{
			auto shot = CShot::Create(this, DirectX::XMFLOAT2(m_pos.x, m_pos.z), 16, 270 - DirectX::XMConvertToDegrees(m_rot.y) + 3 * i, WHITE, SIZE07);
			shot->FromPlayer();
			shot->SetColliderScale(PLAYER_SHOT_COLLIDER_SCALE);
		}
	}
	m_isShot = true;
}

void CPlayer::OnCollision(CObject* _obj)
{
	if (_obj->GetTagName() == "Wall")
	{
		// �e��ϐ��p��
		auto vtx = _obj->GetBoxVtxVector();		// �ǂ̎l���_�z��
		float minAngle[2] = { 99999, 99999 };	// ���_�ƃv���C���[�x�N�g���̊p�x���������̂Q�Ԃ�ۑ�
		DirectX::XMFLOAT2 collidedVtx[2] = {};	// �p�x�����������_�̃��[�J�����W

		// �ǂ̌��_����v���C���[�܂ł̃x�N�g�����擾���A���K��
		DirectX::XMFLOAT2 fToPlayer = DirectX::XMFLOAT2(_obj->GetPos().x - m_prePos.x, _obj->GetPos().z - m_prePos.z);
		DirectX::XMVECTOR vToPlayer = DirectX::XMLoadFloat2(&fToPlayer);
		vToPlayer = DirectX::XMVector2Normalize(vToPlayer);

		// �ǂ̂S���_�ƃv���C���[�x�N�g���̓��ςɂ��p�x�����߁A���������͕̂ۑ�����B
		for (int i = 0; i < 4; i++)
		{
			// �Y�����_�����o���A���K���B
			DirectX::XMVECTOR vVtx = DirectX::XMLoadFloat2(&(vtx[i]));
			vVtx = DirectX::XMVector2Normalize(vVtx);

			// �v���C���[�x�N�g���Ɠ��ς��Ƃ�B
			float dot;
			DirectX::XMStoreFloat(&dot,DirectX::XMVector2Dot(vVtx, vToPlayer));

			// �P�ʃx�N�g�����m�̓��ό��ʂ�acos�ɓ����Ɗp�x���o�Ă���
			auto angle = abs(acosf(dot));

			// �p�x�����������̂�ۑ�
			if (minAngle[1] > angle)
			{
				minAngle[1] = angle;
				collidedVtx[1] = vtx[i];
			}
			if (minAngle[0] > angle)
			{
				minAngle[1] = minAngle[0];
				minAngle[0] = angle;
				collidedVtx[1] = collidedVtx[0];
				collidedVtx[0] = vtx[i];
			}
		}

		// �p�x���������Q���_���A�x�N�g�����쐬�B���K���B
		DirectX::XMFLOAT2 fWallVector = DirectXUtil::Sub(collidedVtx[0], collidedVtx[1]);
		DirectX::XMVECTOR vWallVector = DirectX::XMLoadFloat2(&fWallVector);
		vWallVector = DirectX::XMVector2Normalize(vWallVector);

		// �v���C���[�̈ړ��x�N�g�����Čv�Z�B���K���͂��Ȃ��B
		auto tmpFloat3 = DirectXUtil::Sub(m_pos, m_prePos);
		DirectX::XMFLOAT2 fMoveVector = DirectX::XMFLOAT2(tmpFloat3.x, tmpFloat3.z);
		DirectX::XMVECTOR vMoveVector = DirectX::XMLoadFloat2(&fMoveVector);

		// �P�ʃx�N�g���ƃx�N�g���̓��ς��Ƃ�ƁA�P�ʃx�N�g���ɂ�����e�̒����ɂȂ�B
		float dot;
		DirectX::XMStoreFloat(&dot, DirectX::XMVector2Dot(vWallVector, vMoveVector));

		// ���ۂ̈ړ��ʂ��v�Z�B
		// Float2��3�����݂��Ă��Ă킩��Â炢���A���K������Ă���߂��ǃx�N�g���Ɂu�e�v�̒����������Ă���B
		DirectX::XMVECTOR vAddVector = DirectX::XMVectorScale(vWallVector, dot);
		DirectX::XMFLOAT2 tmpFloat2;	// ��xFloat2�ɕۑ�
		DirectX::XMStoreFloat2(&tmpFloat2, vAddVector);
		DirectX::XMFLOAT3 fAddVector = DirectX::XMFLOAT3(tmpFloat2.x, 0, tmpFloat2.y);	// ��̌v�Z�̂��߂�Float3�Ɋi�[

		// ���W���X�V����B
		m_pos = DirectXUtil::Add(m_prePos, fAddVector);
		// ���[���h�s����X�V
		ReloadWorldMatrix();
		// �J�����n�s����X�V
		CameraBase::GetPrimary()->Update();
	}

	if (_obj->GetTagName() == "ShotFromEnemy")
	{
		m_testCollisionNum++;
		DebugText::SetData(DebugText::SLOT_PLAYER_COLLIDED, m_testCollisionNum);
	}


}
