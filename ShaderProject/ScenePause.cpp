#include "ScenePause.h"
#include "Input.h"
#include "Sprite.h"

void CScenePause::Init()
{
	// オーバーレイシーンに設定し、ポーズ画像を表示
	m_isOverlay = true;
	m_back = new Texture();
	m_back->Create("Assets/Texture/UI/Pause_Back.png");
}

void CScenePause::Uninit()
{
	delete m_back;
}

void CScenePause::Update(float tick)
{
	// escで開いてescで閉じる関係で、１フレーム目に処理すると開いたと同時に閉じてしまう。
	// 基底Updateにて先にm_frameをインクリメントしている関係で m_frame == 1 が１フレーム目となる。
	if (m_frame == 1)
	{
		return;
	}
	if (IsKeyTrigger(VK_ESCAPE))
	{
		// コールバック関数を呼んで終了
		m_callBack->CallBack(1);
	}
}

void CScenePause::Draw()
{
	Sprite::Reset();
	Sprite::SetTexture(m_back);
	Sprite::SetSize(2, 2);
	Sprite::Draw();
	Sprite::Reset();
}

void CScenePause::SetCallBack(IEventCallBack* callBack)
{
	// コールバックを設定。
	m_callBack = callBack;
}
