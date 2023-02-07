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

//初期化
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

//終了
void CGame::Uninit(void)
{
	Release();
}

//更新
void CGame::Update(void)
{
	//ポーズ中じゃない
	if (CManager::GetPause() == false)
	{
		//ボスがまだ出てきてない
		if (m_bBoss == false)
		{
			//雑魚出現
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

			//ボス出現の時間になったらボス出現
			if (m_Boss.nTime == m_nTime)
			{
				CBoss::Create(m_Boss.pos, m_Boss.nTex, m_Boss.nShadowTex, m_Boss.fInterval, m_Boss.size, m_Boss.nHP, m_Boss.fBulletMax, m_Boss.nMoney, m_Boss.nScore, m_Boss.bParts, m_Boss.nBGMType);
				m_bBoss = true;
			}

			//時間進める
			m_nTime++;
		}
	}
}

//描画
void CGame::Draw(void)
{

}

//作成
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

//読み込む
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
			if (strcmp(&aFile[0], "BG_TEXTURE") == 0)	//背景のテクスチャ
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &m_nBgTex);
			}
			if (strcmp(&aFile[0], "TYPE") == 0)	//雑魚敵の種類
			{
				bType = true;
				nCntType = 0;
			}
			if (strcmp(&aFile[0], "END_TYPE") == 0)	//雑魚敵の種類終了
			{
				bType = false;
			}
			if (strcmp(&aFile[0], "ENEMY") == 0)	//雑魚敵設置
			{
				bEnemy = true;
				nCntEnemy = 0;
			}
			if (strcmp(&aFile[0], "END_ENEMY") == 0)	//雑魚敵設置終了
			{
				bEnemy = false;
			}
			if (strcmp(&aFile[0], "BOSS") == 0)	//ボス敵設置
			{
				bBoss = true;
			}
			if (strcmp(&aFile[0], "END_BOSS") == 0)	//ボス敵設置終了
			{
				bBoss = false;
			}

			//雑魚敵種類設定
			if (bType == true)
			{
				if (strcmp(&aFile[0], "TYPESET") == 0)	//雑魚敵の設定
				{
					bTypeSet = true;
				}
				if (strcmp(&aFile[0], "END_TYPESET") == 0)	//雑魚敵の設定終了
				{
					bTypeSet = false;
					nCntType++;
				}
				if (bTypeSet == true)
				{
					if (strcmp(&aFile[0], "TEXTURE") == 0)	//テクスチャ
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Type[nCntType].nTex);
					}
					if (strcmp(&aFile[0], "SHADOW_TEXTURE") == 0)	//影のテクスチャ
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Type[nCntType].nShadowTex);
					}
					if (strcmp(&aFile[0], "SIZE") == 0)	//ポリゴンの大きさ
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f %f", &m_Type[nCntType].size.x, &m_Type[nCntType].size.y);
					}
					if (strcmp(&aFile[0], "HP") == 0)	//体力
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Type[nCntType].nHP);
					}
					if (strcmp(&aFile[0], "SCORE") == 0)	//スコア
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Type[nCntType].nScore);
					}
					if (strcmp(&aFile[0], "MONEY") == 0)	//金
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Type[nCntType].nMoney);
					}
				}
			}

			//雑魚敵の設置
			if (bEnemy == true)
			{
				if (strcmp(&aFile[0], "ENEMYSET") == 0)				//雑魚敵の設定
				{
					bEnemySet = true;
				}
				if (strcmp(&aFile[0], "END_ENEMYSET") == 0)			//雑魚敵の設定終了
				{
					bEnemySet = false;
					nCntEnemy++;
				}
				if (bEnemySet == true)
				{
					if (strcmp(&aFile[0], "TYPE_NUMBER") == 0)		//雑魚敵の種類
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Enemy[nCntEnemy].nType);
					}
					if (strcmp(&aFile[0], "POS") == 0)				//場所
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f %f %f", &m_Enemy[nCntEnemy].pos.x, &m_Enemy[nCntEnemy].pos.y, &m_Enemy[nCntEnemy].pos.z);
					}
					if (strcmp(&aFile[0], "ROT") == 0)				//向き
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &m_Enemy[nCntEnemy].fRotY);
					}
					if (strcmp(&aFile[0], "INTERVAL") == 0)			//弾撃つまでのインターバル
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &m_Enemy[nCntEnemy].fInterval);
					}
					if (strcmp(&aFile[0], "SPEED") == 0)			//速さ
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &m_Enemy[nCntEnemy].fSpeed);
					}
					if (strcmp(&aFile[0], "BULLET_MAX") == 0)		//2発目撃つまでのインターバル
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &m_Enemy[nCntEnemy].fBulletMax);
					}
					if (strcmp(&aFile[0], "TIME") == 0)				//出てくる時間
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Enemy[nCntEnemy].nTime);
					}
					if (strcmp(&aFile[0], "BULLET_TYPE") == 0)		//弾撃つ種類
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Enemy[nCntEnemy].nBulletType);
					}
					if (strcmp(&aFile[0], "BULLET_SPEED") == 0)		//弾の速度
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &m_Enemy[nCntEnemy].fBulletSpeed);
					}
				}
			}
			if (bBoss == true)
			{
				if (strcmp(&aFile[0], "BOSSSET") == 0)				//ボス敵設定
				{
					bBossSet = true;
				}
				if (strcmp(&aFile[0], "END_BOSSSET") == 0)			//ボス敵設定
				{
					bBossSet = false;
				}
				if (bBossSet == true)
				{
					if (strcmp(&aFile[0], "TEXTURE") == 0)			//テクスチャ設定
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Boss.nTex);
					}
					if (strcmp(&aFile[0], "SHADOW_TEXTURE") == 0)	//影のテクスチャ
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Boss.nShadowTex);
					}
					if (strcmp(&aFile[0], "SIZE") == 0)				//ポリゴンの大きさ
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f %f", &m_Boss.size.x, &m_Boss.size.y);
					}
					if (strcmp(&aFile[0], "HP") == 0)				//体力
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Boss.nHP);
					}
					if (strcmp(&aFile[0], "SCORE") == 0)			//スコア
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Boss.nScore);
					}
					if (strcmp(&aFile[0], "MONEY") == 0)			//金
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Boss.nMoney);
					}
					if (strcmp(&aFile[0], "POS") == 0)				//場所
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f %f %f", &m_Boss.pos.x, &m_Boss.pos.y, &m_Boss.pos.z);
					}
					if (strcmp(&aFile[0], "INTERVAL") == 0)			//最初弾撃つまでの時間
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &m_Boss.fInterval);
					}
					if (strcmp(&aFile[0], "BULLET_MAX") == 0)		//2発目以降に発射するまでのインターバル
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &m_Boss.fBulletMax);
					}
					if (strcmp(&aFile[0], "TIME") == 0)				//出現する時間
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Boss.nTime);
					}
					if (strcmp(&aFile[0], "BGMTYPE") == 0)			//BGMの種類
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_Boss.nBGMType);
					}
					if (strcmp(&aFile[0], "PARTS") == 0)			//パーツがあるか
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
			if (strcmp(&aFile[0], "END_SCRIPT") == 0)	//END_SCRIPTの文字列を見つけたら終了
			{
				break;
			}
		}
		fclose(pFile);
	}
}