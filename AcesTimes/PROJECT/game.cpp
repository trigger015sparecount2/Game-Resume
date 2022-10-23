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
#include "sound.h"
#include "enemy.h"
#include "game.h"
#include "manager.h"
#include "fade.h"
#include "ui.h"
#include "pauseui.h"
#include "shadow.h"
#include "bg.h"
#include "select.h"
#include "boss.h"

//*****************************************************************************
//マクロ
//*****************************************************************************
#define FILE_STAGE1 "data/FILES/stage1.txt"
#define FILE_STAGE2 "data/FILES/stage2.txt"
#define FILE_STAGE3 "data/FILES/stage3.txt"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================


CGame::CGame(PRIORITY Priority) : CScene::CScene(Priority)
{

}

CGame::~CGame(void)
{

}

HRESULT CGame::Init(D3DXVECTOR3)
{
	int nCntEnemy;
	for (nCntEnemy = 0; nCntEnemy < 64; nCntEnemy++)
	{
		m_Enemy[nCntEnemy].nTime = -1;
	}
	Load();
	CBg::Create(D3DXVECTOR2(SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.8f), D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR2(25.0f, 17.0f), m_nBgTex);
	CPlayer::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 600.0f, 0.0f), D3DXVECTOR2(70.0f, 70.0f), CSelect::GetSelectHP(), CSelect::GetSpeed(), 
					CSelect::GetMaxSlow(), CSelect::GetMaxReturn(), CSelect::GetTexType(), CSelect::GetShotType(), CSelect::GetShadowTexType(), CSelect::GetHudTexType());
	m_nCntEnemy = 0;
	m_nTime = 0;
	m_bBoss = false;
	CManager::SetPause(false);
	CManager::ResetMoney();
	return S_OK;
}

void CGame::Uninit(void)
{
	Release();
}

void CGame::Update(void)
{
	if (CManager::GetPause() == false)
	{
		if (m_bBoss == false)
		{
			for (m_nCntEnemy; m_nCntEnemy < 64; m_nCntEnemy++)
			{
				if (m_Enemy[m_nCntEnemy].nTime == m_nTime)
				{
					CEnemy::Create(m_Enemy[m_nCntEnemy].pos, m_Type[m_Enemy[m_nCntEnemy].nType].nTex, m_Type[m_Enemy[m_nCntEnemy].nType].nShadowTex, m_Enemy[m_nCntEnemy].fRotY,
						m_Enemy[m_nCntEnemy].fInterval, m_Enemy[m_nCntEnemy].fSpeed, m_Type[m_Enemy[m_nCntEnemy].nType].size, m_Type[m_Enemy[m_nCntEnemy].nType].nHP,
						m_Enemy[m_nCntEnemy].fBulletMax, m_Type[m_Enemy[m_nCntEnemy].nType].nMoney, m_Type[m_Enemy[m_nCntEnemy].nType].nScore, m_Enemy[m_nCntEnemy].fBulletSpeed, m_Enemy[m_nCntEnemy].nBulletType);
				}
				else
				{
					break;
				}
			}
			if (m_Boss.nTime == m_nTime)
			{
				CBoss::Create(m_Boss.pos, m_Boss.nTex, m_Boss.nShadowTex, m_Boss.fInterval, m_Boss.size, m_Boss.nHP, m_Boss.fBulletMax, m_Boss.nMoney, m_Boss.nScore, m_Boss.bParts, m_Boss.nBGMType);
				m_bBoss = true;
			}
			m_nTime++;
		}
	}
}

void CGame::Draw(void)
{

}

CGame *CGame::Create()
{
	CGame *pGame = NULL;
	pGame = new CGame(PRIORITY_EFFECT);		//メモリ確保
	//NULLチェック
	if (pGame != NULL)
	{
		pGame->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	return pGame;
}

void CGame::Load()
{
	//ファイル読み込み
	char aFile[256];
	FILE *pFile;
	switch (CManager::GetStage())
	{
	case 0:
		pFile = fopen(FILE_STAGE1, "r");
		CSound::Play(CSound::SOUND_LABEL_STAGE1BGM);
		break;
	case 1:
		pFile = fopen(FILE_STAGE2, "r");
		CSound::Play(CSound::SOUND_LABEL_STAGE2BGM);
		break;
	case 2:
		pFile = fopen(FILE_STAGE3, "r");
		CSound::Play(CSound::SOUND_LABEL_STAGE3BGM);
		break;
	default:
		pFile = fopen(FILE_STAGE1, "r");
		CSound::Play(CSound::SOUND_LABEL_STAGE1BGM);
		break;
	}
	bool bEnemy = false;
	bool bEnemySet = false;
	bool bBoss = false;
	bool bBossSet = false;
	bool bType = false;
	bool bTypeSet = false;
	int nCntEnemy = 0;
	int nCntType = 0;
	int nParts = -1;

	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]);
			if (strcmp(&aFile[0], "BG_TEXTURE") == 0)
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &m_nBgTex);
			}
			if (strcmp(&aFile[0], "TYPE") == 0)
			{
				bType = true;
				nCntType = 0;
			}
			if (strcmp(&aFile[0], "END_TYPE") == 0)
			{
				bType = false;
			}
			if (strcmp(&aFile[0], "ENEMY") == 0)
			{
				bEnemy = true;
				nCntEnemy = 0;
			}
			if (strcmp(&aFile[0], "END_ENEMY") == 0)
			{
				bEnemy = false;
			}
			if (strcmp(&aFile[0], "BOSS") == 0)
			{
				bBoss = true;
			}
			if (strcmp(&aFile[0], "END_BOSS") == 0)
			{
				bBoss = false;
			}
			if (bType == true)
			{
				if (strcmp(&aFile[0], "TYPESET") == 0)
				{
					bTypeSet = true;
				}
				if (strcmp(&aFile[0], "END_TYPESET") == 0)
				{
					bTypeSet = false;
					nCntType++;
				}
				if (strcmp(&aFile[0], "TEXTURE") == 0)
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_Type[nCntType].nTex);
				}
				if (strcmp(&aFile[0], "SHADOW_TEXTURE") == 0)
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_Type[nCntType].nShadowTex);
				}
				if (strcmp(&aFile[0], "SIZE") == 0)
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f", &m_Type[nCntType].size.x, &m_Type[nCntType].size.y);
				}
				if (strcmp(&aFile[0], "HP") == 0)
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_Type[nCntType].nHP);
				}
				if (strcmp(&aFile[0], "SCORE") == 0)
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_Type[nCntType].nScore);
				}
				if (strcmp(&aFile[0], "MONEY") == 0)
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_Type[nCntType].nMoney);
				}
			}
			if (bEnemy == true)
			{
				if (strcmp(&aFile[0], "ENEMYSET") == 0)
				{
					bEnemySet = true;
				}
				if (strcmp(&aFile[0], "END_ENEMYSET") == 0)
				{
					bEnemySet = false;
					nCntEnemy++;
				}
				if (bEnemySet == true)
				{
					if (strcmp(&aFile[0], "TYPE_NUMBER") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Enemy[nCntEnemy].nType);
					}
					if (strcmp(&aFile[0], "POS") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f %f %f", &m_Enemy[nCntEnemy].pos.x, &m_Enemy[nCntEnemy].pos.y, &m_Enemy[nCntEnemy].pos.z);
					}
					if (strcmp(&aFile[0], "ROT") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &m_Enemy[nCntEnemy].fRotY);
					}
					if (strcmp(&aFile[0], "INTERVAL") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &m_Enemy[nCntEnemy].fInterval);
					}
					if (strcmp(&aFile[0], "SPEED") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &m_Enemy[nCntEnemy].fSpeed);
					}
					if (strcmp(&aFile[0], "BULLET_MAX") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &m_Enemy[nCntEnemy].fBulletMax);
					}
					if (strcmp(&aFile[0], "TIME") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Enemy[nCntEnemy].nTime);
					}
					if (strcmp(&aFile[0], "BULLET_TYPE") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Enemy[nCntEnemy].nBulletType);
					}
					if (strcmp(&aFile[0], "BULLET_SPEED") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &m_Enemy[nCntEnemy].fBulletSpeed);
					}
				}
			}
			if (bBoss == true)
			{
				if (strcmp(&aFile[0], "BOSSSET") == 0)
				{
					bBossSet = true;
				}
				if (strcmp(&aFile[0], "END_BOSSSET") == 0)
				{
					bBossSet = false;
				}
				if (bBossSet == true)
				{
					if (strcmp(&aFile[0], "TEXTURE") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Boss.nTex);
					}
					if (strcmp(&aFile[0], "SHADOW_TEXTURE") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Boss.nShadowTex);
					}
					if (strcmp(&aFile[0], "SIZE") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f %f", &m_Boss.size.x, &m_Boss.size.y);
					}
					if (strcmp(&aFile[0], "HP") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Boss.nHP);
					}
					if (strcmp(&aFile[0], "SCORE") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Boss.nScore);
					}
					if (strcmp(&aFile[0], "MONEY") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Boss.nMoney);
					}
					if (strcmp(&aFile[0], "POS") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f %f %f", &m_Boss.pos.x, &m_Boss.pos.y, &m_Boss.pos.z);
					}
					if (strcmp(&aFile[0], "INTERVAL") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &m_Boss.fInterval);
					}
					if (strcmp(&aFile[0], "BULLET_MAX") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &m_Boss.fBulletMax);
					}
					if (strcmp(&aFile[0], "TIME") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Boss.nTime);
					}
					if (strcmp(&aFile[0], "BGMTYPE") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Boss.nBGMType);
					}
					if (strcmp(&aFile[0], "PARTS") == 0)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &nParts);
						if (nParts == 0)
						{
							m_Boss.bParts = true;
						}
						else
						{
							m_Boss.bParts = false;
						}
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