#include "Player.h"
#include "Input.h"
CPlayer::CPlayer()
{
	Load("Assets/Model/spot/spot.fbx");
	m_colliderScale = 0.2f;
	UseCollision(true);
}

void CPlayer::Update()
{
	Move();
}

/// <summary>
/// 移動
/// </summary>
void CPlayer::Move()
{
	// 移動ステータス
	int m_state = 0;
	// 移動ベクトル
	DirectX::XMFLOAT3 addPos = {};
	// キーが押されたら対応方向に移動＆ステータスを「移動」に変更
	if (IsKeyPress(VK_UP)) { addPos.z += 1; m_state = 1; }
	if (IsKeyPress(VK_DOWN)) { addPos.z -= 1; m_state = 1; }
	if (IsKeyPress(VK_LEFT)) { addPos.x -= 1; m_state = 1; }
	if (IsKeyPress(VK_RIGHT)) { addPos.x += 1; m_state = 1; }
	// √2走法ができないように正規化
	auto addVec = DirectX::XMLoadFloat3(&addPos);
	addVec = DirectX::XMVector3Normalize(addVec);
	// 移動速度をかけてFloat3に戻す
	addVec = DirectX::XMVectorScale(addVec, 0.18f);
	DirectX::XMStoreFloat3(&addPos, addVec);
	// もし移動している＆シフトが押されているなら低速移動に。正規化したベクトルに定数をかける。
	if (m_state == 1 && IsKeyPress(VK_SHIFT))
	{
		const float dashCoef = 0.7f;
		addPos.x *= dashCoef;
		addPos.y *= dashCoef;
		addPos.z *= dashCoef;
		m_state = 2;
	}
	// 実際に移動させる
	m_pos.x += addPos.x;
	m_pos.y += addPos.y;
	m_pos.z += addPos.z;
	// もし移動しているなら向きを変える
	if (m_state != 0)
	{
		m_rot.y = atan2f(-addPos.x, -addPos.z);
	}
	
}

void CPlayer::OnCollision(CObject* _obj)
{
	CDebugWindow::Print("あたった", m_pos);
}
