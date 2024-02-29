#include "Player.h"
#include "Input.h"
#include "ShotObj.h"
#include "Wall.h"
#include "CameraBase.h"
#include "DebugText.h"
#include "DataPool.h"
#include "Effect.h"
#include "SoundUtil.h"
#include "ShotKiller.h"
#include "SceneResult.h"
#include "Score.h"
#define PLAYER_SHOT_COLLIDER_SCALE (1.0f)

CPlayer* CPlayer::Player;

CPlayer::CPlayer()
{
	Load("Assets/Model/spot/spot.fbx");
	m_colliderScale = 1.0f;
	UseCollision(true);
	m_pos.y = 1.0f;
	m_tag = "Player";
	Player = this;

	// ブラックボードに値をセット。
	DataPool::AddData("Int_PlayerCollision", &m_testCollisionNum);
}

CPlayer::~CPlayer()
{
	Player = nullptr;
}

void CPlayer::Update()
{
	
	Shot();
	Move();
}

/// <summary>
/// 移動
/// </summary>
void CPlayer::Move()
{
	m_prePos = m_pos;	// 移動する前の情報を保存。
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
	addVec = DirectX::XMVectorScale(addVec, 0.2f);
	DirectX::XMStoreFloat3(&addPos, addVec);
	// もし移動している＆シフトが押されているなら低速移動に。正規化したベクトルに定数をかける。
	if (m_state == 1 && IsKeyPress(VK_SHIFT))
	{
		const float dashCoef = 0.6f;
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
	if (m_state != 0 && m_isShot == false)
	{
		m_rot.y = atan2f(-addPos.x, -addPos.z);
	}
	
	// デバッグ。ボス前までワープ
	if (IsKeyPress('U'))
	{
		m_pos.x = 0;
		m_pos.z = 200;
	}

	// デバッグ表示用。座標をそれぞれのスロットに設定。
	DebugText::SetData(DebugText::SLOT_PLAYER_POS_X, m_pos.x);
	DebugText::SetData(DebugText::SLOT_PLAYER_POS_Y, m_pos.z);
}

void CPlayer::Shot()
{
	// ショットキーが押されていなかったら「ショットしてない状態」にして終了
	m_isShot = false;
	if (IsKeyPress('Z') == false) return;

	// 規定フレームごとに自弾を生成
	if (m_frame % 10 == 0)
	{
		for (int i = 0; i < 2; i++)
		{
			// 弾を生成し、プレイヤーからのものだと指定。その後サイズを設定。
			auto shot = CShot::Create(this, DirectX::XMFLOAT2(m_pos.x, m_pos.z), 18, 270 - DirectX::XMConvertToDegrees(m_rot.y) + (rand() % 11) - 5, YELLOW, SIZE07);
			shot->FromPlayer();
			shot->SetColliderScale(PLAYER_SHOT_COLLIDER_SCALE);
		}

	}
	if (m_frame % 12 == 0)
	{
		for (int i = -5; i <= 5; i++)
		{
			// 弾を生成し、プレイヤーからのものだと指定。その後サイズを設定。
			auto shot = CShot::Create(this, DirectX::XMFLOAT2(m_pos.x, m_pos.z), 16, 270 - DirectX::XMConvertToDegrees(m_rot.y) + 3 * i, WHITE, SIZE07);
			shot->FromPlayer();
			shot->SetColliderScale(PLAYER_SHOT_COLLIDER_SCALE);
		}
		Sound::Play("Shot.wav");	// ついでに音も鳴らす
	}
	// 「ショットしている状態」にして抜ける。
	m_isShot = true;
}

void CPlayer::OnCollision(CObject* _obj)
{
	// 壁との当たり判定。かべずりずり～。
	if (_obj->GetTagName() == "Wall")
	{
		// 各種変数用意
		auto vtx = _obj->GetBoxVtxVector();		// 壁の四頂点配列
		float minAngle[2] = { 99999, 99999 };	// 原点とプレイヤーベクトルの角度を小さいの２つぶん保存
		DirectX::XMFLOAT2 collidedVtx[2] = {};	// 角度が小さい頂点のローカル座標

		// 壁の原点からプレイヤーまでのベクトルを取得し、正規化
		DirectX::XMFLOAT2 fToPlayer = DirectX::XMFLOAT2(_obj->GetPos().x - m_prePos.x, _obj->GetPos().z - m_prePos.z);
		DirectX::XMVECTOR vToPlayer = DirectX::XMLoadFloat2(&fToPlayer);
		vToPlayer = DirectX::XMVector2Normalize(vToPlayer);

		// 壁の４頂点とプレイヤーベクトルの内積により角度を求め、小さいものは保存する。
		for (int i = 0; i < 4; i++)
		{
			// 該当頂点を取り出し、正規化。
			DirectX::XMVECTOR vVtx = DirectX::XMLoadFloat2(&(vtx[i]));
			vVtx = DirectX::XMVector2Normalize(vVtx);

			// プレイヤーベクトルと内積をとる。
			float dot;
			DirectX::XMStoreFloat(&dot,DirectX::XMVector2Dot(vVtx, vToPlayer));

			// 単位ベクトル同士の内積結果をacosに入れると角度が出てくる
			auto angle = abs(acosf(dot));

			// 角度が小さいものを保存
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

		// 角度が小さい２頂点より、ベクトルを作成。正規化。
		DirectX::XMFLOAT2 fWallVector = DirectXUtil::Sub(collidedVtx[0], collidedVtx[1]);
		DirectX::XMVECTOR vWallVector = DirectX::XMLoadFloat2(&fWallVector);
		vWallVector = DirectX::XMVector2Normalize(vWallVector);

		// プレイヤーの移動ベクトルを再計算。正規化はしない。
		auto tmpFloat3 = DirectXUtil::Sub(m_pos, m_prePos);
		DirectX::XMFLOAT2 fMoveVector = DirectX::XMFLOAT2(tmpFloat3.x, tmpFloat3.z);
		DirectX::XMVECTOR vMoveVector = DirectX::XMLoadFloat2(&fMoveVector);

		// 単位ベクトルとベクトルの内積をとると、単位ベクトルにおける影の長さになる。
		float dot;
		DirectX::XMStoreFloat(&dot, DirectX::XMVector2Dot(vWallVector, vMoveVector));

		// 実際の移動量を計算。
		// Float2と3が混在していてわかりづらいが、正規化されている近い壁ベクトルに「影」の長さをかけている。
		DirectX::XMVECTOR vAddVector = DirectX::XMVectorScale(vWallVector, dot);
		DirectX::XMFLOAT2 tmpFloat2;	// 一度Float2に保存
		DirectX::XMStoreFloat2(&tmpFloat2, vAddVector);
		DirectX::XMFLOAT3 fAddVector = DirectX::XMFLOAT3(tmpFloat2.x, 0, tmpFloat2.y);	// 後の計算のためにFloat3に格納

		// 座標を更新する。
		m_pos = DirectXUtil::Add(m_prePos, fAddVector);
		// ワールド行列も更新
		ReloadWorldMatrix();
		// カメラ系行列も更新
		CameraBase::GetPrimary()->Update();
	}

	// 敵弾との当たり判定。
	if (_obj->GetTagName() == "ShotFromEnemy")
	{
		// 当たり判定変数を増やし、デバッグデータを該当スロットにセット。
		m_testCollisionNum++;
		DebugText::SetData(DebugText::SLOT_PLAYER_COLLIDED, m_testCollisionNum);

		// リザルトシーンのpublic static変数に直接書き込み・・・　当たった回数を表示させるため。
		CSceneResult::m_collisionCount = m_testCollisionNum;
		// コンボをリセット
		CScore::Ins()->ResetCombo();
	}


}
