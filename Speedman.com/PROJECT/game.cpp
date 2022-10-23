//=============================================================================
//ゲーム管理[Game.cpp]
// Author : satou syusuke
//=============================================================================
//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#if 1
#include "game.h"
#include "player.h"
#include "ui.h"
#include "keyboard.h"
#include "gamepad.h"
#include "sky.h"
#include "straight3d.h"

#include "fade.h"
#include "load.h"
#include "sound.h"

#include "effect_ui.h"
#include "select_ui.h"
#include "change_ui.h"
#include "score.h"

#endif
//*****************************************************************************
//静的
//*****************************************************************************
bool CGame::m_bFade = NULL;
bool CGame::m_bCountFlag = false;
int CGame::m_SelectNum = 1;

//*****************************************************************************
//マクロ
//*****************************************************************************
#define MAX_GAMESERECT (4)
#define GAME_FILE "data/stage.txt"

#if 1
//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CGame::CGame(int nPriority) : CScene::CScene(nPriority)
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
	m_pKeyboard = CManager::GetKeyboard();
	m_pGamePad = CManager::GetGamepad();

	CLoad::Load(GAME_FILE);

	//UI作成
	CUI::Create(D3DXVECTOR3(50.0f, 50.0f, 0.0), 60.0f, 60.0f, 20);		//時計
	CUI::Create(D3DXVECTOR3(1120.0f, 50.0f, 0.0), 60.0f, 60.0f, 21);	//ハート

	//カウントダウンリセット
	CChangeUI::SetCountDown();

	//カウントダウンUI
	//こいつをコメントアウトすればカウントダウンが始まらなくなって、時間経過もしなくなります
	CChangeUI::Create(
		D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f),
		D3DXVECTOR2(200.0f, 200.0f),
		D3DXVECTOR2(-1.0f, -1.0f),
		24,
		D3DXCOLOR(255, 255, 255, 255),
		D3DXCOLOR(0, 0, 0, -3),
		CChangeUI::CHANGE_COUNTDOWN,
		60,
		D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(0.0f, 0.0f));

	////エフェクト配置するUI(テスト)
	//CEffectUI::Create(D3DXVECTOR3(380.0f, 330.0f, 0), 1, 1, 2, CEffectUI::EFFECT_FIRE);

	//番号
	CScore::Create(D3DXVECTOR3(170.0f, 50.0f, 0.0), 30.0f, 60.0f, 2, 3, CScore::NUMBER_TIME);
	CScore::Create(D3DXVECTOR3(1220.0f, 50.0f, 0.0), 30.0f, 60.0f, 2, 2, CScore::NUMBER_LIFE);

	//フェード
	CFade::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), SCREEN_WIDTH, SCREEN_HEIGHT, 1);

	//CSound::Play(CSound::SOUND_LABEL_GAMEBGM);
	//CSound::Play(CSound::SOUND_LABEL_COUNTDOWN);


	//カウントダウン終ったかフラグリセット
	m_bCountFlag = false;
	CManager::SetPause(false);
	m_SelectNum = 1;
	m_bFade = false;

	CSound::Play(CSound::SOUND_LABEL_COUNTDOWN);
	CSound::ControlVoice(CSound::SOUND_LABEL_COUNTDOWN, 3);

	return S_OK;
}

//*****************************************************************************
// 終了
//***************************************************************************** 
void CGame::Uninit()
{
	Release();
}

//*****************************************************************************
// 更新
//***************************************************************************** 
void CGame::Update()
{
	CFade::FADE Fade = CFade::GetFade();
	if (Fade == CFade::FADE_NONE)
	{

		bool GameClear = CManager::GetGameClear();
		bool bPause = CManager::GetPause();
		if (m_pGamePad != NULL)
		{
			float CrossKey = m_pGamePad->TriggerCrossKey();
			if (m_pGamePad->GetButton(CGamePad::DIP_A) == true && GameClear == true)
			{
				CFade::SetFade(CManager::MODE_RESULT);
				m_bFade = true;
			}

			//Pause状態の切り替え
			if (m_pGamePad->GetButton(CGamePad::DIP_START) == true && GameClear == false && bPause == false)
			{
				CManager::SetPause(true);

				//半透明ポリゴンくん
				CChangeUI::Create(
					D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
					D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT),
					D3DXVECTOR2(0.0f, 0.0f),
					1,
					D3DXCOLOR(255, 255, 255, 100),
					D3DXCOLOR(0, 0, 0, 0),
					CChangeUI::CHANGE_PAUSE,
					10,
					D3DXVECTOR2(0.0f, 0.0f),
					D3DXVECTOR2(0.0f, 0.0f));

				//ポーズ中のみのUIの設置
				CChangeUI::Create(
					D3DXVECTOR3(860.0f, 490.0f, 0),
					D3DXVECTOR2(890.0f, 1000.0f),
					D3DXVECTOR2(0.0f, 0.0f),
					34,
					D3DXCOLOR(255, 255, 255, 255),
					D3DXCOLOR(0, 0, 0, 0),
					CChangeUI::CHANGE_PAUSE,
					10,
					D3DXVECTOR2(0.0f, 0.0f),
					D3DXVECTOR2(0.0f, 0.0f));

				//Pause
				CChangeUI::Create(
					D3DXVECTOR3(590.0f, 220.0f, 0),
					D3DXVECTOR2(260.0f, 80.0f),
					D3DXVECTOR2(0.0f, 0.0f),
					33,
					D3DXCOLOR(255, 255, 255, 255),
					D3DXCOLOR(0, 0, 0, 0),
					CChangeUI::CHANGE_PAUSE,
					10,
					D3DXVECTOR2(0.0f, 0.0f),
					D3DXVECTOR2(0.0f, 0.0f));

				//選択式UIの設置
				CSelectUI::Create(D3DXVECTOR3(1000.0f, 430.0f, 0.0f), 225.0f, 60.0f, 31, 1, CSelectUI::SELECT_UI_NOMAL);
				CSelectUI::Create(D3DXVECTOR3(1000.0f, 530.0f, 0.0f), 220.0f, 65.0f, 30, 2, CSelectUI::SELECT_UI_NOMAL);
				CSelectUI::Create(D3DXVECTOR3(1000.0f, 620.0f, 0.0f), 210.0f, 60.0f, 32, 3, CSelectUI::SELECT_UI_NOMAL);

				CSound::Play(CSound::SOUND_LABEL_PAUSE);
				m_SelectNum = 1;
			}
			//Pause解除
			if (m_pGamePad->GetButton(CGamePad::DIP_START) == true && GameClear == false && bPause == true)
			{
				CManager::SetPause(false);
				CSound::Play(CSound::SOUND_LABEL_PAUSE);
			}

			//Pause中の処理
			if (bPause == true)
			{
				if (m_bFade == false)
				{
					if (CrossKey == 0.0f)
					{
						m_SelectNum--;

						if (m_SelectNum <= 0)
						{
							m_SelectNum = MAX_GAMESERECT - 1;
						}
						CSound::Play(CSound::SOUND_LABEL_SELECT);
					}
					else if (CrossKey == 18000.0f)
					{
						m_SelectNum++;
						if (m_SelectNum >= MAX_GAMESERECT)
						{
							m_SelectNum = 1;
						}
						CSound::Play(CSound::SOUND_LABEL_SELECT);
					}
					//決定
					if (m_pGamePad->GetButton(CGamePad::DIP_A) == true && m_SelectNum == 1)
					{
						CManager::SetPause(false);
					}
					else if (m_pGamePad->GetButton(CGamePad::DIP_A) == true && m_SelectNum == 2)
					{
						CFade::SetFade(CManager::MODE_GAME);
						CSound::Stop(CSound::SOUND_LABEL_GAMEBGM);
						CSound::Play(CSound::SOUND_LABEL_CLICK);
						CScore::SetTime(TIME_LIMIT);
						m_bFade = true;
					}
					else if (m_pGamePad->GetButton(CGamePad::DIP_A) == true && m_SelectNum == 3)
					{
						CFade::SetFade(CManager::MODE_TITLE);
						CSound::Stop(CSound::SOUND_LABEL_GAMEBGM);
						CSound::Play(CSound::SOUND_LABEL_CLICK);
						m_bFade = true;
					}
				}
			}
		}
		else if (m_pKeyboard != NULL)
		{
			if ((m_pKeyboard->GetKey(DIK_RETURN) == true || m_pKeyboard->GetKey(DIK_SPACE) == true) && GameClear == true)
			{
				CFade::SetFade(CManager::MODE_RESULT);
				m_bFade = true;
			}

			//Pause状態の切り替え
			if ((m_pKeyboard->GetKey(DIK_ESCAPE) == true) && GameClear == false && bPause == false)
			{
				CManager::SetPause(true);

				//半透明ポリゴンくん
				CChangeUI::Create(
					D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0),
					D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT),
					D3DXVECTOR2(0.0f, 0.0f),
					1,
					D3DXCOLOR(255, 255, 255, 100),
					D3DXCOLOR(0, 0, 0, 0),
					CChangeUI::CHANGE_PAUSE,
					10,
					D3DXVECTOR2(0.0f, 0.0f),
					D3DXVECTOR2(0.0f, 0.0f));

				//ポーズ中のみのUIの設置
				CChangeUI::Create(
					D3DXVECTOR3(860.0f, 490.0f, 0),
					D3DXVECTOR2(890.0f, 1000.0f),
					D3DXVECTOR2(0.0f, 0.0f),
					34,
					D3DXCOLOR(255, 255, 255, 255),
					D3DXCOLOR(0, 0, 0, 0),
					CChangeUI::CHANGE_PAUSE,
					10,
					D3DXVECTOR2(0.0f, 0.0f),
					D3DXVECTOR2(0.0f, 0.0f));

				CChangeUI::Create(
					D3DXVECTOR3(590.0f, 220.0f, 0),
					D3DXVECTOR2(230.0f, 60.0f),
					D3DXVECTOR2(0.0f, 0.0f),
					33,
					D3DXCOLOR(255, 255, 255, 255),
					D3DXCOLOR(0, 0, 0, 0),
					CChangeUI::CHANGE_PAUSE,
					10,
					D3DXVECTOR2(0.0f, 0.0f),
					D3DXVECTOR2(0.0f, 0.0f));

				//選択式UIの設置
				CSelectUI::Create(D3DXVECTOR3(1000.0f, 430.0f, 0.0f), 220.0f, 50.0f, 31, 1, CSelectUI::SELECT_UI_NOMAL);
				CSelectUI::Create(D3DXVECTOR3(1000.0f, 530.0f, 0.0f), 220.0f, 50.0f, 30, 2, CSelectUI::SELECT_UI_NOMAL);
				CSelectUI::Create(D3DXVECTOR3(1000.0f, 620.0f, 0.0f), 220.0f, 50.0f, 32, 3, CSelectUI::SELECT_UI_NOMAL);


				m_SelectNum = 1;
			}
			//ポーズ解除
			if ((m_pKeyboard->GetKey(DIK_ESCAPE) == true) && GameClear == false && bPause == true)
			{
				CManager::SetPause(false);
				CSound::Play(CSound::SOUND_LABEL_PAUSE);
			}

			//Pause中の処理
			if (bPause == true)
			{
				if (m_bFade == false)
				{
					//選択番号の変化
					if (m_pKeyboard->GetKey(DIK_UP) == true || m_pKeyboard->GetKey(DIK_W) == true)
					{
						m_SelectNum--;

						if (m_SelectNum <= 0)
						{
							m_SelectNum = MAX_GAMESERECT - 1;
						}
						CSound::Play(CSound::SOUND_LABEL_SELECT);
					}
					else if (m_pKeyboard->GetKey(DIK_DOWN) == true || m_pKeyboard->GetKey(DIK_S) == true)
					{
						m_SelectNum++;
						if (m_SelectNum >= MAX_GAMESERECT)
						{
							m_SelectNum = 1;
						}
						CSound::Play(CSound::SOUND_LABEL_SELECT);
					}
				}

				if ((m_pKeyboard->GetKey(DIK_RETURN) == true || m_pKeyboard->GetKey(DIK_SPACE) == true) && m_SelectNum == 1)			//Pause解除
				{
					CManager::SetPause(false);
				}
				else if ((m_pKeyboard->GetKey(DIK_RETURN) == true || m_pKeyboard->GetKey(DIK_SPACE) == true) && m_SelectNum == 2)	//リスタート
				{
					CFade::SetFade(CManager::MODE_GAME);
					CSound::Stop(CSound::SOUND_LABEL_GAMEBGM);
					CSound::Play(CSound::SOUND_LABEL_CLICK);
					CScore::SetTime(TIME_LIMIT);
					m_bFade = true;
				}
				else if ((m_pKeyboard->GetKey(DIK_RETURN) == true || m_pKeyboard->GetKey(DIK_SPACE) == true) && m_SelectNum == 3)	//リスタート
				{
					CFade::SetFade(CManager::MODE_TITLE);
					CSound::Stop(CSound::SOUND_LABEL_GAMEBGM);
					CSound::Play(CSound::SOUND_LABEL_CLICK);
					m_bFade = true;
				}
			}
		}
		for (int nCnt = 0; nCnt < 5; nCnt++)
		{
			float Distanse = CIRCLE;
			CStraight3D::Create(D3DXVECTOR3(0.0f + sinf(Distanse) * 20, 600.0f, 5600 + cosf(Distanse) * 20),
				D3DXVECTOR3(5.0f, 5.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 5.0f, 0.0f),
				D3DXCOLOR(255, 255, 255, 255),
				D3DXCOLOR(0, 0, 0, 0),
				3, 30);
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
	pGame = new CGame(0);		//メモリ確保
								//NULLチェック
	if (pGame != NULL)
	{
		pGame->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	return pGame;
}

void CGame::SetCountFlag(bool bFlag)
{
	m_bCountFlag = bFlag;
}