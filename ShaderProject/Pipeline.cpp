#include "Pipeline.h"

#include "ObjectManager.h"
#include "CameraBase.h"
#include "Sprite.h"
#include "Shader.h"
#include "Defines.h"
#include "DirectX.h"
#include "PointLight.h"
#include "Geometory.h"
/// <summary>
/// ���̃f�[�^�BCBuffer�ɓn���p�B
/// </summary>
struct TLightData
{
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 pos;
	float range;
};

/// <summary>
/// �����_�[�^�[�Q�b�g�̖��O���
/// </summary>
enum EPipeRtvName
{
	RTV_ALBEDO,
	RTV_NORMAL,
	RTV_WORLD,
	RTV_SHADOW,

	RTV_MAX
};

/// <summary>
/// ���_�V�F�[�_�̖��O���
/// </summary>
enum EPipeVsName
{
	VS_DEFFERED,

	VS_MAX
};

/// <summary>
/// �s�N�Z���V�F�[�_�̖��O���
/// </summary>
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
	// �e��V�F�[�_�����[�h
	g_pipeVS[VS_DEFFERED].Load("Assets/Shader/VS_Deffered.cso");
	g_pipePS[PS_GBUFFER].Load("Assets/Shader/PS_GBuffer.cso");
	g_pipePS[PS_MAIN].Load("Assets/Shader/PS_Main.cso");

	// �����_�[�^�[�Q�b�g���쐬
	for (int i = 0; i < RTV_MAX; i++)
	{
		g_pipeRTV[i] = new RenderTarget();
	}
	g_pipeRTV[0]->Create(DXGI_FORMAT_R11G11B10_FLOAT, SCREEN_WIDTH, SCREEN_HEIGHT);
	g_pipeRTV[1]->Create(DXGI_FORMAT_R8G8B8A8_SNORM, SCREEN_WIDTH, SCREEN_HEIGHT);
	g_pipeRTV[2]->Create(DXGI_FORMAT_R32G32B32A32_FLOAT, SCREEN_WIDTH, SCREEN_HEIGHT);
	g_pipeRTV[3]->Create(DXGI_FORMAT_R32_FLOAT, SCREEN_WIDTH, SCREEN_HEIGHT);
	// �J�����O�w��
	SetCullingMode(D3D11_CULL_FRONT);
}

void PipelineUninit()
{
	PipelineClearPointLight();
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
	// �|�C���g���C�g�̓����w��B	
	int idx = 0;
	for (; idx < 128; idx++)
	{
		if (g_pipePointLight[idx].pos.z > CameraBase::GetPrimary()->GetPos().z - 15)
		{
			break;
		}
	}



	// �SRTV�̏�����
	for (auto rtv : g_pipeRTV)
	{
		rtv->Clear();
	}
	// GBuffer�ɂ��낢�돑������
	SetRenderTargets(3, g_pipeRTV, dsv);
	CObjectManager::GetIns()->Draw(&(g_pipeVS[0]), &(g_pipePS[0]), RENDER_MODEL);
	DirectX::XMFLOAT4X4 mat;

	// ���ۂɕ`�悷��Ƃ���
	SetRenderTargets(1, &rtv, nullptr);
	Sprite::SetPixelShader(&(g_pipePS[PS_MAIN]));
	// GBuffer�o�C���h
	Sprite::SetTexture(g_pipeRTV[0]);	// �[�������͂����ŏ�������
	g_pipePS[PS_MAIN].SetTexture(1, g_pipeRTV[RTV_NORMAL]);
	g_pipePS[PS_MAIN].SetTexture(2, g_pipeRTV[RTV_WORLD]);
	// �|�C���g���C�g�z��
	g_pipePS[PS_MAIN].WriteBuffer(0, &(g_pipePointLight[idx]));

	// ��ʂ����ς��Ɋg�債�Ď��`��
	DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixIdentity());
	Sprite::SetWorld(mat);
	Sprite::SetView(mat);
	Sprite::SetProjection(mat);
	Sprite::SetSize(2, 2);
	Sprite::Draw();
	
	// GBuffer��p����PS����ʏ��PS�ɖ߂�
	Sprite::SetPixelShader(nullptr);
	
	// �A���t�@�n�̕`��B�P���`��B
	SetRenderTargets(1, &rtv, dsv);
	Sprite::SetView(CameraBase::GetPrimary()->GetView());
	Sprite::SetProjection(CameraBase::GetPrimary()->GetProj());
	CObjectManager::GetIns()->Draw(nullptr, nullptr, RENDER_ALPHA);

	return;
	// GBuffer�̃f�o�b�O�\��
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
	// ���������ƂɃ��C�g�f�[�^���i�[�B
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
	// ���ׂẴ��C�g�f�[�^���������B
	for (int i = 0; i < 128; i++)
	{
		g_pipePointLight[i].pos = { 0,0,0 };
		g_pipePointLight[i].color = { 0,0,0,0 };
		g_pipePointLight[i].range = 0;
	}
}
