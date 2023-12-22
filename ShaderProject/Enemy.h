#pragma once
#include "Model.h"
#include <list>
struct TEnemyShotData
{
	float speed;		// �e��
	int ways;			// ��
	float startAngle;	// �^�񒆂̋ʂ̊p�x
	float wayAngle;		// ���v�����e�̊Ԃ̊p�x
	float frameAngle;	// �t���[���ɂ��̊p�x�������Ĕ���
	int waitFrame;		// �܂��t���[��
	std::string colorID;// �F
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