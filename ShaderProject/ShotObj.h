#pragma once
#include "Billborad.h"
#include <list>
#include <queue>
#include "MeshBuffer.h"

#define SHOT_SPEED_COEF (1.0f / 60)
#define ADDSHOT_NULL (1129894.0f)

struct TShotData
{
	float speed;
	float angle;
	float addSpeed;
	float addAngle;
};
class TShotDataReserve
{
public:
	int frame;
	TShotData data;
};
enum EShotColor
{
	AQUA,
	BLUE,
	GREEN,
	ORANGE,
	PURPLE,
	RED,
	WHITE,
	YELLOW,
	COLOR_MAX
};
enum EShotSize
{
	SIZE01,
	SIZE02,
	SIZE03,
	SIZE04,
	SIZE05,
	SIZE06,
	SIZE07,
	SIZE08,
	SHOT_SIZE_MAX
};

struct TShotGeometry
{
	DirectX::XMFLOAT4 vtx;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT2 uvSize;
};

class CShot : public CBillBoard
{
protected:
	bool m_isKillShot;
	TShotData m_shotData;
	std::list<TShotDataReserve*> m_shotDataReserve;
	DirectX::XMFLOAT3 m_speed;
	bool m_isDropItem = true;
	TShotGeometry* m_myPointer;
public:
	CShot();
	virtual ~CShot();
	void SetKillShot(bool isKillShot = true) { m_isKillShot = isKillShot; }
	void Update();
	void Draw(Shader* vs, Shader* ps) override;
	void AddShotData(int frame, float speed, float degAngle, float addSpeed = 0, float addAngle = 0);
	void OnCollision(CObject* _obj) override;
	void FromPlayer(bool fromPlayer = true);
protected:
	virtual void ShotUpdate() {};

	// StaticÉÅÉìÉo
private:
	static const std::string ColorName[COLOR_MAX];
	static const std::string SizeName[SHOT_SIZE_MAX];
	static const float ShotSize[SHOT_SIZE_MAX];
	static TShotGeometry ShotVtx[1024];
	static std::queue<TShotGeometry*> EmptyShotQueue;
	static MeshBuffer* DrawMesh;
	static Shader* Shaders[3];
	static Texture ShotTex;
protected:
	static float ShotDeleteDepth;
public:
	static CShot* Create(CObject* parent, DirectX::XMFLOAT2 pos, float speed, float degAngle, std::string color, float addSpeed = 0, float addAngle = 0);

	static CShot* Create(CObject* parent, DirectX::XMFLOAT3 pos, float speed, float degAngle, std::string color, float addSpeed = 0, float addAngle = 0)
	{
		return Create(parent, DirectX::XMFLOAT2(pos.x, pos.z), speed,degAngle,color,addSpeed,addAngle);
	}
	static CShot* Create(CObject* parent, DirectX::XMFLOAT2 pos, float speed, float degAngle, EShotColor shotColor, EShotSize shotSize, float addSpeed = 0, float addAngle = 0)
	{
		return Create(parent, pos, speed, degAngle, ColorName[shotColor] + SizeName[shotSize], addSpeed, addAngle);

	}
	static CShot* Create(CObject* parent, DirectX::XMFLOAT3 pos, float speed, float degAngle, EShotColor shotColor, EShotSize shotSize, float addSpeed = 0, float addAngle = 0) 
	{
		return Create(parent, DirectX::XMFLOAT2(pos.x, pos.z), speed, degAngle, shotColor, shotSize, addSpeed = 0, addAngle = 0);
	};

	static void SetInit(CShot* shot, CObject* parent, DirectX::XMFLOAT2 pos, float speed, float degAngle, std::string color, float addSpeed = 0, float addAngle = 0);
	static void SetDeleteDepth(float newDepth);
	static void Init();
	static void Uninit();
	static void AllDraw();
};