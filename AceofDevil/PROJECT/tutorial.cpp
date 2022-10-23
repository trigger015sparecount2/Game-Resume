//=============================================================================
//
// ゲーム [game.h]
// Author : 三上航世
//
//=============================================================================
#include "scene2d.h"
#include "input.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "sound.h"
#include "bg.h"
#include "enemy.h"
#include "effect.h"
#include "tutorial.h"
#include "manager.h"
#include "fade.h"
#include "fire.h"
#include "ice.h"
#include "thunder.h"
#include "thunderband.h"
#include "water.h"
#include "wind.h"
#include "ui.h"
#include "shadow.h"
#include "prize.h"
#include "thundereffect.h"
#include "tutorialui.h"
#include "pauseui.h"

CTutorial::CTutorial(PRIORITY Priority) : CScene::CScene(Priority)
{

}

CTutorial::~CTutorial(void)
{

}

HRESULT CTutorial::Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize)
{
	CBullet::Load();
	CExplosion::Load();
	CPlayer::Load();
	CBg::Load();
	CEnemy::Load();
	CUi::Load();
	CEffect::Load();
	CFire::Load();
	CIce::Load();
	CThunder::Load();
	CThunderBand::Load();
	CWater::Load();
	CWind::Load();
	CShadow::Load();
	CPrize::Load();
	CThundereffect::Load();
	CTutorialUi::Load();
	CPauseUi::SetPause(false);
	CBg::Create(true);
	CUi::Create();
	CTutorialUi::Create();
	CPlayer::Create(D3DXVECTOR3(640.0f, 600.0f, 0.0f));
	CPlayer::SetTuto(true);
	CScene::SetObjType(CScene::OBJTYPE_NONE);
	m_nTime = 0;
	m_pInputKeyboard = CManager::GetInputKeyboard();
	m_pInputGamePad = CManager::GetInputGamePad();
	CSound::Play(CSound::SOUND_LABEL_TUTORIALBGM);
	return S_OK;
}

void CTutorial::Uninit(void)
{
	CBullet::UnLoad();
	CEnemy::UnLoad();
	CExplosion::UnLoad();
	CPlayer::UnLoad();
	CBg::UnLoad();
	CUi::UnLoad();
	CEffect::UnLoad();
	CFire::UnLoad();
	CIce::UnLoad();
	CThunder::UnLoad();
	CThunderBand::UnLoad();
	CWater::UnLoad();
	CWind::UnLoad();
	CShadow::UnLoad();
	CPrize::UnLoad();
	CThundereffect::UnLoad();
	CTutorialUi::UnLoad();
	if (m_pInputGamePad != NULL)
	{
		m_pInputGamePad = NULL;
	}
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard = NULL;
	}
	Release();
}

void CTutorial::Update(void)
{
	if (CTutorialUi::GetClear() == false)
	{
		m_nTime++;
	}
	if (m_pInputGamePad != NULL)
	{
		if (m_pInputGamePad->GetButton(CInputGamePad::DIP_START) == true)
		{
			CFade::SetFade(CManager::MODE_GAME);
		}
	}
	else if (m_pInputKeyboard != NULL)
	{
		if (m_pInputKeyboard->GetKey(DIK_RETURN) == true)
		{
			CFade::SetFade(CManager::MODE_GAME);
		}
	}
	switch (m_nTime)
	{
	case 600:
		CTutorialUi::NextText(); //操作説明②
		CEnemy::Create(D3DXVECTOR3(640.0f, -ENEMY_SIZE * 2.0f, 0.0f), 0, 1.0f, 400, 1.0f);
		break;
	case 1200:
		CTutorialUi::NextText(); //火、氷の説明
		CEnemy::Create(D3DXVECTOR3(840.0f, -ENEMY_SIZE * 2.0f, 0.0f), 0, 1.0f, 200, 1.5f);
		CEnemy::Create(D3DXVECTOR3(440.0f, -ENEMY_SIZE * 2.0f, 0.0f), 0, 1.0f, 200, 1.5f);
		CEnemy::Create(D3DXVECTOR3(1040.0f, -ENEMY_SIZE * 2.0f, 0.0f), 1, 0.8f, 300, 2.0f);
		break;
	case 2000:
		CTutorialUi::NextText(); //雷の説明
		CEnemy::Create(D3DXVECTOR3(640.0f, -ENEMY_SIZE * 2.0f, 0.0f), 3, 1.0f, 200, 1.3f);
		CEnemy::Create(D3DXVECTOR3(940.0f, -ENEMY_SIZE * 2.0f, 0.0f), 3, 0.9f, 200, 1.3f);
		CEnemy::Create(D3DXVECTOR3(340.0f, -ENEMY_SIZE * 2.0f, 0.0f), 3, -0.9f, 200, 1.3f);
		break;
	case 2800:
		CTutorialUi::NextText(); //水の説明
		CEnemy::Create(D3DXVECTOR3(540.0f, SCREEN_HEIGHT + ENEMY_SIZE * 2.0f, 0.0f), 2, 0.0f, 220, 0.8f);
		CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2.0f, 300.0f, 0.0f), 2, 0.5f, 220, 0.8f);
		CEnemy::Create(D3DXVECTOR3(740.0f, -ENEMY_SIZE * 2.0f, 0.0f), 2, 1.0f, 220, 0.8f);
		CEnemy::Create(D3DXVECTOR3(-ENEMY_SIZE * 2.0f, 420.0f, 0.0f), 2, -0.5f, 220, 0.8f);
		break;
	case 3600:
		CTutorialUi::NextText(); //風の説明
		CEnemy::Create(D3DXVECTOR3(540.0f, SCREEN_HEIGHT + ENEMY_SIZE * 2.0f, 0.0f), 0, 0.0f, 150, 1.2f);
		CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2.0f, 300.0f, 0.0f), 1, 0.5f, 150, 1.2f);
		CEnemy::Create(D3DXVECTOR3(740.0f, -ENEMY_SIZE * 2.0f, 0.0f), 2, 1.0f, 150, 1.2f);
		CEnemy::Create(D3DXVECTOR3(-ENEMY_SIZE * 2.0f, 420.0f, 0.0f), 3, -0.5f, 150, 1.2f);
		break;
	case 4400:
		CTutorialUi::NextText(); //属性関係のおさらい
		CEnemy::Create(D3DXVECTOR3(490.0f, -ENEMY_SIZE * 2.0f, 0.0f), 0, 0.8f, 120, 1.0f);
		CEnemy::Create(D3DXVECTOR3(590.0f, -ENEMY_SIZE * 2.0f, 0.0f), 1, 0.9f, 120, 1.0f);
		CEnemy::Create(D3DXVECTOR3(690.0f, -ENEMY_SIZE * 2.0f, 0.0f), 2, -0.9f, 120, 1.0f);
		CEnemy::Create(D3DXVECTOR3(790.0f, -ENEMY_SIZE * 2.0f, 0.0f), 3, -0.8f, 120, 1.0f);
		break; 
	case 5200:
		CTutorialUi::NextText(); //スコアや撃墜数の説明
		CEnemy::Create(D3DXVECTOR3(740.0f, -ENEMY_SIZE * 2.0f, 0.0f), 0, 1.0f, 200, 1.0f);
		CEnemy::Create(D3DXVECTOR3(540.0f, -ENEMY_SIZE * 2.0f, 0.0f), 0, 1.0f, 200, 1.0f);
		break;
	case 6000:
		CTutorialUi::NextText(); //属性ゲージの説明
		CEnemy::Create(D3DXVECTOR3(740.0f, -ENEMY_SIZE * 2.0f, 0.0f), 1, 1.0f, 200, 1.0f);
		CEnemy::Create(D3DXVECTOR3(540.0f, -ENEMY_SIZE * 2.0f, 0.0f), 1, 1.0f, 200, 1.0f);
		break;
	case 6800:
		CTutorialUi::NextText(); //DMGの説明
		CEnemy::Create(D3DXVECTOR3(740.0f, -ENEMY_SIZE * 2.0f, 0.0f), 2, 1.0f, 200, 1.0f);
		CEnemy::Create(D3DXVECTOR3(540.0f, -ENEMY_SIZE * 2.0f, 0.0f), 2, 1.0f, 200, 1.0f);
		break;
	case 7600:
		CTutorialUi::NextText(); //最後
		CEnemy::Create(D3DXVECTOR3(740.0f, -ENEMY_SIZE * 2.0f, 0.0f), 3, 1.0f, 200, 1.0f);
		CEnemy::Create(D3DXVECTOR3(540.0f, -ENEMY_SIZE * 2.0f, 0.0f), 3, 1.0f, 200, 1.0f);
		break;
	case 8200:
		CTutorialUi::NextText(); //フェード
		break;
	}
}

void CTutorial::Draw(void)
{

}

void CTutorial::Damage(int nDamage)
{

}

bool CTutorial::GetDamage(void)
{
	return false;
}