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

#ifdef _DEBUG
void LightBase::Draw()
{
	auto player = CPlayer::GetPrimary();
	if (player == nullptr)return;
	auto drawPos = player->GetPos();
	Geometory::SetView(CameraBase::GetPrimary()->GetView());
	Geometory::SetProjection(CameraBase::GetPrimary()->GetProj());
	DirectX::XMFLOAT3 end(
		drawPos.x - m_direction.x,
		drawPos.y - m_direction.y,
		drawPos.z - m_direction.z
	);
	Geometory::SetColor(m_diffuse);
	Geometory::AddLine(drawPos, end);
	Geometory::DrawLines();

	DirectX::XMFLOAT4X4 mat;
	DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixTranspose(
		DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f) *
		DirectX::XMMatrixTranslation(end.x, end.y, end.z)
	));
	Geometory::SetWorld(mat);
	Geometory::DrawBox();
}
#endif

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