#pragma once
#include "Model.h"

class CEnemy : public Model
{
public:
	CEnemy();
	void Update();

	void OnCollision(CObject* _obj) override;
protected:
	float GetAngleToPlayer();
	int m_life;

};