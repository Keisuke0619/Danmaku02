#pragma once
#include "Object.h"
#include <map>
#include <string>
#include "Texture.h"
class CWorldSprite : public CObject
{
protected:
	Texture* m_texture;



public:
	void LoadTexture(std::string src);
	void Update() override;
	void Draw(Shader* vs, Shader* ps) override;
protected:
	virtual void SetWorldMatrix();
private:
	static std::map<std::string, Texture*> m_texMap;
};
