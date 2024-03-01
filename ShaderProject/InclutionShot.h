#pragma once

#include "ShotObj.h"

/// <summary>
/// ボスのフェーズ４にて大玉から出てくるオレンジの玉
/// </summary>
class CInclusionShot : public CShot
{
public:
	CInclusionShot();
	void Update() override;
};