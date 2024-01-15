#pragma once
#include <Model.h>
class CBossStage01 : Model
{
	struct TLerpData
	{
		DirectX::XMFLOAT2 begin;
		DirectX::XMFLOAT2 end;
		int targetFrame;
		int countFrame;
	};
public:
	CBossStage01();
	void Update() override;

	void OnCollision(CObject* _obj) override;
private:
	void Move();
	void SetMove(DirectX::XMFLOAT2 nextPos, int frames);

	void ChangePhase();
	void Phase01();
	void Phase02();
	void Phase03();
	void Phase04();

	const int m_MaxLife = 1000;
	const int m_MaxPhase = 4;
	int m_life;
	int m_phaseNum;
	void (CBossStage01::*m_phase[4])(void);
	TLerpData m_moveData;
};