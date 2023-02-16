//*****************************************************************************
// スコア処理 [Score.cpp]
// Author : satou syusuke
//*****************************************************************************

#include "Score.h"
#include "Scene2D.h"
#include "Ranking.h"
#include "Game.h"
#include "sound.h"
#include "player.h"
#include "fade.h"

#include <assert.h>

//*****************************************************************************
//静的
//*****************************************************************************
int CScore::m_nNumRank = 0;
int CScore::m_nTimeCalc = 0;
int CScore::m_nTime = TIME_LIMIT;

//*****************************************************************************
//マクロ
//*****************************************************************************
#define TIME_FLAME (140)		//１秒カウント用（これ60じゃないんすかね...？）

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CScore::CScore()
{
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		m_apScene[nCnt] = NULL;
	}

	m_nScore = 0;
	m_AddScore = 0;
	m_nDigit = 0;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CScore::~CScore()
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CScore::Init(D3DXVECTOR3 pos)
{
	int nDigit = GetDigit();

	float fSizeX = GetSizeX();
	float fSizeY = GetSizeY();
	int TexNum = GetTexNum();

	for (int nCnt = 0; nCnt < nDigit; nCnt++)
	{
		if (m_apScene[nCnt] == NULL)
		{
			m_apScene[nCnt] = new CScene2D(5);
			m_apScene[nCnt]->Init(D3DXVECTOR3(pos.x - fSizeX * nCnt, pos.y, 0));

			m_apScene[nCnt]->SetWidth(fSizeX);
			m_apScene[nCnt]->SetHeight(fSizeY);
			m_apScene[nCnt]->SetTexture(TexNum);
		}
	}
	m_bUninit = false;
	return S_OK;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CScore::Uninit()
{
	//CManager::MODE Mode = CManager::GetMode();
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		if (m_apScene[nCnt] != NULL)
		{
			m_apScene[nCnt]->Uninit();
			m_apScene[nCnt] = NULL;
		}
	}
	Release();

}

//*****************************************************************************
// 更新
//*****************************************************************************
void CScore::Update()
{
	CManager::MODE Scene = CManager::GetMode();

	int nDigit = GetDigit();
	int nPlayerLife = CPlayer::GetLife();
	bool ContFlag = CGame::GetCountFlag();
	bool bGameEnd = CManager::GetGameEnd();
	bool bRankOne = CManager::GetSaveRankOne();
	bool Pause = CManager::GetPause();

	if (Scene == CManager::MODE_GAME)
	{
		//ポーズ中じゃない
		if (Pause == false)
		{
			//ゲーム終了してない
			if (bGameEnd == false)
			{
				//タイマーカウントする
				if (ContFlag == true)
				{
					//タイマー処理
					m_nTimeCalc++;
					if (m_nTimeCalc >= TIME_FLAME)
					{
						//時間減らす
						m_nTime--;
						m_nTimeCalc = 0;

						if (m_nTime <= 10 && m_nTime > 0)
						{
							CSound::Play(CSound::SOUND_LABEL_SIREN);
						}
					}
				}
			}
		}
	}

	//ゲームクリア時に一度だけ通す
	if (bGameEnd == true && bRankOne == false)
	{
		int nResulutScore = CManager::GetGameTime() * 2 * (3 * 10) + 100 * (10 * nPlayerLife);
		CRanking::SaveRanking(nResulutScore);
		CManager::SetSaveRankOne(true);
	}

	//タイム0
	if (m_nTime <= 0/* && m_bFade == false*/)
	{
		CFade::SetFade(CManager::MODE_GAMEOVER);
		CSound::Stop(CSound::SOUND_LABEL_GAMEBGM);
		//m_bFade = true;
	}
	//ライフ0
	if (nPlayerLife <= 0)
	{
		CFade::SetFade(CManager::MODE_GAMEOVER);
		CSound::Stop(CSound::SOUND_LABEL_GAMEBGM);
	}

	//タイプごとに更新
	switch (m_aNumbertype)
	{
		//番号タイプがスコア
	case(CScore::NUMBER_SCORE):	//スコア
		m_Num = CManager::GetGameTime() * 2 * (3 * 10) + 100 * (10 * nPlayerLife);
		break;

		//番号タイプがランキング
	case(CScore::NUMBER_RANKING):
		m_Num = CRanking::GetRank(m_RankNum);
		break;

		//番号タイプがライフ
	case(CScore::NUMBER_LIFE):
		m_Num = nPlayerLife;
		break;

		//番号タイプがタイム
	case(CScore::NUMBER_TIME):
		m_Num = m_nTime;
		break;

	default:
		assert(false);
		break;
	}

#if 1
	//スコア
	int score = m_Num;
	for (int i = 0; i < 10; ++i)
	{
		m_aScoreNum[i] = score % 10;
		score /= 10;
	}
#else
	m_aScoreNum[9] = m_Num % 10000000000 / 1000000000;
	m_aScoreNum[8] = m_Num % 1000000000 / 100000000;
	m_aScoreNum[7] = m_Num % 100000000 / 10000000;
	m_aScoreNum[6] = m_Num % 10000000 / 1000000;
	m_aScoreNum[5] = m_Num % 1000000 / 100000;
	m_aScoreNum[4] = m_Num % 100000 / 10000;
	m_aScoreNum[3] = m_Num % 10000 / 1000;
	m_aScoreNum[2] = m_Num % 1000 / 100;
	m_aScoreNum[1] = m_Num % 100 / 10;
	m_aScoreNum[0] = m_Num % 10 / 1;
#endif

	CManager::SetGameTime(m_nTime);

	//スコアのUI設定
	for (int nCnt = 0; nCnt < nDigit; nCnt++)
	{
		m_apScene[nCnt]->SetTex(m_aScoreNum[nCnt], 0.1f);
	}

	if (m_bUninit == true)
	{
		SetDeath(true);
	}
}

//*****************************************************************************
// 描画
//*****************************************************************************
void CScore::Draw()
{
	int nDigit = GetDigit();

	for (int nCnt = 0; nCnt < nDigit; nCnt++)
	{
		if (m_apScene[nCnt] != NULL)
		{
			m_apScene[nCnt]->Draw();
		}
	}
}

//*****************************************************************************
// 作成
//*****************************************************************************
CScore *CScore::Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, int nDigit, CScore::NUMBER_TYPE Numtype)
{
	CScore *pScore = NULL;
	pScore = new CScore;

	pScore->SetDigit(nDigit);
	pScore->SetNumtype(Numtype);

	pScore->SetSizeX(SizeX);
	pScore->SetSizeY(SizeY);
	pScore->SetTexNum(nType);


	if (Numtype == CScore::NUMBER_RANKING)
	{
		pScore->SetRankNum(m_nNumRank);
		m_nNumRank++;
	}
	if (pScore != NULL)
	{
		pScore->Init(pos);
	}

	return pScore;
}

//*****************************************************************************
// 時間セット
//*****************************************************************************
void CScore::SetTime(int nNum)
{
	m_nTime = nNum;		//時間セット
	m_nTimeCalc = 0;	//フレームリセット

}