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
	DirectX::XMFLOAT2 posFromSpawn;
	int toFrame;
};
class CEnemy : public Model
{
public:
	CEnemy();
	void Update();

	void SetSpawnData(const char* modelPath, int autoDestroyFrame, int m_hp);
	void SetOriginPos(DirectX::XMFLOAT2 pos, float height = 0.0f);
	void PushData(TEnemyMoveData data);
	void PushData(TEnemyShotData data);
	void OnCollision(CObject* _obj) override;
protected:
	float GetAngleToPlayer();
	int m_life;

private:
	float m_speed = 0;
	DirectX::XMFLOAT2 m_spawnPos;
	DirectX::XMFLOAT2 m_moveVector;
	int m_destroyFrame = 999;
	std::list<TEnemyShotData> m_shotDatas;
	std::list<TEnemyMoveData> m_moveDatas;
	void CSVRoutine();
	void CSVShot(TEnemyShotData data);
	bool CSVMove(TEnemyMoveData data);
};