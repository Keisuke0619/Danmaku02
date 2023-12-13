#include "Billborad.h"
#include "Sprite.h"
#include "CameraBase.h"
void CBillBoard::SetWorldMatrix()
{
	auto inv = CameraBase::GetPrimary()->GetView(false);
	inv._41 = inv._42 = inv._43 = 0.0f;
	auto mInv = DirectX::XMLoadFloat4x4(&inv);
	mInv = DirectX::XMMatrixInverse(nullptr, mInv);
	
	auto mWorld = DirectX::XMLoadFloat4x4(&m_world);
	mWorld = DirectX::XMMatrixTranspose(mWorld);
	mWorld = mInv * mWorld;
	DirectX::XMFLOAT4X4 fWorld;
	DirectX::XMStoreFloat4x4(&fWorld, DirectX::XMMatrixTranspose(mWorld));

	Sprite::SetWorld(fWorld);
}