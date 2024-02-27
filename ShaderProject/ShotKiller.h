#pragma once
#include "Object.h"

class CShotKiller : public CObject
{
public:
	CShotKiller(DirectX::XMFLOAT3 pos);

public:
	void OnCollision(CObject* _obj) override;
};