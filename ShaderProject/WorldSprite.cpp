#include "WorldSprite.h"
#include "Sprite.h"
#include "CameraBase.h"
std::map<std::string, Texture*> CWorldSprite::m_texMap;

CWorldSprite::CWorldSprite()
{
	// �A���t�@�I�u�W�F�N�g�Ɏw��
	m_renderStageMask = RENDER_ALPHA;
	m_texture = nullptr;
}

void CWorldSprite::LoadTexture(std::string src)
{
	// ���łɃ��[�h����Ă���Ȃ炻�������������o���ďI���B
	if (m_texMap.count(src) != 0)
	{
		m_texture = m_texMap[src];
		return;
	}
	// ���[�h����Ă��Ȃ��Ȃ���߂ă��[�h���A�ۑ����Ă���
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
	Sprite::SetUVScale(m_uvScale.x, m_uvScale.y);
	Sprite::Draw();
	Sprite::SetUVScale(1, 1);
}

void CWorldSprite::ReleaseTexture()
{
	// ���ׂẴe�N�X�`��������
	for (auto tex : m_texMap)
	{
		delete tex.second;
	}
	m_texMap.clear();
}

void CWorldSprite::SetWorldMatrix()
{
	// �h���N���X�ŏ��������邽�߂Ɋ֐���
	Sprite::SetWorld(m_world);
}
