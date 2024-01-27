#include "Pipeline.h"

#include "ObjectManager.h"
#include "CameraBase.h"
#include "Sprite.h"
#include "Shader.h"
#include "Defines.h"

enum EPipeRtvName
{
	RTV_ALBEDO,
	RTV_NORMAL,
	RTV_WORLD,
	RTV_SHADOW,

	RTV_MAX
};

VertexShader g_pipeVS[5];
PixelShader g_pipePS[5];
RenderTarget* g_pipeRTV[RTV_MAX];


void PipelineInit()
{
	g_pipeVS[0].Load("Assets/Shader/VS_Object.cso");
	g_pipePS[0].Load("Assets/Shader/PS_TexColor.cso");

	for (int i = 0; i < RTV_MAX; i++)
	{
		g_pipeRTV[i] = new RenderTarget();
	}

	g_pipeRTV[0]->Create(DXGI_FORMAT_R11G11B10_FLOAT, SCREEN_WIDTH, SCREEN_HEIGHT);
	g_pipeRTV[1]->Create(DXGI_FORMAT_R11G11B10_FLOAT, SCREEN_WIDTH, SCREEN_HEIGHT);
	g_pipeRTV[2]->Create(DXGI_FORMAT_R32_FLOAT, SCREEN_WIDTH, SCREEN_HEIGHT);
	g_pipeRTV[3]->Create(DXGI_FORMAT_R32_FLOAT, SCREEN_WIDTH, SCREEN_HEIGHT);

}

void PipelineUninit()
{
	for (int i = 0; i < RTV_MAX; i++)
	{
		if (g_pipeRTV[i])
		{
			delete g_pipeRTV[i];
			g_pipeRTV[i] = nullptr;
		}
	}
}

void PipelineDraw(RenderTarget* rtv, DepthStencil* dsv)
{

	Sprite::SetView(CameraBase::GetPrimary()->GetView());
	Sprite::SetProjection(CameraBase::GetPrimary()->GetProj());
	CObjectManager::GetIns()->Draw(&(g_pipeVS[0]), &(g_pipePS[0]), RENDER_MODEL);
	
	
	
	
	
	
	
	CObjectManager::GetIns()->Draw(&(g_pipeVS[0]), &(g_pipePS[0]), RENDER_ALPHA);
}
