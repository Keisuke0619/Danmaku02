#pragma once

#include <SceneBase.hpp>
#include <Sprite.h>
#include <functional>

#define RANKING_NUM (64)
struct TRankData
{
	char name[12] = "A";
	int score = 100;
};

class CSceneResult : public SceneBase
{
public:
	static int m_collisionCount;

	void Init() override;
	void Uninit() override;
	void Update(float tick) override;
	void Draw() override;
	void UpdatePhaseInput();
	void UpdatePhaseWaitExit();
private:
	Texture* m_banner[2];
	Texture* m_drawTex;
	TRankData m_rankData[RANKING_NUM];
	int m_cursor = 0;
	int m_rank = -1;
	int m_score = 0;
	float m_posY = 0;
	void (CSceneResult::* m_updateFunc)();
};
