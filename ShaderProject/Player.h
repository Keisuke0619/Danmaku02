#pragma once
#include "Model.h"

class CPlayer : public Model
{
public:
	CPlayer();
	void Update() override;
	void Move();
};