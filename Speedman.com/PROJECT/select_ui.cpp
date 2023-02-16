//*****************************************************************************
// UI（選択式）関連処理(Select_UI.cpp)
// Author : 佐藤秀亮
//
//UIクラスが肥大化したため処理によるクラス分けを行いました
//*****************************************************************************
#include "Select_UI.h"

#include "Title.h"
#include "Game.h"
#include "GameOver.h"
#include "tutorial.h"

#include "keyboard.h"
#include "Fade.h"
#include "Player.h"


//*****************************************************************************
//静的メンバ
//*****************************************************************************
int CSelectUI::m_nPauseSelect = 1;
bool CSelectUI::m_bFade = false;
CKeyboard *CSelectUI::m_pKeyboard = NULL;

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CSelectUI::CSelectUI(int nPriority) : CUI(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_UINum = -1;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CSelectUI::~CSelectUI()
{

}

//*****************************************************************************
//初期化
//*****************************************************************************
HRESULT CSelectUI::Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, int SelectNum, SELECT_UI_TYPE UIType)
{
	CUI::Init(pos, SizeX, SizeY, nType);
	CScene2D::SetObjType(CScene::OBJECTTYPE_SERECTUI);

	m_pos = pos;
	m_UINum = SelectNum;

	m_nType = UIType;

	m_fSizeX = SizeX;
	m_fSizeY = SizeY;

	m_pKeyboard = CManager::GetKeyboard();
	m_bUninit = false;
	return S_OK;
}

//*****************************************************************************
//終了
//*****************************************************************************
void CSelectUI::Uninit()
{
	CUI::Uninit();
}

//*****************************************************************************
//更新
//*****************************************************************************
void CSelectUI::Update()
{
	CManager::MODE Mode = CManager::GetMode();
	bool bPause = CManager::GetPause();
	int nControl = CPlayer::GetControlNum();

	switch (m_nType)
	{
	case(CSelectUI::SELECT_UI_NOMAL):
		switch (Mode)
		{
		case(CManager::MODE_TITLE):	//タイトル
			CTitle *pTitle;
			pTitle = CManager::GetTitle();
			if (pTitle != NULL)
			{
				int nSerectNum = ((CTitle*)pTitle)->GetSerectNum();	//セレクト番号取得

				//選んでるほう明るくする
				if (m_UINum == nSerectNum)
				{
					m_color.r = 255;
					m_color.g = 255;
					m_color.b = 255;
					m_color.a = 255;

					CScene2D::ColorChange(m_color);

					CScene2D::SetWidth(m_fSizeX + 20);
					CScene2D::SetHeight(m_fSizeY + 10);

				}
				//選んでないほう暗くする
				else
				{
					m_color.r = 100;
					m_color.g = 100;
					m_color.b = 100;
					m_color.a = 100;

					CScene2D::ColorChange(m_color);
					CScene2D::SetWidth(m_fSizeX);
					CScene2D::SetHeight(m_fSizeY);
				}
			}
			break;

		case(CManager::MODE_GAME):	//ゲーム
			CGame *pGame;
			pGame = CManager::GetGame();
			if (pGame != NULL)
			{
				int nSerectNum = ((CGame*)pGame)->GetSelectNum();	//セレクト番号取得
				
				//選んでるほう明るくする
				if (m_UINum == nSerectNum)
				{
					m_color.r = 255;
					m_color.g = 255;
					m_color.b = 255;
					m_color.a = 255;

					CScene2D::ColorChange(m_color);

					CScene2D::SetWidth(m_fSizeX + 20);
					CScene2D::SetHeight(m_fSizeY + 10);

				}
				//選んでないほう暗くする
				else
				{
					m_color.r = 100;
					m_color.g = 100;
					m_color.b = 100;
					m_color.a = 100;

					CScene2D::ColorChange(m_color);
					CScene2D::SetWidth(m_fSizeX);
					CScene2D::SetHeight(m_fSizeY);
				}
			}
			if (bPause == false)
			{
				m_bUninit = true;
			}
			break;
		case(CManager::MODE_TUTORIAL):	//チュートリアル
			CTutorial *pTutorial;
			pTutorial = CManager::GetTutorial();
			if (pTutorial != NULL)
			{
				int nSerectNum = ((CTutorial*)pTutorial)->GetSelectNum();	//セレクト番号取得
				
				//選んでるほう明るくする
				if (m_UINum == nSerectNum)
				{
					m_color.r = 255;
					m_color.g = 255;
					m_color.b = 255;
					m_color.a = 255;

					CScene2D::ColorChange(m_color);

					CScene2D::SetWidth(m_fSizeX + 20);
					CScene2D::SetHeight(m_fSizeY + 10);

				}
				//選んでないほう暗くする
				else
				{
					m_color.r = 100;
					m_color.g = 100;
					m_color.b = 100;
					m_color.a = 100;

					CScene2D::ColorChange(m_color);
					CScene2D::SetWidth(m_fSizeX);
					CScene2D::SetHeight(m_fSizeY);
				}
			}
			if (bPause == false)
			{
				m_bUninit = true;
			}
			break;

		case(CManager::MODE_GAMEOVER):	//ゲームオーバー
			CGameOver *pGameOver;
			pGameOver = CManager::GetGameOver();

			if (pGameOver != NULL)
			{
				int nSerectNum = ((CGameOver*)pGameOver)->GetSerectNum();	//セレクト番号取得

				//選んでるほう明るくする
				if (m_UINum == nSerectNum)
				{
					m_color.r = 255;
					m_color.g = 255;
					m_color.b = 255;
					m_color.a = 255;

					CScene2D::ColorChange(m_color);

				}
				//選んでないほう暗くする
				else
				{
					m_color.r = 100;
					m_color.g = 100;
					m_color.b = 100;
					m_color.a = 100;

					CScene2D::ColorChange(m_color);
				}
			}
			break;
		}
		break;
	case(CSelectUI::SELECT_UI_CONTROL):
		//選んでるほう明るくする
		if (nControl == m_UINum)
		{
			m_color.r = 255;
			m_color.g = 255;
			m_color.b = 255;
			m_color.a = 255;
		}
		//選んでないほう暗くする
		else if (nControl != m_UINum)
		{
			m_color.r = 0;
			m_color.g = 0;
			m_color.b = 0;
			m_color.a = 0;
		}
		CScene2D::ColorChange(m_color);
		CScene2D::SetWidth(m_fSizeX);
		CScene2D::SetHeight(m_fSizeY);
		break;
	}

	if (m_bUninit == true)
	{
		SetDeath(true);
	}

}

//*****************************************************************************
//描画
//*****************************************************************************
void CSelectUI::Draw()
{
	CUI::Draw();
}

//*****************************************************************************
//作成
//*****************************************************************************
CSelectUI *CSelectUI::Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, int SelectNum, SELECT_UI_TYPE UIType)
{
	CSelectUI * pSelectUI = NULL;
	pSelectUI = new CSelectUI(5);
	if (pSelectUI != NULL)
	{
		pSelectUI->Init(pos, SizeX, SizeY, nType, SelectNum, UIType);
	}
	return pSelectUI;
}