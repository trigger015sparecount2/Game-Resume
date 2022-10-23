//=============================================================================
//ゲーム管理[Game.cpp]
// Author : 三上航世
//=============================================================================
//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#if 1
#include "game.h"
#include "pauseui.h"
#include "player.h"
#include "enemy.h"
#include "ui.h"
#include "keyboard.h"
#include "gamepad.h"
#include "sky.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "startui.h"
#include "object.h"

#include "field.h"
#include "wall.h"
#include "light.h"

#include "load.h"
#include "sound.h"

#endif
//*****************************************************************************
//静的
//*****************************************************************************
bool CGame::m_bCountFlag = false;
int CGame::m_SelectNum = 1;
int CGame::m_nRound = 1;
bool CGame::m_bRoundChange = false;

//*****************************************************************************
//マクロ
//*****************************************************************************
//#define GAME_FILE "data/stage.txt"

#if 1
//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CGame::CGame(PRIORITY Priority) : CScene::CScene(Priority)
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CGame::~CGame()
{

}
#endif

//*****************************************************************************
// 初期化
//***************************************************************************** 
HRESULT CGame::Init(D3DXVECTOR3 /*pos*/)
{
	CEnemy::Load("data/FILES/motion_bigshadow.txt");
	//CLoad::Load(GAME_FILE);

	
	//UI作成
	CUI::Create(D3DXVECTOR3(1200.0f, 620.0f, 0.0), 25.0f, 60.0f, 1, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));		//HP
	CUI::Create(D3DXVECTOR3(1037.5f, 620.0f, 0.0), 300.0f, 60.0f, -1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	//HPゲージ下地
	if (CManager::GetGamepad() != NULL)
	{
		CUI::Create(D3DXVECTOR3(120.0f, 550.0f, 0.0), 200.0f, 300.0f, 4, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	//操作方法ゲームパッド
	}
	else
	{
		CUI::Create(D3DXVECTOR3(120.0f, 550.0f, 0.0), 200.0f, 300.0f, 3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	//操作方法キーボード
	}
	CPauseUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, -1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f));
	CPauseUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 300.0f, 400.0f, 8, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	
	CSound::Play(CSound::SOUND_LABEL_GAMEBGM);
	CSky::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0001f, CPlane::GetTexture(2));
	CLight::Create(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), 0);
	CLight::Create(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1);
	CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data/FILES/motion_DualPlayer.txt");
	CEnemy::Create(D3DXVECTOR3(-300.0f, 0.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CEnemy::Create(D3DXVECTOR3(300.0f, 0.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CWall::Create(D3DXVECTOR3(1500.0f, 400.0f, 0.0f), D3DXVECTOR3(0.0f, 400.0f, 2000.0f), 0, D3DXVECTOR2(24.0f, 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));					//奥
	CWall::Create(D3DXVECTOR3(1500.0f, 400.0f, 0.0f), D3DXVECTOR3(0.0f, 400.0f, -2000.0f), 0, D3DXVECTOR2(24.0f, 3.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));				//手前
	CWall::Create(D3DXVECTOR3(800.0f, 400.0f, 0.0f), D3DXVECTOR3(3000.0f, 400.0f, 0.0f), 0, D3DXVECTOR2(10.0f, 3.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));			//右
	CWall::Create(D3DXVECTOR3(800.0f, 400.0f, 0.0f), D3DXVECTOR3(-3000.0f, 400.0f, 0.0f), 0, D3DXVECTOR2(10.0f, 3.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));		//左
	CWall::Create(D3DXVECTOR3(1030.0f, 400.0f, 0.0f), D3DXVECTOR3(2250.0f, 400.0f, 1350.0f), 0, D3DXVECTOR2(10.0f, 3.0f), D3DXVECTOR3(0.0f, 0.7f, 0.0f));				//右奥
	CWall::Create(D3DXVECTOR3(1030.0f, 400.0f, 0.0f), D3DXVECTOR3(-2250.0f, 400.0f, 1350.0f), 0, D3DXVECTOR2(10.0f, 3.0f), D3DXVECTOR3(0.0f, -0.7f, 0.0f));				//左奥
	CWall::Create(D3DXVECTOR3(1030.0f, 400.0f, 0.0f), D3DXVECTOR3(-2250.0f, 400.0f, -1350.0f), 0, D3DXVECTOR2(10.0f, 3.0f), D3DXVECTOR3(0.0f, -D3DX_PI + 0.7f, 0.0f));	//左手前
	CWall::Create(D3DXVECTOR3(1030.0f, 400.0f, 0.0f), D3DXVECTOR3(2250.0f, 400.0f, -1350.0f), 0, D3DXVECTOR2(10.0f, 3.0f), D3DXVECTOR3(0.0f, D3DX_PI - 0.7f, 0.0f));	//右手前
	CField::Create(D3DXVECTOR3(3000.0f, 0.0f, 2000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, D3DXVECTOR2(40.0f, 40.0f), true);
	CStartUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f - 100.0f, 0.0f), SCREEN_WIDTH, 350.0f, 15, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0);
	CManager::SetCountdown(true);
	CManager::SetGameClear(false);
	CManager::SetGameEnd(false);
	CManager::SetEnd(false);
	m_nRound = 1;
	return S_OK;
}

//*****************************************************************************
// 終了
//***************************************************************************** 
void CGame::Uninit()
{
	CEnemy::UnLoad();
	Release();
}

//*****************************************************************************
// 更新
//***************************************************************************** 
void CGame::Update()
{
	if (CEnemy::GetEnemies() <= 0 && m_nRound < 9)
	{
		m_nRound++;
		switch (m_nRound)
		{
		case 0:
			//CEnemy::Create(D3DXVECTOR3(30.0f, 0.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			break;
		case 1:
			//CEnemy::Create(D3DXVECTOR3(30.0f, 0.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			break;
		case 2:
			CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(300.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(-300.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			break;
		case 3:
			CEnemy::Create(D3DXVECTOR3(-600.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(600.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(-600.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(600.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			break;
		case 4:
			CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(-500.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(500.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(-200.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(200.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			break;
		case 5:
			CEnemy::Create(D3DXVECTOR3(1000.0f, 0.0f, 1200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(-1000.0f, 0.0f, 1200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(700.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(-700.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(200.0f, 0.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(-200.0f, 0.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			break;
		case 6:
			CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, 1300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(400.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(400.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(-400.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(400.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(30.0f, 0.0f, -1300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			break;
		case 7:
			CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, 1500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(-400.0f, 0.0f, 7000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(400.0f, 0.0f, 700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(-1000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(1000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(-400.0f, 0.0f, -700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(400.0f, 0.0f, -700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, -1500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			break;
		case 8:
			CEnemy::Create(D3DXVECTOR3(1000.0f, 0.0f, 1700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(-1000.0f, 0.0f, 1700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(1000.0f, 0.0f, -1700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(-1000.0f, 0.0f, -1700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(2500.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(2500.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(-2500.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(-2500.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(1850.0f, 0.0f, 1550.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(2650.0f, 0.0f, 1050.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(-1850.0f, 0.0f, 1550.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(-2650.0f, 0.0f, 1050.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(1850.0f, 0.0f, -1550.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(2650.0f, 0.0f, -1050.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(-1850.0f, 0.0f, -1550.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			CEnemy::Create(D3DXVECTOR3(-2650.0f, 0.0f, -1050.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			break;
		case 9:
			CManager::SetGameClear(true);
			m_nRound++;
			break;
		default:
			break;
		}
		if (m_nRound < 9)
		{
			m_bRoundChange = true;
		}
	}
}

//*****************************************************************************
//描画
//***************************************************************************** 
void CGame::Draw()
{

}

//*****************************************************************************
//作成
//***************************************************************************** 
CGame *CGame::Create()
{
	CGame *pGame = NULL;
	pGame = new CGame(PRIORITY_ORBIT);		//メモリ確保
	//NULLチェック
	if (pGame != NULL)
	{
		pGame->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	return pGame;
}

int CGame::GetRound()
{
	m_bRoundChange = false;
	return m_nRound;
}