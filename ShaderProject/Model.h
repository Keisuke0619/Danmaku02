#ifndef __MODEL_H__
#define __MODEL_H__

#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <functional>
#include "Shader.h"
#include "MeshBuffer.h"
#include "Texture.h"
#include "Object.h"
#include <map>
class Model : public CObject
{
public:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};
	struct Material
	{
		DirectX::XMFLOAT4 diffuse;
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 specular;
		std::shared_ptr<Texture> texture;
	};
	using Materials = std::vector<Material>;
	struct Mesh
	{
		std::shared_ptr<MeshBuffer> mesh;
		unsigned int materialID;
	};
	using Meshes = std::vector<Mesh>;

	struct RemakeInfo
	{
		UINT vtxNum;
		void* dest;
		const void* source;
		UINT idxNum;
		const void* idx;
	};

public:
	Model();
	~Model();
	void SetVertexShader(Shader* vs);
	void SetPixelShader(Shader* ps);
	const Mesh* GetMesh(unsigned int index);
	unsigned int GetMeshNum();

public:
	bool Load(const char* file, float scale = 1.0f, bool flip = false, DirectX::XMFLOAT3 offset = {0,0,0});
	void Draw(Shader* vs, Shader* ps);
	void TestDraw();
	
	void RemakeVertex(int vtxSize, std::function<void(RemakeInfo& data)> func);

private:
	void MakeDefaultShader();

private:
	static std::shared_ptr<VertexShader> m_defVS;
	static std::shared_ptr<PixelShader> m_defPS;
private:
	Meshes m_meshes;
	Materials m_materials;
	VertexShader* m_pVS;
	PixelShader* m_pPS;

};

class MeshPool
{
private:
	static MeshPool* m_Ins;
public:
	static MeshPool* Ins() { if (m_Ins) return m_Ins; m_Ins = new MeshPool(); return m_Ins; }
private:
	std::map<std::string, Model::Meshes> m_meshes;
	std::map<std::string, Model::Materials> m_materials;
public:
	bool Load(const char* file, Model::Meshes* mesh, Model::Materials* material, float scale = 1.0f, bool flip = false, DirectX::XMFLOAT3 offset = {0,0,0});
	void DeleteAll();
};

#endif // __MODEL_H__