#include "Object.h"

void CObject::Init(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 degRot, DirectX::XMFLOAT3 scale, unsigned renderMask)
{
	m_pos = pos;
	m_degRot = degRot;
	m_scale = scale;
	m_renderStageMask = renderMask;
}

void CObject::UpdateBase()
{
	Update();
	auto T = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	auto R = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_degRot.y));
	R *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_degRot.x));
	R *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_degRot.z));
	auto S = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	auto mat = S * R * T;
	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMStoreFloat4x4(&m_world, mat);
}

void CObject::Update()
{
}

void CObject::DrawForBuffer()
{
	Draw();
}

void CObject::DrawForAlpha()
{
	Draw();
}
