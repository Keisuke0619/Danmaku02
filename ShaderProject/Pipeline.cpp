#include "Pipeline.h"

#include "ObjectManager.h"
#include "CameraBase.h"
#include "Sprite.h"
#include "Shader.h"
#include "Defines.h"
#include "DirectX.h"
#include "PointLight.h"

struct TLightData
{
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 pos;
	float range;
};

enum EPipeRtvName
{
	RTV_ALBEDO,
	RTV_NORMAL,
	RTV_WORLD,
	RTV_SHADOW,

	RTV_MAX
};

enum EPipeVsName
{
	VS_DEFFERED,

	VS_MAX
};

enum EPipePsName
{
	PS_GBUFFER,
	PS_MAIN,

	PS_MAX
};
VertexShader g_pipeVS[VS_MAX];
PixelShader g_pipePS[PS_MAX];
RenderTarget* g_pipeRTV[RTV_MAX];
TLightData g_pipePointLight[128];

void PipelineInit()
{
	g_pipeVS[VS_DEFFERED].Load("Assets/Shader/VS_Deffered.cso");
	g_pipePS[PS_GBUFFER].Load("Assets/Shader/PS_GBuffer.cso");
	g_pipePS[PS_MAIN].Load("Assets/Shader/PS_Main.cso");

	for (int i = 0; i < RTV_MAX; i++)
	{
		g_pipeRTV[i] = new RenderTarget();
	}

	g_pipeRTV[0]->Create(DXGI_FORMAT_R11G11B10_FLOAT, SCREEN_WIDTH, SCREEN_HEIGHT);
	g_pipeRTV[1]->Create(DXGI_FORMAT_R8G8B8A8_SNORM, SCREEN_WIDTH, SCREEN_HEIGHT);
	g_pipeRTV[2]->Create(DXGI_FORMAT_R32G32B32A32_FLOAT, SCREEN_WIDTH, SCREEN_HEIGHT);
	g_pipeRTV[3]->Create(DXGI_FORMAT_R32_FLOAT, SCREEN_WIDTH, SCREEN_HEIGHT);
	SetCullingMode(D3D11_CULL_FRONT);
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
	for (auto rtv : g_pipeRTV)
	{
		rtv->Clear();
	}

	SetRenderTargets(3, g_pipeRTV, dsv);
	CObjectManager::GetIns()->Draw(&(g_pipeVS[0]), &(g_pipePS[0]), RENDER_MODEL);
	DirectX::XMFLOAT4X4 mat;

	SetRenderTargets(1, &rtv, nullptr);
	DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixIdentity());
	Sprite::SetWorld(mat);
	Sprite::SetView(mat);
	Sprite::SetProjection(mat);
	Sprite::SetPixelShader(&(g_pipePS[PS_MAIN]));
	g_pipePS[PS_MAIN].SetTexture(1, g_pipeRTV[RTV_NORMAL]);
	g_pipePS[PS_MAIN].SetTexture(2, g_pipeRTV[RTV_WORLD]);
	g_pipePS[PS_MAIN].WriteBuffer(0, g_pipePointLight);

	Sprite::SetSize(2, 2);
	Sprite::SetTexture(g_pipeRTV[0]);
	Sprite::Draw();
	
	Sprite::SetPixelShader(nullptr);
	
	
	SetRenderTargets(1, &rtv, dsv);
	Sprite::SetView(CameraBase::GetPrimary()->GetView());
	Sprite::SetProjection(CameraBase::GetPrimary()->GetProj());
	CObjectManager::GetIns()->Draw(nullptr, nullptr, RENDER_ALPHA);

	for (int i = 0; i < RTV_MAX; i++)
	{
		Sprite::SetTexture(g_pipeRTV[i]);
		DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixIdentity());
		Sprite::SetView(mat);
		DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixTranspose(DirectX::XMMatrixOrthographicLH(8.0f, 4.5f, 0.1f, 100.0f)));
		Sprite::SetProjection(mat);
		DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixTranspose(
			DirectX::XMMatrixTranslation(2.0f, 1.5f - i * 0.75f, 0.0f)));
		Sprite::SetWorld(mat);
		Sprite::SetSize(1.5f, 0.75f);
		Sprite::Draw();
	}
}

void PipelinePushPointLight(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 color, float range)
{
	for (int i = 0; i < 128; i++)
	{
		if (g_pipePointLight[i].range != 0)
		{
			int a = 0;
			continue;
		}
		g_pipePointLight[i].pos = pos;
		g_pipePointLight[i].color = DirectX::XMFLOAT4(color.x, color.y, color.z, 1);
		g_pipePointLight[i].range = range;
		break;
	}
}

void PipelineClearPointLight()
{
	for (auto point : g_pipePointLight)
	{
		point.color = { 0,0,0,0 };
		point.pos = { 0,0,0 };
		point.range = 0;
	}
}
