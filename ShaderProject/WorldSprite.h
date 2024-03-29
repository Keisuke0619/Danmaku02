#pragma once
#include "Object.h"
#include <map>
#include <string>
#include "Texture.h"
class CWorldSprite : public CObject
{
protected:
	Texture* m_texture;
	DirectX::XMFLOAT2 m_uvScale = DirectX::XMFLOAT2(1,1);


public:
	CWorldSprite();
	void LoadTexture(std::string src);
	void Update() override;
	void Draw(Shader* vs, Shader* ps) override;

	void SetUVScale(DirectX::XMFLOAT2 uvScale) { m_uvScale = uvScale; }

	static void ReleaseTexture();
protected:
	virtual void SetWorldMatrix();
private:
	static std::map<std::string, Texture*> m_texMap;
};
