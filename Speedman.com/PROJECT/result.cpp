//=============================================================================
//リザルト管理[Result.cpp]
// Author : 佐藤秀亮
//=============================================================================
//*****************************************************************************
// インクルードファイル
//***************************************************************************** 
#if 1
#include "Result.h"
#include "keyboard.h"
#include "gamepad.h"
#include "UI.h"
#include "effect_ui.h"

#include "Fade.h"
#include "Score.h"
#include "Ranking.h"
#include "Bg.h"

#include "sound.h"
#endif
//*****************************************************************************
// 静的メンバ変数
//***************************************************************************** 
bool CResult::m_bFade = NULL;

#if 1
//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CResult::CResult()
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
HRESULT CResult::Init(D3DXVECTOR3 /*pos*/)
{
	m_pKeyboard = CManager::GetKeyboard();
	m_pGamePad = CManager::GetGamepad();
	CRanking::LoadRanking();		//ランキングのロード
	bool RankOnly = CManager::GetRankOnly();

	//背景
	CBG::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), SCREEN_WIDTH, SCREEN_HEIGHT, 15);

	//UIの配置
	CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 190.0f, 0), 300.0f, 50.0f, 29);		//UI

																					//ランキングの番号UI配置
	CUI::Create(D3DXVECTOR3(520.0f, 270.0f, 0), 60.0f, 50.0f, 3);		//UI
	CUI::Create(D3DXVECTOR3(520.0f, 350.0f, 0), 60.0f, 50.0f, 4);		//UI
	CUI::Create(D3DXVECTOR3(520.0f, 430.0f, 0), 60.0f, 50.0f, 5);		//UI
	CUI::Create(D3DXVECTOR3(520.0f, 510.0f, 0), 60.0f, 50.0f, 6);		//UI
	CUI::Create(D3DXVECTOR3(520.0f, 590.0f, 0), 60.0f, 50.0f, 7);		//UI

	if (RankOnly == false)
	{
		CUI::Create(D3DXVECTOR3(640.0f, 50.0f, 0), 300.0f, 50.0f, 28);		//UI
		//プレイヤースコア
		CScore::Create(D3DXVECTOR3(700.0f, 100, 0), 30, 40, 2, 5, CScore::NUMBER_SCORE);
	}
	else
	{
		CSound::Play(CSound::SOUND_LABEL_RESULTBGM);
		CSound::ControlVoice(CSound::SOUND_LABEL_RESULTBGM, 1.3f);
	}

	//エフェクト配置するUI(テスト)
	CEffectUI::Create(D3DXVECTOR3(300.0f, SCREEN_HEIGHT, 0), 1, 1, 0, CEffectUI::EFFECT_RANKCYCLE);
	CEffectUI::Create(D3DXVECTOR3(980.0f, SCREEN_HEIGHT, 0), 1, 1, 0, CEffectUI::EFFECT_RANKCYCLE);

	//ランキングの順番をリセット・繰り返すと増えまくる
	CScore::SetRankReset();

	//ランキング番号配置
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		CScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 130.0f, 270.0f + 80.0f * nCnt, 0.0f), 40.0f, 50.0f, 2, 5, CScore::NUMBER_RANKING);
	}

	//フェード
	CFade::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), SCREEN_WIDTH, SCREEN_HEIGHT, 1);
	m_bFade = false;

	return S_OK;
}

//*****************************************************************************
// 終了
//***************************************************************************** 
void CResult::Uninit()
{
	CScene::Release();
}

//*****************************************************************************
// kousinn
//***************************************************************************** 
void CResult::Update()
{

	CFade::FADE Fade = CFade::GetFade();
	if (Fade == CFade::FADE_NONE)
	{
		if (m_pGamePad != NULL)
		{
			if (m_pGamePad->GetButton(CGamePad::DIP_A))
			{
				CFade::SetFade(CManager::MODE_TITLE);

				CManager::SetRankOnly(false);
				m_bFade = true;
				CSound::Stop(CSound::SOUND_LABEL_RESULTBGM);
			}
		}
		else if (m_pKeyboard != NULL)
		{
			if (m_pKeyboard->GetKey(DIK_RETURN) == true || m_pKeyboard->GetKey(DIK_SPACE) == true)
			{
				CFade::SetFade(CManager::MODE_TITLE);

				CManager::SetRankOnly(false);
				m_bFade = true;
				CSound::Stop(CSound::SOUND_LABEL_RESULTBGM);
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

}

//*****************************************************************************
//作成
//***************************************************************************** 
CResult *CResult::Create()
{
	CResult *pResult = NULL;
	pResult = new CResult;		//メモリ確保
								//NULLチェック
	if (pResult != NULL)
	{
		pResult->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	return pResult;
}
#endif
