#pragma once
#include "Model.h"

class CPlayer : public Model
{
public:
	CPlayer();
	virtual ~CPlayer();
	void Update() override;
	void Move();
	void Shot();
	void OnCollision(CObject* _obj) override;
private:
	bool m_isShot;	// ショットを撃っているか。移動時の向きに使用
	DirectX::XMFLOAT3 m_prePos;	// １フレーム前の座標。壁との当たり判定、かべずりに使用

	int m_testCollisionNum = 0;	// 弾に当たった回数


public:
	// どこからでもアクセスできちゃいます・・・
	static CPlayer* GetPrimary() { return Player; };
private:
	static CPlayer* Player;
};