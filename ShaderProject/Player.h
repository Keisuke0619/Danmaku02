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
	bool m_isShot;	// �V���b�g�������Ă��邩�B�ړ����̌����Ɏg�p
	DirectX::XMFLOAT3 m_prePos;	// �P�t���[���O�̍��W�B�ǂƂ̓����蔻��A���ׂ���Ɏg�p

	int m_testCollisionNum = 0;	// �e�ɓ���������


public:
	// �ǂ�����ł��A�N�Z�X�ł����Ⴂ�܂��E�E�E
	static CPlayer* GetPrimary() { return Player; };
private:
	static CPlayer* Player;
};