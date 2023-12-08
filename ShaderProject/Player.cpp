#include "Player.h"
#include "Input.h"
CPlayer::CPlayer()
{
	Load("Assets/Model/spot/spot.fbx");
}

void CPlayer::Update()
{
	Move();
}

void CPlayer::Move()
{
	int m_state = 0;
	DirectX::XMFLOAT3 addPos = {};
	if (IsKeyPress(VK_UP)) { addPos.z += 1; m_state = 1; }
	if (IsKeyPress(VK_DOWN)) { addPos.z -= 1; m_state = 1; }
	if (IsKeyPress(VK_LEFT)) { addPos.x -= 1; m_state = 1; }
	if (IsKeyPress(VK_RIGHT)) { addPos.x += 1; m_state = 1; }
	auto addVec = DirectX::XMLoadFloat3(&addPos);
	addVec = DirectX::XMVector3Normalize(addVec);
	addVec = DirectX::XMVectorScale(addVec, 0.18f);
	DirectX::XMStoreFloat3(&addPos, addVec);
	if (m_state == 1 && IsKeyPress(VK_SHIFT))
	{
		const float dashCoef = 0.7f;
		addPos.x *= dashCoef;
		addPos.y *= dashCoef;
		addPos.z *= dashCoef;
		m_state = 2;
	}

	m_pos.x += addPos.x;
	m_pos.y += addPos.y;
	m_pos.z += addPos.z;
	if (m_state != 0)
	{
		m_rot.y = atan2f(addPos.x, addPos.z);
	}
	
}
