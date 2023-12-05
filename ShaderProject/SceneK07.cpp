#include "SceneK07.h"

#include "CameraBase.h"
#include "LightBase.h"
#include "Input.h"
#include "Sprite.h"
#include "Defines.h"

void SceneK07::Init()
{
	Shader* shader[] = {
	CreateObj<VertexShader>("VS_Object"),
	CreateObj<PixelShader>("PS_TexColor"),
	CreateObj<PixelShader>("PS_FrDepth"),
	CreateObj<VertexShader>("VS_SpriteShadow"),
	CreateObj<VertexShader>("VS_K07"),
	CreateObj<PixelShader>("PS_K07Shadow"),
	};
	const char* filePath[] = {
		"Assets/Shader/VS_Object.cso",
		"Assets/Shader/PS_TexColor.cso",
		"Assets/Shader/PS_FrDepth.cso",
		"Assets/Shader/VS_SpriteShadow.cso",
		"Assets/Shader/VS_K07.cso",
		"Assets/Shader/PS_K07Shadow.cso",
	};
	static_assert(_countof(shader) == _countof(filePath), "");
	for (int i = 0; i < _countof(shader); ++i)
		shader[i]->Load(filePath[i]);

	// 深度バッファ
	auto rtv = CreateObj<RenderTarget>("DepthRTV");
	rtv->Create(DXGI_FORMAT_R32_FLOAT, SCREEN_WIDTH, SCREEN_HEIGHT);
	auto dsv = CreateObj<DepthStencil>("OnceDSV");
	dsv->Create(SCREEN_WIDTH, SCREEN_HEIGHT, false);

	// ライトバッファ
	const float bufSize = 4.0f;
	rtv = CreateObj<RenderTarget>("LightRTV");
	rtv->Create(DXGI_FORMAT_R32_FLOAT, SCREEN_WIDTH * bufSize, SCREEN_HEIGHT * bufSize);
	dsv = CreateObj<DepthStencil>("LightDSV");
	dsv->Create(SCREEN_WIDTH * bufSize, SCREEN_HEIGHT * bufSize, false);
	
	Sprite::SetVertexShader(nullptr);
	Sprite::SetPixelShader(nullptr);

	// モデル読み込み
	CreateObj<Model>("Field")->Load("Assets/Model/K07/SP_k07_Field.fbx", 1.0f, false);
	CreateObj<Model>("Tree")->Load("Assets/Model/K07/Tree.fbx", 0.003f);
	//CreateObj<Model>("Building01")->Load("Assets/Model/K07/SP_k07_Building01.fbx", 1.0f, false);
	//CreateObj<Texture>("FieldTexture")->Create("Assets/Model/field/p0305_m.jpg");

	InitSky();
}

void SceneK07::Uninit()
{
}

void SceneK07::Update(float tick)
{
	
}


void SceneK07::Draw()
{

	// 描画するモデル設定
	std::list<ModelData> models;
	DirectX::XMMATRIX modelMat = DirectX::XMMatrixIdentity();
	models.push_back(CreateData(GetObj<Model>("Model"), { -1,0.7f,0 }, {0.28f,0,0}));
	models.push_back(CreateData(GetObj<Model>("Tree"), { 4.0f, 1.5f, -3.0f }, {0,0,0}, {1,2,1}));
	models.push_back(CreateData(GetObj<Model>("Tree"), { -4.0f, 1.15f, -0.6f }, { 0,0,0 }, { 1,2,1 }));
	models.push_back(CreateData(GetObj<Model>("Field")));

	// いろいろ読み込み
	auto cam = GetObj<CameraBase>("Camera");
	auto light = GetObj<LightBase>("Light");
	auto rtvDepth = GetObj<RenderTarget>("DepthRTV");
	auto dsvOnce = GetObj<DepthStencil>("OnceDSV");
	auto rtvDefault = GetObj<RenderTarget>("RTV");
	auto dsvDefault = GetObj<DepthStencil>("DSV");
	auto rtvLight = GetObj<RenderTarget>("LightRTV");
	auto dsvLight = GetObj<DepthStencil>("LightDSV");

	Shader* shader[] = {
		GetObj<Shader>("VS_Object"),
		GetObj<Shader>("PS_TexColor"),
		GetObj<Shader>("PS_FrDepth"),
		GetObj<Shader>("VS_SpriteShadow"),
		GetObj<Shader>("VS_K07"),
		GetObj<Shader>("PS_K07Shadow"),
	};
	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());
	mat[1] = cam->GetView();
	mat[2] = cam->GetProj();

	DirectX::XMFLOAT3 lightDir = light->GetDirection();
	DirectX::XMFLOAT4 lightBuf[] = {
		light->GetDiffuse(),
		light->GetAmbient(),
		{lightDir.x, lightDir.y, lightDir.z, 0.0f}
	};
	DirectX::XMFLOAT3 lightPos = light->GetPos();
	DirectX::XMFLOAT4X4 lightMat[3];
	DirectX::XMFLOAT4 lightDirBuf[] = {
		light->GetDiffuse(),
		light->GetAmbient(),
		{lightDir.x, lightDir.y, lightDir.z, 0.0f}
	};
	lightMat[0] = mat[0];
	DirectX::XMStoreFloat4x4(
		&lightMat[1],
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixLookAtLH(
				DirectX::XMVectorSet(lightPos.x, lightPos.y, lightPos.z, 0),
				DirectX::XMVectorSet(lightPos.x + lightDir.x, lightPos.y + lightDir.y, lightPos.z + lightDir.z, 0),
				DirectX::XMVectorSet(0, 1, 0, 0)
			)
		)
	);
	DirectX::XMStoreFloat4x4(
		&lightMat[2],
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixOrthographicLH(
				20.0f,
				20.0f,
				-100.0f,
				100.0f
			)
		)
	);

	shader[0]->WriteBuffer(0, mat);
	shader[0]->WriteBuffer(1, lightBuf);

	// 深度描画
	dsvOnce->Clear();
	rtvDepth->Clear();
	SetRenderTargets(1, &rtvDepth, dsvOnce);
	DrawContent(models, shader[0], shader[2], mat);

	// ライト描画
	float rtvReset[] = { 1,1,1,1 };
	rtvLight->Clear(rtvReset);
	dsvLight->Clear();
	SetRenderTargets(1, &rtvLight, dsvLight);
	DrawContent(models, shader[0], shader[2], lightMat);

	// スカイキューブ描画
	SetRenderTargets(1, &rtvDefault, nullptr);
	DrawSky();


	// 本描画
	shader[4]->WriteBuffer(1, lightMat);
	shader[5]->SetTexture(1, rtvLight);
	shader[5]->WriteBuffer(0, &lightDirBuf);
	SetRenderTargets(1, &rtvDefault, dsvDefault);
	
	{
		DirectX::XMFLOAT4X4 world = *mat;
		for (auto model : models)
		{
			mat[0] = model.world;
			lightMat[0] = mat[0];
			shader[4]->WriteBuffer(0, mat);
			shader[4]->WriteBuffer(1, lightMat);
			Model::SetVertexShader(shader[4]);
			Model::SetPixelShader(shader[5]);
			model.model->Draw();
		}
		mat[0] = world;
	}



	DirectX::XMStoreFloat4x4(&mat[1],
		DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mat[2],	// 平衡投影
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixOrthographicLH(1, 1, 0, 1)	// 横幅縦幅奥行
		));
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetSize(0.2f, 0.2f);
	Sprite::SetVertexShader(nullptr);
	Sprite::SetPixelShader(nullptr);

	Sprite::SetTexture(rtvDepth);
	Sprite::SetOffset(0.37f, 0.37f);
	Sprite::Draw();

	Sprite::SetTexture(rtvLight);
	Sprite::SetOffset(0.37f, 0.15f);
	Sprite::Draw();
}

void SceneK07::DrawContent(std::list<ModelData> models, Shader* vs, Shader* ps, DirectX::XMFLOAT4X4* mat)
{
	DirectX::XMFLOAT4X4 world = *mat;
	for (auto model : models)
	{
		mat[0] = model.world;
		vs->WriteBuffer(0, mat);
		model.model->SetVertexShader(vs);
		model.model->SetPixelShader(ps);
		model.model->Draw();
	}
	mat[0] = world;
}

ModelData SceneK07::CreateData(Model* model, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale)
{
	DirectX::XMMATRIX mat = DirectX::XMMatrixIdentity();
	mat *= DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	mat *= DirectX::XMMatrixRotationX(rot.x);
	mat *= DirectX::XMMatrixRotationY(rot.y);
	mat *= DirectX::XMMatrixRotationZ(rot.z);
	mat *= DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	mat = DirectX::XMMatrixTranspose(mat);
	ModelData ret;
	ret.model = model;
	DirectX::XMStoreFloat4x4(&(ret.world), mat);
	return ret;
}

void SceneK07::InitSky()
{
	Texture* texes[] =
	{
		CreateObj<Texture>("Sky01"),
		CreateObj<Texture>("Sky02"),
		CreateObj<Texture>("Sky03"),
		CreateObj<Texture>("Sky04"),
		CreateObj<Texture>("Sky05"),
		CreateObj<Texture>("Sky06"),
	};
	const char* texPath[] =
	{
		"Assets/Texture/Sky/Sky01.tga",
		"Assets/Texture/Sky/Sky02.tga",
		"Assets/Texture/Sky/Sky03.tga",
		"Assets/Texture/Sky/Sky04.tga",
		"Assets/Texture/Sky/Sky05.tga",
		"Assets/Texture/Sky/Sky06.tga",
	};
	for (int i = 0; i < _countof(texes); ++i)
		texes[i]->Create(texPath[i]);
}

void SceneK07::DrawSky()
{
	DirectX::XMFLOAT4X4 mat[3] = {};
	auto pCamera = GetObj<CameraBase>("Camera");
	mat[1] = pCamera->GetView();
	mat[2] = pCamera->GetProj();

	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetPixelShader(nullptr);
	Sprite::SetVertexShader(nullptr);

	Texture* skies[] = {
		GetObj<Texture>("Sky01"),
		GetObj<Texture>("Sky04"),
		GetObj<Texture>("Sky02"),
		GetObj<Texture>("Sky03"),
		GetObj<Texture>("Sky05"),
		GetObj<Texture>("Sky06"),
	};
	DirectX::XMFLOAT3 skyPos[] =
	{
		{0, -0.5f, 0},
		{0, 0.5f, 0},
		{0, 0, 0.5f},
		{0.5f, 0, 0},
		{0, 0, -0.5f},
		{-0.5f, 0, 0},
	};
	DirectX::XMFLOAT2 skyRot[] =
	{
		{3,1},
		{3,3},
		{0,0},
		{1,0},
		{2,0},
		{3,0},
	};
	Sprite::SetSize(1, 1);
	Sprite::SetOffset(0, 0);
	for (int i = 0; i < 6; i++)
	{
		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationX(skyRot[i].y * 3.14f / 2) * DirectX::XMMatrixRotationY(skyRot[i].x * 3.14f / 2) * DirectX::XMMatrixTranslation(skyPos[i].x, skyPos[i].y, skyPos[i].z) * DirectX::XMMatrixTranslation(pCamera->GetPos().x, pCamera->GetPos().y, pCamera->GetPos().z)));
		Sprite::SetWorld(mat[0]);
		Sprite::SetTexture(skies[i]);
		Sprite::Draw();
	}
}
