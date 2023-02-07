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
#include "game.h"
#include "manager.h"
#include "fade.h"
#include "fire.h"
#include "ice.h"
#include "thunder.h"
#include "thunderband.h"
#include "water.h"
#include "wind.h"
#include "ui.h"
#include "pauseui.h"
#include "shadow.h"
#include "prize.h"
#include "thundereffect.h"
#include "boss.h"

CGame::CGame(PRIORITY Priority) : CScene::CScene(Priority)
{

}

CGame::~CGame(void)
{

}

//初期化
HRESULT CGame::Init(float, float, D3DXVECTOR3, float)
{
	//テクスチャの読み込み
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
	CPauseUi::Load();
	CShadow::Load();
	CPrize::Load();
	CThundereffect::Load();
	CBoss::Load();

	//背景、UI、プレイヤー、ポーズUIの作成、音楽を流す
	CBg::Create(true);
	CUi::Create();
	CPlayer::Create(D3DXVECTOR3(640.0f, 600.0f, 0.0f));
	CPlayer::SetTuto(false);
	CScene::SetObjType(CScene::OBJTYPE_NONE);
	CPauseUi::Create();
	CPauseUi::SetPause(false);
	CSound::Play(CSound::SOUND_LABEL_GAMEBGM);
	
	//時間リセット
	m_nTime = 0;
	return S_OK;
}

//終了
void CGame::Uninit(void)
{
	//テクスチャの破棄
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
	CPauseUi::UnLoad();
	CShadow::UnLoad();
	CPrize::UnLoad();
	CThundereffect::UnLoad();
	CBoss::UnLoad();

	Release();
}

//更新
void CGame::Update(void)
{
	//ポーズ中じゃない
	if (CPauseUi::GetPause() == false)
	{
		//時間を進める
		m_nTime++;
		//時間によって敵を作成
		switch (m_nTime)
		{
		case 100:
			CEnemy::Create(D3DXVECTOR3(340.0f, -ENEMY_SIZE * 2.0f, 0.0f), 0, -0.9f, 150, 1.4f);
			break;
		case 200:
			CEnemy::Create(D3DXVECTOR3(540.0f, -ENEMY_SIZE * 2.0f, 0.0f), 0, 1.0f, 150, 1.4f);
			break;
		case 300:
			CEnemy::Create(D3DXVECTOR3(740.0f, -ENEMY_SIZE * 2.0f, 0.0f), 0, 0.9f, 150, 1.4f);
			break;
		case 500:
			CEnemy::Create(D3DXVECTOR3(940.0f, -ENEMY_SIZE * 2.0f, 0.0f), 1, -0.9f, 300, 1.4f);
			break;
		case 600:
			CEnemy::Create(D3DXVECTOR3(740.0f, -ENEMY_SIZE * 2.0f, 0.0f), 1, 1.0f, 210, 1.4f);
			break;
		case 700:
			CEnemy::Create(D3DXVECTOR3(540.0f, -ENEMY_SIZE * 2.0f, 0.0f), 1, 0.9f, 120, 1.4f);
			break;
		case 900:
			CEnemy::Create(D3DXVECTOR3(640.0f, -ENEMY_SIZE * 2.0f, 0.0f), 2, 1.0f, 200, 1.3f);
			CEnemy::Create(D3DXVECTOR3(540.0f, -ENEMY_SIZE * 2.0f, 0.0f), 2, 0.9f, 200, 1.3f);
			CEnemy::Create(D3DXVECTOR3(740.0f, -ENEMY_SIZE * 2.0f, 0.0f), 2, -0.9f, 200, 1.3f);
			break;
		case 1100:
			CEnemy::Create(D3DXVECTOR3(640.0f, -ENEMY_SIZE * 2.0f, 0.0f), 3, 1.0f, 270, 1.5f);
			break;
		case 1150:
			CEnemy::Create(D3DXVECTOR3(540.0f, -ENEMY_SIZE * 2.0f, 0.0f), 3, 1.0f, 120, 1.5f);
			CEnemy::Create(D3DXVECTOR3(740.0f, -ENEMY_SIZE * 2.0f, 0.0f), 3, 1.0f, 120, 1.5f);
			break;
		case 1300:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2, 100.0f, 0.0f), 2, 0.7f, 270, 1.0f);
			break;
		case 1380:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2, 100.0f, 0.0f), 2, 0.7f, 220, 1.0f);
			break;
		case 1460:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2, 100.0f, 0.0f), 2, 0.7f, 170, 1.0f);
			break;
		case 1540:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2, 100.0f, 0.0f), 2, 0.7f, 120, 1.0f);
			break;
		case 1800:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT + ENEMY_SIZE * 2, 0.0f), 3, 0.0f, 350, 2.0f);
			break;
		case 1850:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 100.0f, SCREEN_HEIGHT + ENEMY_SIZE * 2, 0.0f), 2, 0.0f, 300, 2.0f);
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 100.0f, SCREEN_HEIGHT + ENEMY_SIZE * 2, 0.0f), 2, 0.0f, 300, 2.0f);
			break;
		case 1900:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200.0f, SCREEN_HEIGHT + ENEMY_SIZE * 2, 0.0f), 1, 0.0f, 250, 2.0f);
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 200.0f, SCREEN_HEIGHT + ENEMY_SIZE * 2, 0.0f), 1, 0.0f, 250, 2.0f);
			break;
		case 1950:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300.0f, SCREEN_HEIGHT + ENEMY_SIZE * 2, 0.0f), 0, 0.0f, 200, 2.0f);
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300.0f, SCREEN_HEIGHT + ENEMY_SIZE * 2, 0.0f), 0, 0.0f, 200, 2.0f);
			break;
		case 2200:
			CEnemy::Create(D3DXVECTOR3(-ENEMY_SIZE * 2, 150.0f, 0.0f), 0, -0.5f, 300, 2.0f);
			break;
		case 2300:
			CEnemy::Create(D3DXVECTOR3(-ENEMY_SIZE * 2, 150.0f, 0.0f), 0, -0.5f, 300, 2.0f);
			break;
		case 2400:
			CEnemy::Create(D3DXVECTOR3(-ENEMY_SIZE * 2, 150.0f, 0.0f), 0, -0.5f, 300, 2.0f);
			break;
		case 2500:
			CEnemy::Create(D3DXVECTOR3(-ENEMY_SIZE * 2, 150.0f, 0.0f), 0, -0.5f, 300, 2.0f);
			break;
		case 2800:
			CEnemy::Create(D3DXVECTOR3(400.0f, -ENEMY_SIZE * 2, 0.0f), 2, -0.7f, 250, 1.4f);
			CEnemy::Create(D3DXVECTOR3(400.0f, -ENEMY_SIZE * 2, 0.0f), 2, -0.9f, 250, 1.4f);
			CEnemy::Create(D3DXVECTOR3(400.0f, -ENEMY_SIZE * 2, 0.0f), 2, 0.9f, 250, 1.4f);
			CEnemy::Create(D3DXVECTOR3(400.0f, -ENEMY_SIZE * 2, 0.0f), 2, 0.7f, 250, 1.4f);
			break;
		case 3000:
			CEnemy::Create(D3DXVECTOR3(-ENEMY_SIZE * 2, 300.0f, 0.0f), 1, -0.4f, 400, 1.6f);
			break;
		case 3050:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2, 300.0f, 0.0f), 0, 0.4f, 360, 1.6f);
			break;
		case 3100:
			CEnemy::Create(D3DXVECTOR3(-ENEMY_SIZE * 2, 300.0f, 0.0f), 0, -0.4f, 280, 1.6f);
			break;
		case 3150:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2, 300.0f, 0.0f), 1, 0.4f, 240, 1.6f);
			break;
		case 3200:
			CEnemy::Create(D3DXVECTOR3(-ENEMY_SIZE * 2, 300.0f, 0.0f), 1, -0.4f, 160, 1.4f);
			break;
		case 3250:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2, 300.0f, 0.0f), 0, 0.4f, 120, 1.6f);
			break;
		case 3500:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2, 100.0f, 0.0f), 0, 0.5f, 460, 1.6f);
			break;
		case 3550:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2, 200.0f, 0.0f), 1, 0.5f, 410, 1.8f);
			break;
		case 3600:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2, 300.0f, 0.0f), 2, 0.5f, 360, 2.0f);
			break;
		case 3650:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2, 400.0f, 0.0f), 3, 0.5f, 310, 2.35f);
			break;
		case 3900:
			CEnemy::Create(D3DXVECTOR3(400.0f, -ENEMY_SIZE * 2, 0.0f), 0, -0.75f, 260, 1.8f);
			break;
		case 3950:
			CEnemy::Create(D3DXVECTOR3(880.0f, -ENEMY_SIZE * 2, 0.0f), 0, 0.75f, 260, 1.8f);
			break;
		case 4000:
			CEnemy::Create(D3DXVECTOR3(400.0f, -ENEMY_SIZE * 2, 0.0f), 0, -0.75f, 260, 1.8f);
			break;
		case 4050:
			CEnemy::Create(D3DXVECTOR3(880.0f, -ENEMY_SIZE * 2, 0.0f), 0, 0.75f, 260, 1.8f);
			break;
		case 4300:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2, 150.0f, 0.0f), 3, 0.4f, 200, 1.8f);
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2, 200.0f, 0.0f), 3, 0.5f, 250, 1.8f);
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2, 250.0f, 0.0f), 3, 0.6f, 300, 1.8f);
			break;
		case 4500:
			CEnemy::Create(D3DXVECTOR3(800.0f, SCREEN_HEIGHT + ENEMY_SIZE * 2, 0.0f), 1, 0.0f, 260, 2.0f);
			break;
		case 4550:
			CEnemy::Create(D3DXVECTOR3(800.0f, SCREEN_HEIGHT + ENEMY_SIZE * 2, 0.0f), 1, 0.0f, 210, 2.0f);
			break;
		case 4600:
			CEnemy::Create(D3DXVECTOR3(800.0f, SCREEN_HEIGHT + ENEMY_SIZE * 2, 0.0f), 1, 0.0f, 160, 2.0f);
			break;
		case 4700:
			CEnemy::Create(D3DXVECTOR3(480.0f, SCREEN_HEIGHT + ENEMY_SIZE * 2, 0.0f), 0, 0.0f, 260, 2.0f);
			break;
		case 4750:
			CEnemy::Create(D3DXVECTOR3(480.0f, SCREEN_HEIGHT + ENEMY_SIZE * 2, 0.0f), 0, 0.0f, 210, 2.0f);
			break;
		case 4800:
			CEnemy::Create(D3DXVECTOR3(480.0f, SCREEN_HEIGHT + ENEMY_SIZE * 2, 0.0f), 0, 0.0f, 160, 2.0f);
			break;
		case 5000:
			CEnemy::Create(D3DXVECTOR3(540.0f, -ENEMY_SIZE * 2, 0.0f), 0, 1.0f, 200, 1.2f);
			CEnemy::Create(D3DXVECTOR3(740.0f, -ENEMY_SIZE * 2, 0.0f), 1, 1.0f, 200, 1.2f);
			break;
		case 5050:
			CEnemy::Create(D3DXVECTOR3(540.0f, -ENEMY_SIZE * 2, 0.0f), 2, 0.9f, 200, 1.2f);
			CEnemy::Create(D3DXVECTOR3(740.0f, -ENEMY_SIZE * 2, 0.0f), 3, -0.9f, 200, 1.2f);
			break;
		case 5200:
			CEnemy::Create(D3DXVECTOR3(-ENEMY_SIZE * 2, 600.0f, 0.0f), 3, -0.3f, 300, 2.0f);
			break;
		case 5250:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2, 600.0f, 0.0f), 3, 0.3f, 300, 2.0f);
			break;
		case 5300:
			CEnemy::Create(D3DXVECTOR3(-ENEMY_SIZE * 2, 600.0f, 0.0f), 2, -0.3f, 300, 2.0f);
			break;
		case 5350:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2, 600.0f, 0.0f), 2, 0.3f, 300, 2.0f);
			break;
		case 5400:
			CEnemy::Create(D3DXVECTOR3(-ENEMY_SIZE * 2, 600.0f, 0.0f), 1, -0.3f, 300, 2.0f);
			break;
		case 5450:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2, 600.0f, 0.0f), 1, 0.3f, 300, 2.0f);
			break;
		case 5500:
			CEnemy::Create(D3DXVECTOR3(-ENEMY_SIZE * 2, 600.0f, 0.0f), 0, -0.3f, 300, 2.0f);
			break;
		case 5550:
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH + ENEMY_SIZE * 2, 600.0f, 0.0f), 0, 0.3f, 300, 2.0f);
			break;
		case 5800:
			CEnemy::Create(D3DXVECTOR3(800.0f, -ENEMY_SIZE * 2, 0.0f), 3, 1.0f, 300, 1.3f);
			break;
		case 5850:
			CEnemy::Create(D3DXVECTOR3(700.0f, -ENEMY_SIZE * 2, 0.0f), 3, 1.0f, 300, 1.3f);
			CEnemy::Create(D3DXVECTOR3(900.0f, -ENEMY_SIZE * 2, 0.0f), 3, 1.0f, 300, 1.3f);
			break;
		case 6100:
			CEnemy::Create(D3DXVECTOR3(-ENEMY_SIZE * 2, 150.0f, 0.0f), 2, -0.4f, 200, 1.8f);
			CEnemy::Create(D3DXVECTOR3(-ENEMY_SIZE * 2, 200.0f, 0.0f), 2, -0.5f, 220, 1.8f);
			CEnemy::Create(D3DXVECTOR3(-ENEMY_SIZE * 2, 250.0f, 0.0f), 2, -0.6f, 240, 1.8f);
			break;
		case 6300:
			CEnemy::Create(D3DXVECTOR3(640.0f, -ENEMY_SIZE * 2, 0.0f), 3, 1.0f, 300, 1.0f);
			break;
		case 6350:
			CEnemy::Create(D3DXVECTOR3(540.0f, -ENEMY_SIZE * 2, 0.0f), 2, 0.9f, 270, 1.0f);
			CEnemy::Create(D3DXVECTOR3(740.0f, -ENEMY_SIZE * 2, 0.0f), 2, -0.9f, 270, 1.0f);
			break;
		case 6400:
			CEnemy::Create(D3DXVECTOR3(440.0f, -ENEMY_SIZE * 2, 0.0f), 1, 0.8f, 240, 1.0f);
			CEnemy::Create(D3DXVECTOR3(840.0f, -ENEMY_SIZE * 2, 0.0f), 1, -0.8f, 240, 1.0f);
			break;
		case 6450:
			CEnemy::Create(D3DXVECTOR3(340.0f, -ENEMY_SIZE * 2, 0.0f), 0, 0.7f, 210, 1.0f);
			CEnemy::Create(D3DXVECTOR3(940.0f, -ENEMY_SIZE * 2, 0.0f), 0, -0.7f, 210, 1.0f);
			break;
		case 7200: //ボス登場
			CSound::Stop(CSound::SOUND_LABEL_GAMEBGM);
			CSound::Play(CSound::SOUND_LABEL_BOSSBGM);
			CBoss::Create();
		default:
			break;
		}
	}
}

//描画
void CGame::Draw(void)
{

}