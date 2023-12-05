#pragma once
#include "CameraBase.h"
#include <Windows.h>

class CameraWipe : public CameraBase
{
public:
	CameraWipe();
	~CameraWipe();
	void Update() final;
private:
};