#pragma once

#include "Model.h"
#include "DirectX.h"
enum EDrawMask
{
	RENDER_BUFFER = 1 << 0,
	RENDER_ALPHA = 1 << 1,

};
class CObject : public Model
{
private:
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_degRot;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT4X4 m_world;
	unsigned m_renderStageMask;
public:
	void Init(DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3 degRot = DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(0, 0, 0), unsigned renderMask = 1);
	virtual void UpdateBase() final;
	void DrawForBuffer();
	void DrawForAlpha();

protected:
	virtual void Update();

};