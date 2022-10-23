//=============================================================================
// セレクト管理[select.cpp]
// Author : 三上航世
//=============================================================================
//*****************************************************************************
//インクルード
//*****************************************************************************
#if 1
#include "select.h"
#include "keyboard.h"
#include "ui.h"
#include "renderer.h"

#include "game.h"
#include "gamepad.h"
#include "fade.h"

#include "sound.h"
#endif

//*****************************************************************************
//静的
//*****************************************************************************
CSelect::Aircraft CSelect::m_aAircraft[6] = {};

//*****************************************************************************
//マクロ
//*****************************************************************************


#if 1
//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CSelect::CSelect(PRIORITY Priority) : CScene(Priority)
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CSelect::~CSelect()
{
}
#endif

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CSelect::Init(D3DXVECTOR3)
{
	m_pKeyboard = CManager::GetKeyboard();
	m_pGamePad = CManager::GetGamepad();
	Load();
	RankLoad();
	CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), -1, D3DXCOLOR(0.0f, 0.3f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[0] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 65.0f, 0.0f), D3DXVECTOR2(240.0f, 50.0f), 29, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pUI[1] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 465.0f, 0.0f), D3DXVECTOR2(240.0f, 50.0f), 29, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[2] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 265.0f, 0.0f), D3DXVECTOR2(240.0f, 235.0f), 30, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[3] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 190.0f, 0.0f), D3DXVECTOR2(170.0f, 170.0f), m_aAircraft[0].nHudTexType, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	
	m_pUI[4] = CUI::Create(D3DXVECTOR3(1040.0f, 130.0f, 0.0f), D3DXVECTOR2(240.0f, 50.0f), 29, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pUI[5] = CUI::Create(D3DXVECTOR3(1040.0f, 400.0f, 0.0f), D3DXVECTOR2(240.0f, 50.0f), 29, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[6] = CUI::Create(D3DXVECTOR3(1040.0f, 265.0f, 0.0f), D3DXVECTOR2(240.0f, 145.0f), 30, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[7] = CUI::Create(D3DXVECTOR3(1040.0f, 265.0f, 0.0f), D3DXVECTOR2(170.0f, 170.0f), m_aAircraft[1].nHudTexType, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_pUI[8] = CUI::Create(D3DXVECTOR3(1440.0f, 130.0f, 0.0f), D3DXVECTOR2(240.0f, 50.0f), 29, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pUI[9] = CUI::Create(D3DXVECTOR3(1440.0f, 400.0f, 0.0f), D3DXVECTOR2(240.0f, 50.0f), 29, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[10] = CUI::Create(D3DXVECTOR3(1440.0f, 265.0f, 0.0f), D3DXVECTOR2(240.0f, 145.0f), 30, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[11] = CUI::Create(D3DXVECTOR3(1440.0f, 265.0f, 0.0f), D3DXVECTOR2(170.0f, 170.0f), m_aAircraft[2].nHudTexType, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_pUI[12] = CUI::Create(D3DXVECTOR3(-560.0f, 130.0f, 0.0f), D3DXVECTOR2(240.0f, 50.0f), 29, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pUI[13] = CUI::Create(D3DXVECTOR3(-560.0f, 400.0f, 0.0f), D3DXVECTOR2(240.0f, 50.0f), 29, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[14] = CUI::Create(D3DXVECTOR3(-560.0f, 265.0f, 0.0f), D3DXVECTOR2(240.0f, 145.0f), 30, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[15] = CUI::Create(D3DXVECTOR3(-560.0f, 265.0f, 0.0f), D3DXVECTOR2(170.0f, 170.0f), m_aAircraft[3].nHudTexType, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_pUI[16] = CUI::Create(D3DXVECTOR3(-160.0f, 130.0f, 0.0f), D3DXVECTOR2(240.0f, 50.0f), 29, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pUI[17] = CUI::Create(D3DXVECTOR3(-160.0f, 400.0f, 0.0f), D3DXVECTOR2(240.0f, 50.0f), 29, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[18] = CUI::Create(D3DXVECTOR3(-160.0f, 265.0f, 0.0f), D3DXVECTOR2(240.0f, 145.0f), 30, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[19] = CUI::Create(D3DXVECTOR3(-160.0f, 265.0f, 0.0f), D3DXVECTOR2(170.0f, 170.0f), m_aAircraft[4].nHudTexType, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	
	m_pUI[20] = CUI::Create(D3DXVECTOR3(240.0f, 130.0f, 0.0f), D3DXVECTOR2(240.0f, 50.0f), 29, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pUI[21] = CUI::Create(D3DXVECTOR3(240.0f, 400.0f, 0.0f), D3DXVECTOR2(240.0f, 50.0f), 29, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[22] = CUI::Create(D3DXVECTOR3(240.0f, 265.0f, 0.0f), D3DXVECTOR2(240.0f, 145.0f), 30, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[23] = CUI::Create(D3DXVECTOR3(240.0f, 265.0f, 0.0f), D3DXVECTOR2(170.0f, 170.0f), m_aAircraft[5].nHudTexType, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_pUI[24] = CUI::Create(D3DXVECTOR3(600.0f, 337.0f, 0.0f), D3DXVECTOR2(0.0f, 13.0f), 32, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[25] = CUI::Create(D3DXVECTOR3(600.0f, 363.0f, 0.0f), D3DXVECTOR2(0.0f, 13.0f), 32, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[26] = CUI::Create(D3DXVECTOR3(600.0f, 389.0f, 0.0f), D3DXVECTOR2(0.0f, 13.0f), 32, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[27] = CUI::Create(D3DXVECTOR3(600.0f, 415.0f, 0.0f), D3DXVECTOR2(0.0f, 13.0f), 32, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[28] = CUI::Create(D3DXVECTOR3(600.0f, 441.0f, 0.0f), D3DXVECTOR2(0.0f, 13.0f), 32, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_pUI[29] = CUI::Create(D3DXVECTOR3(440.0f, 270.0f, 0.0f), D3DXVECTOR2(40.0f, 60.0f), 31, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[30] = CUI::Create(D3DXVECTOR3(840.0f, 270.0f, 0.0f), D3DXVECTOR2(40.0f, 60.0f), 31, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	m_pStageUI[0] = CUI::Create(D3DXVECTOR3(240.0f, 65.0f, 0.0f), D3DXVECTOR2(240.0f, 50.0f), 29, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pStageUI[1] = CUI::Create(D3DXVECTOR3(240.0f, 465.0f, 0.0f), D3DXVECTOR2(240.0f, 50.0f), 29, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pStageUI[2] = CUI::Create(D3DXVECTOR3(240.0f, 265.0f, 0.0f), D3DXVECTOR2(240.0f, 235.0f), 30, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pStageUI[3] = CUI::Create(D3DXVECTOR3(240.0f, 200.0f, 0.0f), D3DXVECTOR2(170.0f, 170.0f), 33, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pStageUI[4] = CUI::Create(D3DXVECTOR3(240.0f, 400.0f, 0.0f), D3DXVECTOR2(150.0f, 60.0f), 35, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_pStageUI[5] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 65.0f, 0.0f), D3DXVECTOR2(240.0f, 50.0f), 29, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pStageUI[6] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 465.0f, 0.0f), D3DXVECTOR2(240.0f, 50.0f), 29, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pStageUI[7] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 265.0f, 0.0f), D3DXVECTOR2(240.0f, 235.0f), 30, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pStageUI[8] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 200.0f, 0.0f), D3DXVECTOR2(170.0f, 170.0f), 7, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pStageUI[9] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f), D3DXVECTOR2(200.0f, 60.0f), 36, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_pStageUI[10] = CUI::Create(D3DXVECTOR3(1040.0f, 65.0f, 0.0f), D3DXVECTOR2(240.0f, 50.0f), 29, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pStageUI[11] = CUI::Create(D3DXVECTOR3(1040.0f, 465.0f, 0.0f), D3DXVECTOR2(240.0f, 50.0f), 29, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pStageUI[12] = CUI::Create(D3DXVECTOR3(1040.0f, 265.0f, 0.0f), D3DXVECTOR2(240.0f, 235.0f), 30, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pStageUI[13] = CUI::Create(D3DXVECTOR3(1040.0f, 200.0f, 0.0f), D3DXVECTOR2(170.0f, 170.0f), 34, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pStageUI[14] = CUI::Create(D3DXVECTOR3(1040.0f, 400.0f, 0.0f), D3DXVECTOR2(150.0f, 60.0f), 37, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	CUI::Create(D3DXVECTOR3(100.0f, 620.0f, 0.0f), D3DXVECTOR2(30.0f, 140.0f), 5, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CUI::Create(D3DXVECTOR3(1180.0f, 620.0f, 0.0f), D3DXVECTOR2(30.0f, 140.0f), 5, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	CUI::Create(D3DXVECTOR3(640.0f, 620.0f, 0.0f), D3DXVECTOR2(744.0f, 140.0f), 6, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	CManager::SetPause(false);
	m_nSelect[0] = 0;
	m_nSelect[1] = 0;
	D3DXCreateFont(CManager::GetRenderer()->CRenderer::GetDevice(), 40, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "PixelMplus10", &m_pFont[0]);
	D3DXCreateFont(CManager::GetRenderer()->CRenderer::GetDevice(), 26, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "PixelMplus10", &m_pFont[1]);
	D3DXCreateFont(CManager::GetRenderer()->CRenderer::GetDevice(), 26, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "PixelMplus10", &m_pFont[2]);
	D3DXCreateFont(CManager::GetRenderer()->CRenderer::GetDevice(), 50, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "PixelMplus10", &m_pFont[3]);
	m_nClear = 0;
	m_bClear = true;
	m_nCntSelect = 0;
	m_nCntGauge = 10;
	m_fStageClear = 0.0f;
	m_Select = SELECT_NONE;
	return S_OK;
}

//*****************************************************************************
// 終了
//***************************************************************************** 
void CSelect::Uninit()
{
	int nCnt;
	for (nCnt = 0; nCnt < 4; nCnt++)
	{
		if (m_pFont[nCnt] != NULL)
		{
			m_pFont[nCnt]->Release();
			m_pFont[nCnt] = NULL;
		}
	}
	if (m_pGamePad != NULL)
	{
		m_pGamePad = NULL;
	}
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard = NULL;
	}
	for (nCnt = 0; nCnt < 15; nCnt++)
	{
		if (m_pStageUI[nCnt] != NULL)
		{
			m_pStageUI[nCnt] = NULL;
		}
	}
	for (nCnt = 0; nCnt < 31; nCnt++)
	{
		if (m_pUI[nCnt] != NULL)
		{
			m_pUI[nCnt] = NULL;
		}
	}
	CScene::Release();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CSelect::Update()
{
	if (m_nCntChange > 0)
	{
		m_nCntChange--;
		if (m_nCntChange <= 0)
		{
			Text();
			m_nCntChange = 0;
		}
	}
	if (m_bStage == true && m_fStageClear < 1.0f)
	{
		m_fStageClear += 0.1f;
		if (m_fStageClear > 1.0f)
		{
			m_fStageClear = 1.0f;
		}
		int nCntStage;
		for (nCntStage = 0; nCntStage < 15; nCntStage++)
		{
			if ((nCntStage + 2) % 5 == 0)
			{
				if (m_nSelect[1] * 5 <= nCntStage && nCntStage < (m_nSelect[1] + 1) * 5)
				{
					m_pStageUI[nCntStage]->ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fStageClear));
				}
				else
				{
					m_pStageUI[nCntStage]->ColorChange(D3DXCOLOR(0.6f, 0.6f, 0.6f, m_fStageClear));
				}
			}
			else
			{
				if (m_nSelect[1] * 5 <= nCntStage && nCntStage < (m_nSelect[1] + 1) * 5)
				{
					m_pStageUI[nCntStage]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fStageClear));
				}
				else
				{
					m_pStageUI[nCntStage]->ColorChange(D3DXCOLOR(0.0f, 0.6f, 0.0f, m_fStageClear));
				}
			}
		}
	}
	else if(m_bStage == false && m_fStageClear > 0.0f)
	{
		m_fStageClear -= 0.1f;
		if (m_fStageClear < 0.0f)
		{
			m_pUI[29]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
			m_pUI[30]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
			m_fStageClear = 0.0f;
			m_nCntGauge = 10;
		}
		int nCntStage;
		for (nCntStage = 0; nCntStage < 15; nCntStage++)
		{
			if ((nCntStage + 2) % 5 == 0)
			{
				if (m_nSelect[1] * 5 <= nCntStage && nCntStage < (m_nSelect[1] + 1) * 5)
				{
					m_pStageUI[nCntStage]->ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fStageClear));
				}
				else
				{
					m_pStageUI[nCntStage]->ColorChange(D3DXCOLOR(0.6f, 0.6f, 0.6f, m_fStageClear));
				}
			}
			else
			{
				if (m_nSelect[1] * 5 <= nCntStage && nCntStage < (m_nSelect[1] + 1) * 5)
				{
					m_pStageUI[nCntStage]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fStageClear));
				}
				else
				{
					m_pStageUI[nCntStage]->ColorChange(D3DXCOLOR(0.0f, 0.6f, 0.0f, m_fStageClear));
				}
			}
		}
	}
	if (m_nCntSelect > 0)
	{
		m_nCntSelect--;
		int nCntGroup;
		int nCntUI;
		switch (m_Select)
		{
		case SELECT_LEFT:
			for (nCntGroup = 0; nCntGroup < 6; nCntGroup++)
			{
				for (nCntUI = 0; nCntUI < 4; nCntUI++)
				{
					D3DXVECTOR3 pos = m_pUI[nCntGroup * 4 + nCntUI]->GetPos();
					D3DXVECTOR2 size = m_pUI[nCntGroup * 4 + nCntUI]->GetSize();
					pos.x += 50.0f;
					if (pos.x > 1440.0f)
					{
						pos.x -= 2400.0f;
					}
					if (nCntGroup == m_nSelect[0])
					{
						switch (nCntUI)
						{
						case 0: //四角上
							pos.y -= 9.0f;
							if (pos.y < 65.0f)
							{
								pos.y = 65.0f;
							}
							break;
						case 1: //四角下
							pos.y += 9.0f;
							if (pos.y > 465.0f)
							{
								pos.y = 465.0f;
							}
							break;
						case 2: //四角真ん中
							size.y += 12.0f;
							if (size.y > 235.0f)
							{
								size.y = 235.0f;
							}
							break;
						case 3: //HUD戦闘機
							pos.y -= 10.0f;
							if (pos.y < 190.0f)
							{
								pos.y = 190.0f;
							}
							break;
						default:
							break;
						}
					}
					else if ((m_nSelect[0] + 1) % 6 == nCntGroup)
					{
						switch (nCntUI)
						{
						case 0: //四角上
							pos.y += 9.0f;
							if (pos.y > 130.0f)
							{
								pos.y = 130.0f;
							}
							break;
						case 1: //四角下
							pos.y -= 9.0f;
							if (pos.y < 400.0f)
							{
								pos.y = 400.0f;
							}
							break;
						case 2: //四角真ん中
							size.y -= 12.0f;
							if (size.y < 145.0f)
							{
								size.y = 145.0f;
							}
							break;
						case 3: //HUD戦闘機
							pos.y += 10.0f;
							if (pos.y > 265.0f)
							{
								pos.y = 265.0f;
							}
							break;
						default:
							break;
						}
					}
					m_pUI[nCntGroup * 4 + nCntUI]->SetPos(pos);
					m_pUI[nCntGroup * 4 + nCntUI]->SetSize(size);
					m_pUI[nCntGroup * 4 + nCntUI]->Set();
				}
			}
			break;
		case SELECT_RIGHT:
			for (nCntGroup = 0; nCntGroup < 6; nCntGroup++)
			{
				for (nCntUI = 0; nCntUI < 4; nCntUI++)
				{
					D3DXVECTOR3 pos = m_pUI[nCntGroup * 4 + nCntUI]->GetPos();
					D3DXVECTOR2 size = m_pUI[nCntGroup * 4 + nCntUI]->GetSize();
					pos.x -= 50.0f;
					if (pos.x < -560.0f)
					{
						pos.x += 2400.0f;
					}
					if (nCntGroup == m_nSelect[0])
					{
						switch (nCntUI)
						{
						case 0: //四角上
							pos.y -= 9.0f;
							if (pos.y < 65.0f)
							{
								pos.y = 65.0f;
							}
							break;
						case 1: //四角下
							pos.y += 9.0f;
							if (pos.y > 465.0f)
							{
								pos.y = 465.0f;
							}
							break;
						case 2: //四角真ん中
							size.y += 12.0f;
							if (size.y > 235.0f)
							{
								size.y = 235.0f;
							}
							break;
						case 3: //HUD戦闘機
							pos.y -= 10.0f;
							if (pos.y < 190.0f)
							{
								pos.y = 190.0f;
							}
							break;
						default:
							break;
						}
					}
					else if (m_nSelect[0] == 0 && nCntGroup == 5 || m_nSelect[0] - 1 == nCntGroup)
					{
						switch (nCntUI)
						{
						case 0: //四角上
							pos.y += 9.0f;
							if (pos.y > 130.0f)
							{
								pos.y = 130.0f;
							}
							break;
						case 1: //四角下
							pos.y -= 9.0f;
							if (pos.y < 400.0f)
							{
								pos.y = 400.0f;
							}
							break;
						case 2: //四角真ん中
							size.y -= 12.0f;
							if (size.y < 145.0f)
							{
								size.y = 145.0f;
							}
							break;
						case 3: //HUD戦闘機
							pos.y += 10.0f;
							if (pos.y > 265.0f)
							{
								pos.y = 265.0f;
							}
							break;
						default:
							break;
						}
					}
					m_pUI[nCntGroup * 4 + nCntUI]->SetPos(pos);
					m_pUI[nCntGroup * 4 + nCntUI]->SetSize(size);
					m_pUI[nCntGroup * 4 + nCntUI]->Set();
				}
			}
			break;
		default:
			break;
		}
		if (m_nCntSelect <= 0)
		{
			m_nCntSelect = 0;
			m_nCntGauge = 10;
		}
	}
	if (m_nCntGauge > 0)
	{
		m_nCntGauge--;
		int nCntUI;
		for (nCntUI = 0; nCntUI < 5; nCntUI++)
		{
			D3DXVECTOR2 size = m_pUI[24 + nCntUI]->GetSize();
			switch (nCntUI)
			{
			case 0:
				size.x += 17.0f * (m_aAircraft[m_nSelect[0]].fSpeed / 10.0f);
				break;
			case 1:
				size.x += 17.0f * ((float)m_aAircraft[m_nSelect[0]].nHP / 10.0f);
				break;
			case 2:
				size.x += 17.0f * ((float)(m_aAircraft[m_nSelect[0]].ShotType + 1) / CPlayer::SHOTTYPE_MAX);
				break;
			case 3: 
				size.x += 17.0f * ((1000.0f - m_aAircraft[m_nSelect[0]].nMaxSlow) / 1000.0f);
				break;
			case 4:
				size.x += 17.0f * ((1500.0f - m_aAircraft[m_nSelect[0]].nMaxReturn) / 1500.0f);
				break;
			default:
				break;
			}
			m_pUI[24 + nCntUI]->SetHGauge(size);
		}
		if (m_nCntGauge <= 0)
		{
			m_nCntGauge = 0;
		}
	}
	if (m_pGamePad != NULL)
	{
		if (m_bStage == true)
		{
			if (m_pGamePad->TriggerCrossKey() == 27000.0f)
			{
				Select(-1);
			}
			else if (m_pGamePad->TriggerCrossKey() == 9000.0f)
			{
				Select(1);
			}
		}
		else if(m_bBuy == false)
		{
			if (m_pGamePad->PressCrossKey() == 27000.0f)
			{
				Select(-1);
			}
			else if (m_pGamePad->PressCrossKey() == 9000.0f)
			{
				Select(1);
			}
		}
		if (m_pGamePad->GetButton(CGamePad::DIP_B) == true)
		{
			if (CManager::GetAircraft(m_nSelect[0]) == true)
			{
				if (CFade::GetFade() == CFade::FADE_NONE)
				{
					if (m_bStage == true)
					{
						CManager::SetType(m_nSelect[0]);
						CManager::SetStage(m_nSelect[1]);
						CFade::SetFade(CManager::MODE_GAME);
						CSound::Play(CSound::SOUND_LABEL_YES);
					}
					else
					{
						StageSelect();
					}
				}
			}
			else
			{
				Buy();
			}
		}
		if (m_pGamePad->GetButton(CGamePad::DIP_A) == true)
		{
			if (m_bBuy == true)
			{
				m_bBuy = false;
				Text();
				CSound::Play(CSound::SOUND_LABEL_CANCEL);
			}
			else if (m_bStage == true)
			{
				StageCancel();
			}
			else if (CFade::GetFade() == CFade::FADE_NONE)
			{
				CFade::SetFade(CManager::MODE_TITLE);
				CSound::Play(CSound::SOUND_LABEL_CANCEL);
			}
		}
	}
	else if (m_pKeyboard != NULL)
	{
		if (m_bStage == true)
		{
			if (m_pKeyboard->GetKey(DIK_A) == true)
			{
				Select(-1);
			}
			else if (m_pKeyboard->GetKey(DIK_D) == true)
			{
				Select(1);
			}
		}
		else if (m_bBuy == false)
		{
			if (m_pKeyboard->GetPress(DIK_A) == true)
			{
				Select(-1);
			}
			else if (m_pKeyboard->GetPress(DIK_D) == true)
			{
				Select(1);
			}
		}
		if (m_pKeyboard->GetKey(DIK_J) == true)
		{
			if (CManager::GetAircraft(m_nSelect[0]) == true)
			{
				if (CFade::GetFade() == CFade::FADE_NONE)
				{
					if (m_bStage == true)
					{
						CManager::SetType(m_nSelect[0]);
						CManager::SetStage(m_nSelect[1]);
						CFade::SetFade(CManager::MODE_GAME);
						CSound::Play(CSound::SOUND_LABEL_YES);
					}
					else
					{
						StageSelect();
					}
				}
			}
			else
			{
				Buy();
			}
		}
		if (m_pKeyboard->GetKey(DIK_I) == true)
		{
			if (m_bBuy == true)
			{
				m_bBuy = false;
				Text();
				CSound::Play(CSound::SOUND_LABEL_CANCEL);
			}
			else if (m_bStage == true)
			{
				StageCancel();
			}
			else if (CFade::GetFade() == CFade::FADE_NONE)
			{
				CFade::SetFade(CManager::MODE_TITLE);
				CSound::Play(CSound::SOUND_LABEL_CANCEL);
			}
		}
	}
	if (m_bClear == true)
	{
		m_nClear -= 26;
		if (m_nClear <= 0)
		{
			m_nClear = 0;
			m_bClear = false;
			m_nCntChange = 0;
			Text();
		}
	}
	else if (m_nClear < 255)
	{
		m_nClear += 26;
		if (m_nClear >= 255)
		{
			m_nClear = 255;
		}
	}
}

//*****************************************************************************
//描画
//***************************************************************************** 
void CSelect::Draw()
{
	RECT rect[4];
	rect[0] = { 110, 535, SCREEN_WIDTH - 100, SCREEN_HEIGHT - 20 };
	rect[1] = { 110, 578, SCREEN_WIDTH - 100, SCREEN_HEIGHT - 20 };
	rect[2] = { 400, 324, 600, SCREEN_HEIGHT - 20 };
	rect[3] = { 870, 470, 1160, SCREEN_HEIGHT - 20 };
	char str[2][256];

	wsprintf(&str[0][0], "SPEED:\nDEFENSE:\nRANGE:\nSLOW:\nRETURN:");

	wsprintf(&str[1][0], "MONEY:%6d", CManager::GetMoney());

	// テキスト描画
	m_pFont[0]->DrawText(NULL, m_Name, -1, &rect[0], DT_WORDBREAK, D3DCOLOR_ARGB(m_nClear, 0, 255, 0));	//名前
	m_pFont[1]->DrawText(NULL, m_Str, -1, &rect[1], DT_WORDBREAK, D3DCOLOR_ARGB(m_nClear, 0, 255, 0));	//説明文
	if (m_nCntSelect <= 0 && m_bStage == false)
	{
		m_pFont[2]->DrawText(NULL, &str[0][0], -1, &rect[2], DT_RIGHT, D3DCOLOR_ARGB(255, 0, 255, 0));	//ステータス
	}
	if (m_fStageClear <= 0.0f && m_bStage == false)
	{
		m_pFont[3]->DrawText(NULL, &str[1][0], -1, &rect[3], DT_RIGHT, D3DCOLOR_ARGB(255, 0, 255, 0));	//所持金
	}
}

//*****************************************************************************
//作成
//*****************************************************************************
CSelect *CSelect::Create()
{
	CSelect *pSelect = NULL;
	pSelect = new CSelect(PRIORITY_PAUSEUI);		//メモリ確保
													//NULLチェック
	if (pSelect != NULL)
	{
		pSelect->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	return pSelect;
}

void CSelect::Load()
{
	//ファイル読み込み
	char aFile[256];
	FILE *pFile = fopen(FILE_AIRCRAFT, "r");

	bool bStatus;
	int nCntAircraft = 0;

	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]);

			if (strcmp(&aFile[0], "STATUS") == 0)
			{
				bStatus = true;
			}
			if (strcmp(&aFile[0], "END_STATUS") == 0)
			{
				bStatus = false;
				nCntAircraft++;
			}
			if (bStatus == true)
			{
				if (strcmp(&aFile[0], "NAME") == 0)
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%s", &m_aAircraft[nCntAircraft].aName[0]);
				}
				if (strcmp(&aFile[0], "NICKNAME") == 0)
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%s", &m_aAircraft[nCntAircraft].aNickName[0]);
				}
				if (strcmp(&aFile[0], "SPEED") == 0)
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &m_aAircraft[nCntAircraft].fSpeed);
				}
				if (strcmp(&aFile[0], "HP") == 0)
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_aAircraft[nCntAircraft].nHP);
				}
				if (strcmp(&aFile[0], "SHOTTYPE") == 0)
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_aAircraft[nCntAircraft].ShotType);
				}
				if (strcmp(&aFile[0], "TEXTYPE") == 0)
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_aAircraft[nCntAircraft].nTexType);
				}
				if (strcmp(&aFile[0], "HUD_TEXTYPE") == 0)
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_aAircraft[nCntAircraft].nHudTexType);
				}
				if (strcmp(&aFile[0], "SHADOW_TEXTYPE") == 0)
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_aAircraft[nCntAircraft].nShadowTex);
				}
				if (strcmp(&aFile[0], "MAX_SLOW") == 0)
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_aAircraft[nCntAircraft].nMaxSlow);
				}
				if (strcmp(&aFile[0], "MAX_RETURN") == 0)
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_aAircraft[nCntAircraft].nMaxReturn);
				}
				if (strcmp(&aFile[0], "MONEY") == 0)
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_aAircraft[nCntAircraft].nMoney);
				}
				if (strcmp(&aFile[0], "EXPLAIN_LINES") == 0)
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_aAircraft[nCntAircraft].nExplainLines);
				}
				if (strcmp(&aFile[0], "EXPLAIN") == 0)
				{
					fscanf(pFile, "%s", &aFile[0]);
					int nCntlines;
					for (nCntlines = 0; nCntlines < m_aAircraft[nCntAircraft].nExplainLines; nCntlines++)
					{
						fscanf(pFile, "%s", &m_aAircraft[nCntAircraft].aExplain[nCntlines][0]);
					}
				}
			}
			if (strcmp(&aFile[0], "END_SCRIPT") == 0)	//END_SCRIPTの文字列を見つけたら
			{
				break;
			}
		}
		fclose(pFile);
	}
}

void CSelect::RankLoad()
{
	//ファイル読み込み
	FILE *pFile = fopen(FILE_RANK, "r");
	int nCnt;
	if (pFile != NULL)
	{
		for (nCnt = 0; nCnt < 3; nCnt++)
		{
			fscanf(pFile, "%d", &m_nRank[nCnt]);
		}
		fclose(pFile);
	}
}

void CSelect::StageSelect()
{
	if (CFade::GetFade() == CFade::FADE_NONE)
	{
		int nCntStage;
		for (nCntStage = 0; nCntStage < 15; nCntStage++)
		{
			if ((nCntStage + 2) % 5 == 0)
			{
				if (nCntStage < 5)
				{
					m_pStageUI[nCntStage]->ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}
				else
				{
					m_pStageUI[nCntStage]->ColorChange(D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.0f));
				}
			}
			else
			{
				if (nCntStage < 5)
				{
					m_pStageUI[nCntStage]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f));
				}
				else
				{
					m_pStageUI[nCntStage]->ColorChange(D3DXCOLOR(0.0f, 0.6f, 0.0f, 0.0f));
				}
			}
		}
		int nCntText;
		for (nCntText = 0; nCntText < 5; nCntText++)
		{
			m_pUI[24 + nCntText]->SetSize(D3DXVECTOR2(0.0f, 13.0f));
			m_pUI[24 + nCntText]->Set();
		}
		m_pUI[29]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		m_pUI[30]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		m_bClear = true;
		m_bStage = true;
		CSound::Play(CSound::SOUND_LABEL_YES);
	}
}

void CSelect::StageCancel()
{
	CSound::Play(CSound::SOUND_LABEL_CANCEL);
	m_bClear = true;
	m_bStage = false;
	m_nSelect[1] = 0;
}

void CSelect::Select(int nAdd)
{
	if (m_nCntGauge <= 0 && m_nCntSelect <= 0 && CFade::GetFade() == CFade::FADE_NONE)
	{
		if (m_bStage == true)
		{
			int nCntText;
			for (nCntText = 0; nCntText < 5; nCntText++)
			{
				if (nCntText == 3)
				{
					m_pStageUI[m_nSelect[1] * 5 + nCntText]->ColorChange(D3DXCOLOR(0.6f, 0.6f, 0.6f, m_fStageClear));
				}
				else
				{
					m_pStageUI[m_nSelect[1] * 5 + nCntText]->ColorChange(D3DXCOLOR(0.0f, 0.6f, 0.0f, m_fStageClear));
				}
			}
			m_nSelect[1] += nAdd;
			if (m_nSelect[1] < 0)
			{
				m_nSelect[1] = 2;
			}
			else if (m_nSelect[1] > 2)
			{
				m_nSelect[1] = 0;
			}
			for (nCntText = 0; nCntText < 5; nCntText++)
			{
				if (nCntText == 3)
				{
					m_pStageUI[m_nSelect[1] * 5 + nCntText]->ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fStageClear));
				}
				else
				{
					m_pStageUI[m_nSelect[1] * 5 + nCntText]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fStageClear));
				}
			}
			m_bClear = true;
		}
		else if (m_bBuy == false)
		{
			int nCntText;
			for (nCntText = 0; nCntText < 4; nCntText++)
			{
				m_pUI[m_nSelect[0] * 4 + nCntText]->ColorChange(D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f));
			}
			for (nCntText = 0; nCntText < 5; nCntText++)
			{
				m_pUI[24 + nCntText]->SetSize(D3DXVECTOR2(0.0f, 13.0f));
				m_pUI[24 + nCntText]->Set();
			}
			if (nAdd > 0)
			{
				m_Select = SELECT_RIGHT;
			}
			else
			{
				m_Select = SELECT_LEFT;
			}
			m_nSelect[0] += nAdd;
			if (m_nSelect[0] < 0)
			{
				m_nSelect[0] = 5;
			}
			else if (m_nSelect[0] > 5)
			{
				m_nSelect[0] = 0;
			}
			m_bClear = true;
			for (nCntText = 0; nCntText < 4; nCntText++)
			{
				m_pUI[m_nSelect[0] * 4 + nCntText]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
			}
			m_nCntSelect = 8;
		}
		CSound::Play(CSound::SOUND_LABEL_CHOICE);
	}
}

void CSelect::Buy()
{
	int nMoney = CManager::GetMoney();
	if (m_bBuy == true)
	{
		nMoney -= m_aAircraft[m_nSelect[0]].nMoney;
		CManager::SetMoney(nMoney);
		CManager::SetAircraft(m_nSelect[0], true);
		memset(&m_Name[0], 0, sizeof(m_Name));
		memset(&m_Str[0], 0, sizeof(m_Str));
		wsprintf(&m_Name[0], "NAME:%-10sNICKNAME:%-22s購入済み\n", &m_aAircraft[m_nSelect[0]].aName, &m_aAircraft[m_nSelect[0]].aNickName, m_aAircraft[m_nSelect[0]].nMoney);
		int nCntlines;
		for (nCntlines = 0; nCntlines < m_aAircraft[m_nSelect[0]].nExplainLines; nCntlines++)
		{
			wsprintf(&m_Str[strlen(m_Str)], "%s\n", &m_aAircraft[m_nSelect[0]].aExplain[nCntlines][0]);
		}
		m_bBuy = false;
		CSound::Play(CSound::SOUND_LABEL_BUY);
	}
	else
	{
		if (m_aAircraft[m_nSelect[0]].nMoney > nMoney)
		{
			memset(&m_Str[0], 0, sizeof(m_Str));
			wsprintf(&m_Str[0], "お金が足りません\n");
			m_nCntChange = 100;
			CSound::Play(CSound::SOUND_LABEL_CANCEL);
		}
		else
		{
			memset(&m_Str[0], 0, sizeof(m_Str));
			if (m_pGamePad == NULL)
			{
				wsprintf(&m_Str[0], "この機体を購入しますか？\n決定:J    キャンセル:I\n");
			}
			else
			{
				wsprintf(&m_Str[0], "この機体を購入しますか？\n決定:B    キャンセル:A\n");
			}
			m_bBuy = true;
			CSound::Play(CSound::SOUND_LABEL_YES);
		}
	}
}

void CSelect::Text()
{
	memset(&m_Name[0], 0, sizeof(m_Name));
	memset(&m_Str[0], 0, sizeof(m_Str));
	if (m_bStage == true)
	{
		switch (m_nSelect[1])
		{
		case 0:
			wsprintf(&m_Name[0], "STAGE:初陣     EASY     HIGH SCORE:%8d\n", m_nRank[0]);
			wsprintf(&m_Str[0], "時は2100年、科学は大幅に発展し、時間すらも操れるようになった。\nそんな中、ある企業が戦争を仕掛けてきた。我々は撤退するしかなかった。\n");
			wsprintf(&m_Str[strlen(m_Str)], "後方で急遽、傭兵部隊を再編成した。その傭兵部隊の中に'彼'がいた。\n'彼'にとっての初任務。だがここから、'彼'の物語は始まる。\n");
			break;
		case 1:
			wsprintf(&m_Name[0], "STAGE:突破     NORMAL   HIGH SCORE:%8d\n", m_nRank[1]);
			wsprintf(&m_Str[0], "傭兵部隊の一人としてとてつもない活躍をする'彼'。\nこれなら戦争終結の日も近いだろうと楽観視する人もそう少なくなかった。\n");
			wsprintf(&m_Str[strlen(m_Str)], "そんな時、一部の味方部隊が敵の攻撃で壊滅しそうになっているという報告を受けた。\n'彼'はたった1人、味方部隊を撤退させるための殿として参加する。");
			break;
		case 2:
			wsprintf(&m_Name[0], "STAGE:英雄     HARD     HIGH SCORE:%8d\n", m_nRank[2]);
			wsprintf(&m_Str[0], "無事に部隊の撤退に成功させ、味方からは多大な支持を受ける'彼'。\nそんな中、とある情報が得られた。それは、敵の基地に関するものだった。\n");
			wsprintf(&m_Str[strlen(m_Str)], "だがそこは強力な敵もいるという。'彼'はたった1人で敵基地を壊滅させることとなった。\nこの任務をこなしたら戦争は終わる。失敗は許されない。");
			break;
		default:
			break;
		}
	}
	else
	{
		if (CManager::GetAircraft(m_nSelect[0]) == true)
		{
			wsprintf(&m_Name[0], "NAME:%-10sNICKNAME:%-22s購入済み\n", &m_aAircraft[m_nSelect[0]].aName, &m_aAircraft[m_nSelect[0]].aNickName);
		}
		else
		{
			wsprintf(&m_Name[0], "NAME:%-10sNICKNAME:%-22sMONEY:%6d\n", &m_aAircraft[m_nSelect[0]].aName, &m_aAircraft[m_nSelect[0]].aNickName, m_aAircraft[m_nSelect[0]].nMoney);
		}
		int nCntlines;
		for (nCntlines = 0; nCntlines < m_aAircraft[m_nSelect[0]].nExplainLines; nCntlines++)
		{
			wsprintf(&m_Str[strlen(m_Str)], "%s\n", &m_aAircraft[m_nSelect[0]].aExplain[nCntlines][0]);
		}
	}
}