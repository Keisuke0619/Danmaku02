#pragma once

#include "Texture.h"
#include <DirectXMath.h>
void PipelineInit();
void PipelineUninit();
void PipelineDraw(RenderTarget* defaultRTV, DepthStencil* defaultDSV);
void PipelinePushPointLight(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 color, float range);
void PipelineClearPointLight();