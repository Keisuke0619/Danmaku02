#pragma once
#include "Model.h"
#include <list>
struct TEnemyShotData
{
	float speed;		// 弾速
	int ways;			// 数
	float startAngle;	// 真ん中の玉の角度
	float wayAngle;		// ｎＷａｙ弾の間の角度
	float frameAngle;	// フレームにこの角度をかけて発射
	int waitFrame;		// まちフレーム
	std::string colorID;// 色
};

struct TEnemyMoveData
{
	int frame;
	float posXfromSpawn;
	float posYfromSpawn;
	int toFrame;
};
class CEnemy : public Model
{
public:
	CEnemy();
	void Update();

	void OnCollision(CObject* _obj) override;
protected:
	float GetAngleToPlayer();
	int m_life;

private:
	std::list<TEnemyShotData> m_shotDatas;
	std::list<TEnemyMoveData> m_moveDatas;
	void CSVShot(TEnemyShotData data);
};