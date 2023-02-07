//=============================================================================
//リザルト管理[Result.cpp]
// Author : 三上航世
//=============================================================================
//*****************************************************************************
// インクルードファイル
//***************************************************************************** 
#include "result.h"
#include "keyboard.h"
#include "gamepad.h"
#include "ui.h"
#include "fade.h"
#include "sound.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// 静的メンバ変数
//***************************************************************************** 


#if 1
//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CResult::CResult(PRIORITY Priority) :CScene(Priority)
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CResult::~CResult()
{

}
#endif
//*****************************************************************************
// 初期化
//***************************************************************************** 
HRESULT CResult::Init(D3DXVECTOR3)
{
	m_pKeyboard = CManager::GetKeyboard();
	m_pGamePad = CManager::GetGamepad();
	CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), -1, D3DXCOLOR(0.0f, 0.2f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 80.0f, 0.0f), D3DXVECTOR2(200.0f, 50.0f), 62, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 600.0f, 0.0f), D3DXVECTOR2(400.0f, 40.0f), 24, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	RankLoad();
	RankSave();
	int nMoney = CManager::GetMoney();
	nMoney += CManager::GetStageMoney();
	if (nMoney > 999999)
	{
		nMoney = 999999;
	}
	CSound::Play(CSound::SOUND_LABEL_RESULTBGM);
	CManager::SetMoney(nMoney);
	D3DXCreateFont(CManager::GetRenderer()->CRenderer::GetDevice(), 90, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "PixelMplus10", &m_pFont[0]);
	D3DXCreateFont(CManager::GetRenderer()->CRenderer::GetDevice(), 90, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "PixelMplus10", &m_pFont[1]);
	D3DXCreateFont(CManager::GetRenderer()->CRenderer::GetDevice(), 90, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "PixelMplus10", &m_pFont[2]);

	return S_OK;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CResult::Uninit()
{
	if (m_pGamePad != NULL)
	{
		m_pGamePad = NULL;
	}
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard = NULL;
	}
	CScene::Release();
}

//*****************************************************************************
// 更新
//***************************************************************************** 
void CResult::Update()
{
	//Pressの点滅
	if (m_bClear == true)
	{
		//薄くする
		m_fClear -= 0.05f;
		m_pUI->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear));
		if (m_fClear <= 0.0f)
		{
			m_fClear = 0.0f;
			m_bClear = false;
		}
	}
	else
	{
		//濃くする
		m_fClear += 0.05f;
		m_pUI->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear));
		if (m_fClear >= 1.0f)
		{
			m_fClear = 1.0f;
			m_bClear = true;
		}
	}
	//ゲームパッド繋がっているか
	if (m_pGamePad != NULL)
	{
		//何か押されたらタイトルに移行
		if (m_pGamePad->GetAnyButton() == true)
		{
			if (CFade::GetFade() == CFade::FADE_NONE)
			{
				CFade::SetFade(CManager::MODE_TITLE);
				CSound::Play(CSound::SOUND_LABEL_YES);
			}
		}
	}
	//キーボードが繋がってる
	else if (m_pKeyboard != NULL)
	{
		//何か押されたらタイトルに移行
		if (m_pKeyboard->GetAnyKey() == true)
		{
			if (CFade::GetFade() == CFade::FADE_NONE)
			{
				CFade::SetFade(CManager::MODE_TITLE);
				CSound::Play(CSound::SOUND_LABEL_YES);
			}
		}
	}
}

//描画・作成
#if 1
//*****************************************************************************
//描画
//***************************************************************************** 
void CResult::Draw()
{
	RECT rect[3];
	rect[0] = { 350, 160, SCREEN_WIDTH - 110, SCREEN_HEIGHT - 20 };	//ステージ
	rect[1] = { 350, 310, SCREEN_WIDTH - 110, SCREEN_HEIGHT - 20 };	//今回のスコア
	rect[2] = { 350, 410, SCREEN_WIDTH - 110, SCREEN_HEIGHT - 20 };	//獲得賞金
	char str[3][32];

	wsprintf(&str[0][0], "STAGE     :%d", CManager::GetStage() + 1);
	wsprintf(&str[1][0], "YOUR SCORE:%-8d", CManager::GetScore());
	wsprintf(&str[2][0], " GET MONEY:%-8d", CManager::GetStageMoney());

	// テキスト描画
	m_pFont[0]->DrawText(NULL, &str[0][0], -1, &rect[0], DT_LEFT, D3DCOLOR_ARGB(255, 0, 255, 0));	//ステージ
	m_pFont[1]->DrawText(NULL, &str[1][0], -1, &rect[1], DT_LEFT, D3DCOLOR_ARGB(255, 0, 255, 0));	//今回のスコア
	m_pFont[2]->DrawText(NULL, &str[2][0], -1, &rect[2], DT_LEFT, D3DCOLOR_ARGB(255, 0, 255, 0));	//獲得賞金
}

//*****************************************************************************
//作成
//*****************************************************************************
CResult *CResult::Create()
{
	CResult *pResult = NULL;
	pResult = new CResult(PRIORITY_PAUSEUI);		//メモリ確保
												//NULLチェック
	if (pResult != NULL)
	{
		pResult->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	return pResult;
}
#endif

//ランキング読み込み
void CResult::RankLoad()
{
	//ファイル読み込み
	FILE *pFile = fopen(FILE_RANK, "r");
	int nCnt;
	if (pFile != NULL)
	{
		for (nCnt = 0; nCnt < 3; nCnt++)
		{
			fscanf(pFile, "%d", &m_nScore[nCnt]);
		}
		fclose(pFile);
	}
}

//ランキングセーブ
void CResult::RankSave()
{
	if (m_nScore[CManager::GetStage()] < CManager::GetScore())
	{
		m_nScore[CManager::GetStage()] = CManager::GetScore();
		//ファイル読み込み
		FILE *pFile = fopen(FILE_RANK, "w");
		int nCnt;
		if (pFile != NULL)
		{
			for (nCnt = 0; nCnt < 3; nCnt++)
			{
				fprintf(pFile, "%d\n", m_nScore[nCnt]);
			}
			fclose(pFile);
		}
	}
}