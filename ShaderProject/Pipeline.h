#pragma once

#include "Texture.h"

void PipelineInit();
void PipelineUninit();
void PipelineDraw(RenderTarget* defaultRTV, DepthStencil* defaultDSV);