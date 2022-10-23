//=============================================================================
//
//リザルトのUI処理 [resultui.h]
// Author : 三上航世
//
//=============================================================================
#include "resultui.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"
#include "input.h"
#include "fade.h"
#include "ui.h"
#include "sound.h"

//マクロ定義
#define FILENAME "data/FILE/rank.txt"
#define MAX_RANK (5)

//静的メンバ変数
LPDIRECT3DTEXTURE9 CResultUi::m_pTexture[12] = {};
CScene2D *CResultUi::m_apScene[100] = {};

CResultUi::CResultUi(PRIORITY Priority) : CScene::CScene(Priority)
{

}

CResultUi::~CResultUi(void)
{

}

HRESULT CResultUi::Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize)
{
	m_nClearTime = 0;
	m_nKillRank = MAX_RANK;
	m_nScoreRank = MAX_RANK;
	m_bRankClear = false;
	int aScore[MAX_RANK];
	memset(&aScore[0], 0, sizeof(aScore));
	int aKill[MAX_RANK];
	memset(&aKill[0], 0, sizeof(aKill));
	int nScore;
	int nKill;
	nScore = CUi::GetScore();
	nKill = CUi::GetKill();
	
	//スコア、撃墜数ファイルロード
	FILE *pFile;
	int nCount;
	pFile = fopen(FILENAME, "r");
	if (pFile != NULL)
	{
		for (nCount = 0; nCount < MAX_RANK; nCount++)
		{
			fscanf(pFile, "%d", &aScore[nCount]);
		}
		for (nCount = 0; nCount < MAX_RANK; nCount++)
		{
			fscanf(pFile, "%d", &aKill[nCount]);
		}
		fclose(pFile);
	}

	//ランキング入れ替え
	int nCntRank1;
	int nCntRank2;
	int nLagData = 0;
	//スコア
	for (nCntRank1 = 0; nCntRank1 < MAX_RANK; nCntRank1++)
	{
		if (aScore[nCntRank1] < nScore)
		{
			for (nCntRank2 = 0; nCntRank2 < MAX_RANK - nCntRank1 - 1; nCntRank2++)
			{
				if (nCntRank2 == MAX_RANK - nCntRank1 - 1)
				{
					nLagData = nScore;
				}
				else
				{
					nLagData = aScore[(MAX_RANK - 1) - nCntRank2];
				}
				aScore[(MAX_RANK - 1) - nCntRank2] = aScore[(MAX_RANK - 1) - (nCntRank2 + 1)];
				aScore[(MAX_RANK - 1) - (nCntRank2 + 1)] = nLagData;
			}
			aScore[nCntRank1] = nScore;
			break;
		}
	}
	//撃墜数
	for (nCntRank1 = 0; nCntRank1 < MAX_RANK; nCntRank1++)
	{
		if (aKill[nCntRank1] < nKill)
		{
			for (nCntRank2 = 0; nCntRank2 < MAX_RANK - nCntRank1 - 1; nCntRank2++)
			{
				if (nCntRank2 == MAX_RANK - nCntRank1 - 1)
				{
					nLagData = nKill;
				}
				else
				{
					nLagData = aKill[(MAX_RANK - 1) - nCntRank2];
				}
				aKill[(MAX_RANK - 1) - nCntRank2] = aKill[(MAX_RANK - 1) - (nCntRank2 + 1)];
				aKill[(MAX_RANK - 1) - (nCntRank2 + 1)] = nLagData;
			}
			aKill[nCntRank1] = nKill;
			break;
		}
	}

	//スコア、撃墜数ファイルセーブ
	pFile = fopen(FILENAME, "w");
	if (pFile != NULL)
	{
		for (nCount = 0; nCount < MAX_RANK; nCount++)
		{
			fprintf(pFile, "%d\n", aScore[nCount]);
		}
		for (nCount = 0; nCount < MAX_RANK; nCount++)
		{
			fprintf(pFile, "%d\n", aKill[nCount]);
		}
		fclose(pFile);
	}
	m_pInputGamePad = CManager::GetInputGamePad();
	m_pInputKeyboard = CManager::GetInputKeyboard();
	int nCntLife = 0;
	//背景
	//端
	m_apScene[nCntLife] = CScene2D::Create(SCREEN_WIDTH, SCREEN_HEIGHT, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), PRIORITY_BG);
	m_apScene[nCntLife]->BindTexture(m_pTexture[0]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 0.2f, 0.0f, 1.0f));
	nCntLife++;

	//リザルト（文字）
	m_apScene[nCntLife] = CScene2D::Create(100.0f, 30.0f, D3DXVECTOR3(SCREEN_WIDTH / 2, 60.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->BindTexture(m_pTexture[1]);
	nCntLife++;

	//SCORE（文字）
	m_apScene[nCntLife] = CScene2D::Create(80.0f, 30.0f, D3DXVECTOR3(460.0f, 80.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[4]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	nCntLife++;

	//KILL（文字）
	m_apScene[nCntLife] = CScene2D::Create(60.0f, 30.0f, D3DXVECTOR3(870.0f, 80.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[2]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	nCntLife++;

	//YOU（文字）
	int nCntScore;
	m_apScene[nCntLife] = CScene2D::Create(50.0f, 30.0f, D3DXVECTOR3(320.0f, 180.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[10]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	nCntLife++;
	//今回のスコア
	int nNumber;
	int nRank;
	for (nCntScore = 0; nCntScore < 8; nCntScore++)
	{
		m_apScene[nCntLife] = CScene2D::Create(20.0f, 30.0f, D3DXVECTOR3(390.0f + nCntScore * 20.0f, 180.0f, 0.0f), PRIORITY_UI);
		m_apScene[nCntLife]->BindTexture(m_pTexture[3]);
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		nRank = pow(10, 8 - nCntScore);
		nNumber = nScore % nRank / (nRank / 10);
		m_apScene[nCntLife]->SetTexture(nNumber, 10);
		nCntLife++;
	}

	//YOU（文字）
	m_apScene[nCntLife] = CScene2D::Create(50.0f, 30.0f, D3DXVECTOR3(780.0f, 180.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[10]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	nCntLife++;
	//今回のキル数
	int nCntKill;
	for (nCntKill = 0; nCntKill < 3; nCntKill++)
	{
		m_apScene[nCntLife] = CScene2D::Create(20.0f, 30.0f, D3DXVECTOR3(850.0f + nCntKill * 20.0f, 180.0f, 0.0f), PRIORITY_UI);
		m_apScene[nCntLife]->BindTexture(m_pTexture[3]);
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		nRank = pow(10, 3 - nCntKill);
		nNumber = nKill % nRank / (nRank / 10);
		m_apScene[nCntLife]->SetTexture(nNumber, 10);
		nCntLife++;
	}

	//スコアランク
	int nScoreData = 0;
	int nLagScore = 0;
	int nScoreRank = 0;
	int nCntRank;
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (nScoreData == aScore[nCntRank])
		{
			nLagScore++;
		}
		else
		{
			nScoreRank += nLagScore;
			nLagScore = 1;
		}
		if (m_nScoreRank >= MAX_RANK)
		{
			if (nScore == aScore[nCntRank])
			{
				m_nScoreRank = nCntRank;
			}
		}
		m_apScene[nCntLife] = CScene2D::Create(40.0f, 30.0f, D3DXVECTOR3(340.0f, 280.0f + nCntRank * 60.0f, 0.0f), PRIORITY_UI);
		m_apScene[nCntLife]->BindTexture(m_pTexture[5 + nScoreRank]);
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		nCntLife++;
		for (nCntScore = 0; nCntScore < 8; nCntScore++)
		{
			m_apScene[nCntLife] = CScene2D::Create(20.0f, 30.0f, D3DXVECTOR3(390.0f + nCntScore * 20.0f, 280.0f + nCntRank * 60.0f, 0.0f), PRIORITY_UI);
			m_apScene[nCntLife]->BindTexture(m_pTexture[3]);
			m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
			nRank = pow(10, 8 - nCntScore);
			nNumber = aScore[nCntRank] % nRank / (nRank / 10);
			m_apScene[nCntLife]->SetTexture(nNumber, 10);
			nCntLife++;
		}
		nScoreData = aScore[nCntRank];
	}

	int nKillData = 0;
	int nLagKill = 0;
	int nKillRank = 0;
	//撃墜数ランク
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (nKillData == aKill[nCntRank])
		{
			nLagKill++;
		}
		else
		{
			nKillRank += nLagKill;
			nLagKill = 1;
		}
		if (m_nKillRank >= MAX_RANK)
		{
			if (nKill == aKill[nCntRank])
			{
				m_nKillRank = nCntRank;
			}
		}
		m_apScene[nCntLife] = CScene2D::Create(40.0f, 30.0f, D3DXVECTOR3(790.0f, 280.0f + nCntRank * 60.0f, 0.0f), PRIORITY_UI);
		m_apScene[nCntLife]->BindTexture(m_pTexture[5 + nKillRank]);
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		nCntLife++;
		for (nCntKill = 0; nCntKill < 3; nCntKill++)
		{
			m_apScene[nCntLife] = CScene2D::Create(20.0f, 30.0f, D3DXVECTOR3(850.0f + nCntKill * 20.0f, 280.0f + nCntRank * 60.0f, 0.0f), PRIORITY_UI);
			m_apScene[nCntLife]->BindTexture(m_pTexture[3]);
			m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
			nRank = pow(10, 3 - nCntKill);
			nNumber = aKill[nCntRank] % nRank / (nRank / 10);
			m_apScene[nCntLife]->SetTexture(nNumber, 10);
			nCntLife++;
		}
		nKillData = aKill[nCntRank];
	}

	//press（文字）
	m_apScene[nCntLife] = CScene2D::Create(340.0f, 40.0f, D3DXVECTOR3(SCREEN_WIDTH / 2, 620.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[11]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f));
	nCntLife++;
	
	m_nTime = 0;
	m_bClear = true;
	m_fClear = 0.0f;
	return S_OK;
}

void CResultUi::Uninit(void)
{
	int nCntScene;
	for (nCntScene = 0; nCntScene < 100; nCntScene++)
	{
		if (m_apScene[nCntScene] != NULL)
		{
			m_apScene[nCntScene] = NULL;
		}
	}
	Release();
}

void CResultUi::Update(void)
{
	if (m_nTime >= 120)
	{
		if (m_pInputGamePad != NULL)
		{
			if (m_pInputGamePad->GetAnyButton() == true)
			{
				CSound::Play(CSound::SOUND_LABEL_YES);
				CFade::SetFade(CManager::MODE_TITLE);
			}
		}
		else if (m_pInputKeyboard != NULL)
		{
			if (m_pInputKeyboard->GetAnyKey() == true)
			{
				CSound::Play(CSound::SOUND_LABEL_YES);
				CFade::SetFade(CManager::MODE_TITLE);
			}
		}
		//PRESS～の点滅
		if (m_bClear == true)
		{
			m_fClear += 0.1f;
			if (m_fClear >= 1.0f)
			{
				m_fClear = 1.0f;
				m_bClear = false;
			}
		}
		else
		{
			m_fClear -= 0.1f;
			if (m_fClear <= 0.0f)
			{
				m_fClear = 0.0f;
				m_bClear = true;
			}
		}
		m_apScene[82]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear));
	}
	else
	{
		m_nTime++;
	}
	//順位の点滅
	m_nClearTime++;
	if (m_nClearTime >= 5)
	{
		if (m_bRankClear == true)
		{
			if (m_nScoreRank < MAX_RANK)
			{
				m_apScene[17 + 9 * m_nScoreRank]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
			}
			if (m_nKillRank < MAX_RANK)
			{
				m_apScene[62 + 4 * m_nKillRank]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
			}
			m_bRankClear = false;
		}
		else
		{
			if (m_nScoreRank < MAX_RANK)
			{
				m_apScene[17 + 9 * m_nScoreRank]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f));
			}
			if (m_nKillRank < MAX_RANK)
			{
				m_apScene[62 + 4 * m_nKillRank]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f));
			}
			m_bRankClear = true;
		}
		m_nClearTime = 0;
	}
}

void CResultUi::Draw(void)
{

}

HRESULT CResultUi::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/whitebg.png",
		&m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/result.png",
		&m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/kill.png",
		&m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number.png",
		&m_pTexture[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/SCORE.png",
		&m_pTexture[4]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/1st.png",
		&m_pTexture[5]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/2nd.png",
		&m_pTexture[6]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/3rd.png",
		&m_pTexture[7]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/4th.png",
		&m_pTexture[8]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/5th.png",
		&m_pTexture[9]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/you.png",
		&m_pTexture[10]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/pressanybutton.png",
		&m_pTexture[11]);
	return S_OK;
}

void CResultUi::UnLoad(void)
{
	int nCntLife;
	for (nCntLife = 0; nCntLife < 12; nCntLife++)
	{
		if (m_pTexture[nCntLife] != NULL)
		{
			m_pTexture[nCntLife]->Release();
			m_pTexture[nCntLife] = NULL;
		}
	}
}

CResultUi *CResultUi::Create(void)
{
	CResultUi *pUi;
	pUi = new CResultUi(PRIORITY_UI);
	if (pUi != NULL)
	{
		pUi->Init(0.0f, 0.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f);
	}
	return pUi;
}

void CResultUi::Damage(int nDamage)
{

}

bool CResultUi::GetDamage(void)
{
	return false;
}