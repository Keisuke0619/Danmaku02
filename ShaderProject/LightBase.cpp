#include "LightBase.h"
#include "Geometory.h"
#include "CameraBase.h"
#include "Player.h"
LightBase::LightBase()
	: m_pos(0.0f, 0.0f, 0.0f)
	, m_direction(0.0f, -1.0f, 0.0f)
	, m_diffuse(1.0f, 1.0f, 1.0f, 1.0f)
	, m_ambient(0.3f, 0.3f, 0.3f, 1.0f)
{
}
LightBase::~LightBase()
{
}

DirectX::XMFLOAT3 LightBase::GetPos()
{
	return m_pos;
}
DirectX::XMFLOAT3 LightBase::GetDirection()
{
	return m_direction;
}
DirectX::XMFLOAT4 LightBase::GetDiffuse()
{
	return m_diffuse;
}
DirectX::XMFLOAT4 LightBase::GetAmbient()
{
	return m_ambient;
}

void LightBase::SetPos(DirectX::XMFLOAT3 pos)
{
	m_pos = pos;
}
void LightBase::SetDirection(DirectX::XMFLOAT3 dir)
{
	m_direction = dir;
}
void LightBase::SetDiffuse(DirectX::XMFLOAT4 color)
{
	m_diffuse = color;
}
void LightBase::SetAmbient(DirectX::XMFLOAT4 color)
{
	m_ambient = color;
}