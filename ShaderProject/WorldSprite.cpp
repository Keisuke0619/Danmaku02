#include "WorldSprite.h"
#include "Sprite.h"
#include "CameraBase.h"
std::map<std::string, Texture*> CWorldSprite::m_texMap;

void CWorldSprite::LoadTexture(std::string src)
{
	if (m_texMap.count(src) != 0)
	{
		m_texture = m_texMap[src];
		return;
	}
	m_texture = new Texture();
	m_texture->Create(src.c_str());
	m_texMap[src] = m_texture;
}

void CWorldSprite::Update()
{
}

void CWorldSprite::Draw(Shader* vs, Shader* ps)
{
	Sprite::SetTexture(m_texture);
	SetWorldMatrix();
	Sprite::SetView(CameraBase::GetPrimary()->GetView());
	Sprite::SetProjection(CameraBase::GetPrimary()->GetProj());
	Sprite::Draw();
	Sprite::Reset();
}

void CWorldSprite::SetWorldMatrix()
{
	Sprite::SetWorld(m_world);
}
