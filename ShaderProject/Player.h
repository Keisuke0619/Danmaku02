#pragma once
#include "Model.h"

class CPlayer : public Model
{
public:
	CPlayer();
	void Update() override;
	void Move();
	void Shot();
	void OnCollision(CObject* _obj) override;
private:
	bool m_isShot;


public:
	static CPlayer* GetPrimary() { return Player; };
private:
	static CPlayer* Player;
};