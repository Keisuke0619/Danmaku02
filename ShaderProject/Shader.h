#ifndef __SHADER_H__
#define __SHADER_H__

#include "DirectX.h"
#include "Texture.h"
#include "UnorderedAccessView.h"
#include <string>
#include <map>
#include <vector>

// シェーダーの基本クラス
class Shader
{
protected:
	enum Kind
	{
		Vertex,
		Geometry,
		Hull,
		Domain,
		Pixel,
		Compute,
	};
protected:
	Shader(Kind kind);
public:
	virtual ~Shader();
public:
	// シェーダーファイル(*.cso)を読み込む処理
	HRESULT Load(const char* pFileName);
	// 文字列からシェーダをコンパイル
	HRESULT Compile(const char* pCode);


	// 定数の書き込み
	void WriteBuffer(UINT slot, void* pData);
	// テクスチャの設定
	void SetTexture(UINT slot, Texture* tex);
	// シェーダーを描画に使用
	virtual void Bind(void) = 0;

private:
	HRESULT Make(void* pData, UINT size);
protected:
	// シェーダーファイルを読み込んだ後、シェーダーの種類別に処理を行う
	virtual HRESULT MakeShader(void* pData, UINT size) = 0;

private:
	Kind m_kind;
protected:
	std::vector<ID3D11Buffer*> m_pBuffers;
	std::vector<ID3D11ShaderResourceView*> m_pTextures;
};

//----------
// 頂点シェーダ
class VertexShader : public Shader
{
public:
	VertexShader();
	~VertexShader();
	void Bind(void);
protected:
	HRESULT MakeShader(void* pData, UINT size);

private:
	ID3D11VertexShader* m_pVS;
	ID3D11InputLayout* m_pInputLayout;
};
//----------
// ピクセルシェーダ
class PixelShader : public Shader
{
public:
	PixelShader();
	~PixelShader();
	void Bind(void);
protected:
	HRESULT MakeShader(void* pData, UINT size);
private:
	ID3D11PixelShader* m_pPS;
};

//----------
// ハルシェーダー
class HullShader : public Shader
{
public:
	HullShader();
	~HullShader();
	void Bind();
	static void Unbind();
protected:
	HRESULT MakeShader(void* pData, UINT size);
private:
	ID3D11HullShader* m_pHS;
};

//----------
// ドメインシェーダー
class DomainShader : public Shader
{
public:
	DomainShader();
	~DomainShader();
	void Bind();
	static void Unbind();
protected:
	HRESULT MakeShader(void* pData, UINT size);
private:
	ID3D11DomainShader* m_pDS;
};

//----------
// ジオメトリシェーダー
class GeometryShader : public Shader
{
public:
	GeometryShader();
	~GeometryShader();
	void Bind();
	static void Unbind();
protected:
	HRESULT MakeShader(void* pData, UINT size);
private:
	ID3D11GeometryShader* m_pGS;
};

//----------
// コンピュートシェーダー
class ComputeShader : public Shader
{
public:
	ComputeShader();
	~ComputeShader();
	void Bind();
	static void Unbind();
	void Dispatch(UINT num, UnorderedAccessView** ppUAV, UINT thread);

protected:
	HRESULT MakeShader(void* pData, UINT size);
private:
	ID3D11ComputeShader* m_pCS;
};


#endif