#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <DirectXMath.h>
#include <memory>
#include "MeshBuffer.h"
#include "Shader.h"
#include "Texture.h"

class Sprite
{
public:
	static void Init();
	static void Uninit();
	static void Draw();

	static void SetOffset(DirectX::XMFLOAT2 offset);
	static void SetSize(DirectX::XMFLOAT2 size);
	static void SetUVPos(DirectX::XMFLOAT2 pos);
	static void SetUVScale(DirectX::XMFLOAT2 scale);
	static void SetColor(DirectX::XMFLOAT4 color);
	static void SetOffset(float _x, float _y) { SetOffset(DirectX::XMFLOAT2(_x, _y)); }
	static void SetSize(float _x, float _y) { SetSize(DirectX::XMFLOAT2(_x, _y)); }
	static void SetUVPos(float _x, float _y) { SetUVPos(DirectX::XMFLOAT2(_x, _y)); }
	static void SetUVScale(float _x, float _y) { SetUVScale(DirectX::XMFLOAT2(_x, _y)); }
	static void SetColor(float _r, float _g, float _b, float _a = 1) { SetColor(DirectX::XMFLOAT4(_r, _g, _b, _a));	}
	static void SetTexture(Texture* tex);

	static void SetWorld(DirectX::XMFLOAT4X4 world);
	static void SetView(DirectX::XMFLOAT4X4 view);
	static void SetProjection(DirectX::XMFLOAT4X4 proj);

	static void SetVertexShader(Shader* vs);
	static void SetPixelShader(Shader* ps);

	static void Reset();
private:
	struct Data
	{
		std::shared_ptr<MeshBuffer> mesh;
		DirectX::XMFLOAT4X4 matrix[3];
		DirectX::XMFLOAT4 param[3];
		Texture* texture;
		Shader* vs;
		Shader* ps;
	};
	static Data m_data;
	static std::shared_ptr<VertexShader> m_defVS;
	static std::shared_ptr<PixelShader> m_defPS;
};

#endif // __SPRITE_H__