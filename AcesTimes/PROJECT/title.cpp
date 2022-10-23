//=============================================================================
// タイトル管理[Title.cpp]
// Author : 三上航世
//=============================================================================
//*****************************************************************************
//インクルード
//*****************************************************************************
#if 1
#include "Title.h"
#include "keyboard.h"
#include "UI.h"

#include "game.h"
#include "gamepad.h"
#include "fade.h"

//#include "player.h"
//#include "light.h"

#include "sound.h"
#endif

//*****************************************************************************
//静的
//*****************************************************************************
//int CTitle::m_SerectNam = 1;

//*****************************************************************************
//マクロ
//*****************************************************************************
//#define MAX_TITLESERECT (4)

#if 1
//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CTitle::CTitle(PRIORITY Priority) : CScene(Priority)
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTitle::~CTitle()
{
}
#endif

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CTitle::Init(D3DXVECTOR3 /*pos*/)
{
	m_pKeyboard = CManager::GetKeyboard();
	m_pGamePad = CManager::GetGamepad();

	m_pUI[3] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.8f), 34, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[3]->SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(25.0f, 17.0f));
	CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 130.0f, 0.0f), D3DXVECTOR2(400.0f, 80.0f), 17, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[0] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 490.0f, 0.0f), D3DXVECTOR2(260.0f, 50.0f), 18, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[1] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 570.0f, 0.0f), D3DXVECTOR2(120.0f, 50.0f), 19, D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[2] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 650.0f, 0.0f), D3DXVECTOR2(120.0f, 50.0f), 20, D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_nSelect = 0;
	CManager::SetPause(false);
	CSound::Play(CSound::SOUND_LABEL_TITLEBGM);
	return S_OK;
}

//*****************************************************************************
// 終了
//***************************************************************************** 
void CTitle::Uninit()
{
	int nCnt;
	if (m_pGamePad != NULL)
	{
		m_pGamePad = NULL;
	}
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard = NULL;
	}
	for (nCnt = 0; nCnt < 4; nCnt++)
	{
		if (m_pUI[nCnt] != NULL)
		{
			m_pUI[nCnt] = NULL;
		}
	}
	CScene::Release();
}

//*****************************************************************************
// kousinn
//***************************************************************************** 
void CTitle::Update()
{
	if (m_pGamePad != NULL)
	{
		if (m_pGamePad->TriggerCrossKey() == 0.0f)
		{
			Select(-1);
		}
		else if(m_pGamePad->TriggerCrossKey() == 18000.0f)
		{
			Select(1);
		}
		if (m_pGamePad->GetButton(CGamePad::DIP_B) == true)
		{
			if (CFade::GetFade() == CFade::FADE_NONE)
			{
				Choice();
			}
		}
	}
	else if (m_pKeyboard != NULL)
	{
		if (m_pKeyboard->GetKey(DIK_W) == true)
		{
			Select(-1);
		}
		else if (m_pKeyboard->GetKey(DIK_S) == true)
		{
			Select(1);
		}
		if (m_pKeyboard->GetKey(DIK_J) == true)
		{
			if (CFade::GetFade() == CFade::FADE_NONE)
			{
				Choice();
			}
		}
	}
}

//*****************************************************************************
//描画
//***************************************************************************** 
void CTitle::Draw()
{

}

//*****************************************************************************
//描画
//***************************************************************************** 
CTitle *CTitle::Create()
{
	CTitle *pTitle = NULL;
	pTitle = new CTitle(PRIORITY_EFFECT);		//メモリ確保
	//NULLチェック
	if (pTitle != NULL)
	{
		pTitle->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	return pTitle;
}

void CTitle::Select(int nAdd)
{
	m_pUI[m_nSelect]->ColorChange(D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
	m_nSelect += nAdd;
	if (m_nSelect < 0)
	{
		m_nSelect = 2;
	}
	else if (m_nSelect > 2)
	{
		m_nSelect = 0;
	}
	m_pUI[m_nSelect]->ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CSound::Play(CSound::SOUND_LABEL_CHOICE);
}

void CTitle::Choice()
{
	switch (m_nSelect)
	{
	case 0:
		CFade::SetFade(CManager::MODE_SELECT);
		break;
	case 1:
		CFade::SetFade(CManager::MODE_LOAD);
		break;
	case 2:
		CFade::SetFade(CManager::MODE_SAVE);
		break;
	default:
		break;
	}
	CSound::Play(CSound::SOUND_LABEL_YES);
}