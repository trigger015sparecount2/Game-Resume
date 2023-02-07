//--------------------------
//Author:三上航世
//ボスの処理(boss.cpp)
//--------------------------
#include "boss.h"
#include "aircraft.h"
#include "barrier.h"
#include "ui.h"
#include "prize.h"
#include "sound.h"
#include "death.h"
#include "bullet.h"
#include "laser.h"
#include "fade.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureBoss[4 * 2] = {}; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBoss = NULL; //頂点バッファへのポインタ
Boss g_aBoss[MAX_BOSS]; //敵
bool g_bseagulldeath;

//初期化処理
void InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBoss;
	pDevice = GetDevice();

	//敵の初期化
	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		g_aBoss[nCntBoss].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].type = BOSS_XU_44;
		g_aBoss[nCntBoss].nLife = 0;
		g_aBoss[nCntBoss].nfire = 0;
		g_aBoss[nCntBoss].nScore = 0;
		g_aBoss[nCntBoss].nTime = 0;
		g_aBoss[nCntBoss].nCounterState = 0;
		g_aBoss[nCntBoss].nBulletTime = 0;
		g_aBoss[nCntBoss].nLaserTime = 0;
		g_aBoss[nCntBoss].fSmall = 0.0f;
		g_aBoss[nCntBoss].state = BOSSSTATE_NORMAL;
		g_aBoss[nCntBoss].bUse = false;
		g_aBoss[nCntBoss].bMissile = false;
		g_aBoss[nCntBoss].bLeft = true;
		g_aBoss[nCntBoss].bUp = true;
		g_aBoss[nCntBoss].bBarrier = false;
	}

	g_bseagulldeath = false;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/XU-44.png",
		&g_apTextureBoss[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/seagull.png",
		&g_apTextureBoss[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/XU-80.png",
		&g_apTextureBoss[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/seagullengine.png",
		&g_apTextureBoss[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/XU-44shadow.png",
		&g_apTextureBoss[4]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/seagullshadow.png",
		&g_apTextureBoss[5]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/XU-80shadow.png",
		&g_apTextureBoss[6]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/seagullfire.png",
		&g_apTextureBoss[7]);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BOSS * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBoss,
		NULL);
	
	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBoss->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntBoss = 0; nCntBoss < MAX_BOSS * 2; nCntBoss++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//頂点カラー
		if (nCntBoss % 2 == 0)
		{
			//自分
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else
		{
			//影
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 120);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 120);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 120);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 120);
		}
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBoss->Unlock();
}

//敵の終了処理
void UninitBoss(void)
{
	int nCntBoss;
	//テクスチャの破棄
	for (nCntBoss = 0; nCntBoss < 4 * 2; nCntBoss++)
	{
		if (g_apTextureBoss[nCntBoss] != NULL)
		{
			g_apTextureBoss[nCntBoss]->Release();
			g_apTextureBoss[nCntBoss] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBoss != NULL)
	{
		g_pVtxBuffBoss->Release();
		g_pVtxBuffBoss = NULL;
	}
}

//敵の更新処理
void UpdateBoss(void)
{
	int nCntBoss;
	VERTEX_2D *pVtx;
	g_pVtxBuffBoss->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		//使われてる
		if (g_aBoss[nCntBoss].bUse == true)
		{
			Aircraft *pAircraft;
			pAircraft = GetAircraft();
			Barrier *pBarrier;
			pBarrier = GetBarrier();
			int nCntBarrier;
			D3DXVECTOR3 pos;

			switch (g_aBoss[nCntBoss].state)
			{
			case BOSSSTATE_NORMAL:	//通常
				break;
			case BOSSSTATE_DAMAGE:	//ダメージ受けた
				g_aBoss[nCntBoss].nCounterState--;
				if (g_aBoss[nCntBoss].nCounterState < 0)
				{
					//色と状態元に戻す
					g_aBoss[nCntBoss].state = BOSSSTATE_NORMAL;
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
				break;
			case BOSSSTATE_DEATH:	//やられた
				g_aBoss[nCntBoss].nCounterState--;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				if (g_aBoss[nCntBoss].nCounterState <= 0)
				{
					if (g_aBoss[nCntBoss].type == BOSS_XU_44 || g_aBoss[nCntBoss].type == BOSS_XU_80)
					{
						//ランダムな場所に爆発起こす
						pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 45.0f + rand() % 90, g_aBoss[nCntBoss].pos.y - 60.0f + rand() % 120, 0.0f);
						SetDeath(pos, true);
					}
					else if (g_aBoss[nCntBoss].type == BOSS_SEAGULL)
					{
						//ランダムな場所に爆発起こす
						pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 495.0f + rand() % 990, g_aBoss[nCntBoss].pos.y - 95.0f + rand() % 190, 0.0f);
						SetDeath(pos, true);
					}
					else if (g_aBoss[nCntBoss].type == BOSS_ENGINE)
					{
						//ランダムな場所に爆発起こす
						if (nCntBoss < 3)
						{
							pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 35.0f + rand() % 70, g_aBoss[nCntBoss].pos.y - 35.0f + rand() % 70, 0.0f);
						}
						else
						{
							pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 20.0f + rand() % 40, g_aBoss[nCntBoss].pos.y - 20.0f + rand() % 40, 0.0f);
						}
						SetDeath(pos, false);
					}
					g_aBoss[nCntBoss].nCounterState = 10;
				}

				//ボス本体
				if (g_aBoss[nCntBoss].type == BOSS_XU_44 || g_aBoss[nCntBoss].type == BOSS_XU_80 || g_bseagulldeath == true)
				{
					//どんどん小さくなる、画面下に行く
					g_aBoss[nCntBoss].fSmall -= 0.05f;
					g_aBoss[nCntBoss].nTime++;
					if (g_aBoss[nCntBoss].nTime % 2 == 0)
					{
						g_aBoss[nCntBoss].pos.x += 3.0f;
					}
					else
					{
						g_aBoss[nCntBoss].pos.x -= 3.0f;
					}
					if (g_aBoss[nCntBoss].nTime >= 600)
					{
						g_aBoss[nCntBoss].pos.y += 3.0f;
					}
					if (g_aBoss[nCntBoss].type == BOSS_XU_44 || g_aBoss[nCntBoss].type == BOSS_XU_80)
					{
						//下に行くとフェードする
						if (g_aBoss[nCntBoss].pos.y >= 785.0f)
						{
							g_aBoss[nCntBoss].bUse = false;
							if (g_aBoss[nCntBoss].type == BOSS_XU_80)
							{
								SetFade(MODE_RESULT, true);
							}
							else
							{
								SetFade(MODE_RESULT, false);
							}
							StopSound();
						}
					}
					else
					{
						//下に行くとフェードする
						if (g_aBoss[nCntBoss].pos.y >= 920.0f)
						{
							g_aBoss[nCntBoss].bUse = false;
							SetFade(MODE_GAME, false);
							StopSound();
						}
					}
				}
				break;
			case BOSSSTATE_BARRIER:	//バリア状態
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				break;
			}

			//登場時、上からゆっくり来る
			if (g_aBoss[nCntBoss].nTime <= 500)
			{
				g_aBoss[nCntBoss].pos.y += 0.5f;
				g_aBoss[nCntBoss].nTime++;
			}
			else if (g_aBoss[nCntBoss].type == BOSS_XU_44 || g_aBoss[nCntBoss].type == BOSS_XU_80)
			{
				//やられてない
				if (g_aBoss[nCntBoss].state != BOSSSTATE_DEATH)
				{
					//カウント進める
					g_aBoss[nCntBoss].nBulletTime++;
					g_aBoss[nCntBoss].nLaserTime++;
					
					//左行く
					if (g_aBoss[nCntBoss].bLeft == true)
					{
						g_aBoss[nCntBoss].pos.x -= 1.5f;
						if (g_aBoss[nCntBoss].pos.x <= 50.0f)
						{
							g_aBoss[nCntBoss].bLeft = false;
						}
					}
					//右行く
					else
					{
						g_aBoss[nCntBoss].pos.x += 1.5f;
						if (g_aBoss[nCntBoss].pos.x >= 1230.0f)
						{
							g_aBoss[nCntBoss].bLeft = true;
						}
					}

					//上行く
					if (g_aBoss[nCntBoss].bUp == true)
					{
						g_aBoss[nCntBoss].pos.y -= 0.5f;
						if (g_aBoss[nCntBoss].pos.y <= 65.0f)
						{
							g_aBoss[nCntBoss].bUp = false;
						}
					}
					//下行く
					else
					{
						g_aBoss[nCntBoss].pos.y += 0.5f;
						if (g_aBoss[nCntBoss].pos.y >= 150.0f)
						{
							g_aBoss[nCntBoss].bUp = true;
						}
					}

					//ショットカウント進んだ
					if (g_aBoss[nCntBoss].nBulletTime >= 120 && g_aBoss[nCntBoss].nBulletTime % 12 == 0)
					{
						//左に移動中、左に撃つ
						if (g_aBoss[nCntBoss].bLeft == true)
						{
							SetBullet(g_aBoss[nCntBoss].pos, D3DXVECTOR3(-2.0f, 9.0f, 0.0f), 2, false);
						}
						//右に移動中、右に撃つ
						else
						{
							SetBullet(g_aBoss[nCntBoss].pos, D3DXVECTOR3(2.0f, 9.0f, 0.0f), 2, false);
						}

						//カウントが進むと、インターバル発生
						if (g_aBoss[nCntBoss].nBulletTime >= 140)
						{
							g_aBoss[nCntBoss].nBulletTime = -120 - (rand() % 30);
						}
					}

					//レーザー発射
					if (g_aBoss[nCntBoss].nLaserTime >= 900)
					{
						if(g_aBoss[nCntBoss].type == BOSS_XU_80)
						SetLaser(g_aBoss[nCntBoss].pos, 1);
						g_aBoss[nCntBoss].nLaserTime = 0;
					}
				}
			}
			else if (g_aBoss[nCntBoss].type == BOSS_SEAGULL)
			{
				//やられてない
				if (g_aBoss[nCntBoss].state != BOSSSTATE_DEATH)
				{
					//カウント進める
					g_aBoss[nCntBoss].nBulletTime++;
					g_aBoss[nCntBoss].nLaserTime++;

					//カウント進むと、あちこちから真下に撃つ
					if (g_aBoss[nCntBoss].nBulletTime >= 40)
					{
						pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (rand() % 7 - 3) * 130.0f, g_aBoss[nCntBoss].pos.y + 80.0f, 0.0f);
						SetBullet(pos, D3DXVECTOR3(0.0f, 7.0f, 0.0f), 2, false);
						g_aBoss[nCntBoss].nBulletTime = 0;
					}

					//レーザー発射
					if (g_aBoss[nCntBoss].nLaserTime >= 900)
					{
						SetLaser(g_aBoss[nCntBoss].pos, 1);
						g_aBoss[nCntBoss].nLaserTime = 0;
					}
				}
			}

			if (g_aBoss[nCntBoss].type == BOSS_XU_44 || g_aBoss[nCntBoss].type == BOSS_XU_80)
			{
				//機体
				pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - (50.0f + g_aBoss[nCntBoss].fSmall), g_aBoss[nCntBoss].pos.y - (65.0f + g_aBoss[nCntBoss].fSmall), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (50.0f + g_aBoss[nCntBoss].fSmall), g_aBoss[nCntBoss].pos.y - (65.0f + g_aBoss[nCntBoss].fSmall), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - (50.0f + g_aBoss[nCntBoss].fSmall), g_aBoss[nCntBoss].pos.y + (65.0f + g_aBoss[nCntBoss].fSmall), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (50.0f + g_aBoss[nCntBoss].fSmall), g_aBoss[nCntBoss].pos.y + (65.0f + g_aBoss[nCntBoss].fSmall), 0.0f);
				pVtx += 4;
				//影
				pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (80.0f + g_aBoss[nCntBoss].fSmall) - 15.0f, g_aBoss[nCntBoss].pos.y + (80.0f + g_aBoss[nCntBoss].fSmall) - 20.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (80.0f + g_aBoss[nCntBoss].fSmall) + 15.0f, g_aBoss[nCntBoss].pos.y + (80.0f + g_aBoss[nCntBoss].fSmall) - 20.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (80.0f + g_aBoss[nCntBoss].fSmall) - 15.0f, g_aBoss[nCntBoss].pos.y + (80.0f + g_aBoss[nCntBoss].fSmall) + 20.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (80.0f + g_aBoss[nCntBoss].fSmall) + 15.0f, g_aBoss[nCntBoss].pos.y + (80.0f + g_aBoss[nCntBoss].fSmall) + 20.0f, 0.0f);
			}
			else if (g_aBoss[nCntBoss].type == BOSS_SEAGULL)
			{
				//機体
				pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - (500.0f + g_aBoss[nCntBoss].fSmall), g_aBoss[nCntBoss].pos.y - (110.0f + g_aBoss[nCntBoss].fSmall), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (500.0f + g_aBoss[nCntBoss].fSmall), g_aBoss[nCntBoss].pos.y - (110.0f + g_aBoss[nCntBoss].fSmall), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - (500.0f + g_aBoss[nCntBoss].fSmall), g_aBoss[nCntBoss].pos.y + (110.0f + g_aBoss[nCntBoss].fSmall), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (500.0f + g_aBoss[nCntBoss].fSmall), g_aBoss[nCntBoss].pos.y + (110.0f + g_aBoss[nCntBoss].fSmall), 0.0f);
				pVtx += 4;
				//影
				pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (300.0f + g_aBoss[nCntBoss].fSmall) - 200.0f, g_aBoss[nCntBoss].pos.y + (300.0f + g_aBoss[nCntBoss].fSmall) - 22.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (300.0f + g_aBoss[nCntBoss].fSmall) + 200.0f, g_aBoss[nCntBoss].pos.y + (300.0f + g_aBoss[nCntBoss].fSmall) - 22.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (300.0f + g_aBoss[nCntBoss].fSmall) - 200.0f, g_aBoss[nCntBoss].pos.y + (300.0f + g_aBoss[nCntBoss].fSmall) + 22.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (300.0f + g_aBoss[nCntBoss].fSmall) + 200.0f, g_aBoss[nCntBoss].pos.y + (300.0f + g_aBoss[nCntBoss].fSmall) + 22.0f, 0.0f);
			}
			else if (g_aBoss[nCntBoss].type == BOSS_ENGINE)
			{
				//カウント進める
				g_aBoss[nCntBoss].nfire++;
				if (nCntBoss < 3)
				{
					//エンジン部分
					pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - (40.0f + g_aBoss[nCntBoss].fSmall), g_aBoss[nCntBoss].pos.y - (40.0f + g_aBoss[nCntBoss].fSmall), 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (40.0f + g_aBoss[nCntBoss].fSmall), g_aBoss[nCntBoss].pos.y - (40.0f + g_aBoss[nCntBoss].fSmall), 0.0f);
					pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - (40.0f + g_aBoss[nCntBoss].fSmall), g_aBoss[nCntBoss].pos.y + (40.0f + g_aBoss[nCntBoss].fSmall), 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (40.0f + g_aBoss[nCntBoss].fSmall), g_aBoss[nCntBoss].pos.y + (40.0f + g_aBoss[nCntBoss].fSmall), 0.0f);
					pVtx += 4;
					if (g_aBoss[nCntBoss].state != BOSSSTATE_DEATH)	//やられてない
					{
						//炎大小させる
						if (g_aBoss[nCntBoss].nfire < 5)
						{
							pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 40.0f, g_aBoss[nCntBoss].pos.y + 90.0f - 50.0f, 0.0f);
							pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 40.0f, g_aBoss[nCntBoss].pos.y + 90.0f - 50.0f, 0.0f);
							pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 40.0f, g_aBoss[nCntBoss].pos.y + 90.0f + 50.0f, 0.0f);
							pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 40.0f, g_aBoss[nCntBoss].pos.y + 90.0f + 50.0f, 0.0f);
						}
						else
						{
							pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 50.0f, g_aBoss[nCntBoss].pos.y + 90.0f - 50.0f, 0.0f);
							pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 50.0f, g_aBoss[nCntBoss].pos.y + 90.0f - 50.0f, 0.0f);
							pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 50.0f, g_aBoss[nCntBoss].pos.y + 90.0f + 65.0f, 0.0f);
							pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 50.0f, g_aBoss[nCntBoss].pos.y + 90.0f + 65.0f, 0.0f);
						}
					}
				}
				else
				{
					//エンジン部分
					pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - (25.0f + g_aBoss[nCntBoss].fSmall), g_aBoss[nCntBoss].pos.y - (25.0f + g_aBoss[nCntBoss].fSmall), 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (25.0f + g_aBoss[nCntBoss].fSmall), g_aBoss[nCntBoss].pos.y - (25.0f + g_aBoss[nCntBoss].fSmall), 0.0f);
					pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - (25.0f + g_aBoss[nCntBoss].fSmall), g_aBoss[nCntBoss].pos.y + (25.0f + g_aBoss[nCntBoss].fSmall), 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (25.0f + g_aBoss[nCntBoss].fSmall), g_aBoss[nCntBoss].pos.y + (25.0f + g_aBoss[nCntBoss].fSmall), 0.0f);
					pVtx += 4;
					if (g_aBoss[nCntBoss].state != BOSSSTATE_DEATH)	//やられてない
					{
						//炎大小させる
						if (g_aBoss[nCntBoss].nfire < 5)
						{
							pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 25.0f, g_aBoss[nCntBoss].pos.y + 65.0f - 40.0f, 0.0f);
							pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 25.0f, g_aBoss[nCntBoss].pos.y + 65.0f - 40.0f, 0.0f);
							pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 25.0f, g_aBoss[nCntBoss].pos.y + 65.0f + 40.0f, 0.0f);
							pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 25.0f, g_aBoss[nCntBoss].pos.y + 65.0f + 40.0f, 0.0f);
						}
						else
						{
							pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 30.0f, g_aBoss[nCntBoss].pos.y + 65.0f - 40.0f, 0.0f);
							pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 30.0f, g_aBoss[nCntBoss].pos.y + 65.0f - 40.0f, 0.0f);
							pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 30.0f, g_aBoss[nCntBoss].pos.y + 65.0f + 50.0f, 0.0f);
							pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 30.0f, g_aBoss[nCntBoss].pos.y + 65.0f + 50.0f, 0.0f);
						}
					}
				}

				//カウント進むとリセット
				if (g_aBoss[nCntBoss].nfire == 10)
				{
					g_aBoss[nCntBoss].nfire = 0;
				}
			}

			//バリア
			for (nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++, pBarrier++)
			{
				//使ってる
				if (pBarrier->bUse == true)
				{
					//バリアに敵がぶつかったときダメージ食らう
					if (g_aBoss[nCntBoss].type == BOSS_XU_44 || g_aBoss[nCntBoss].type == BOSS_XU_80)
					{
						if (pBarrier->pos.x - (BARRIER_WIDTH / 2.0f) - 50.0f <= g_aBoss[nCntBoss].pos.x && g_aBoss[nCntBoss].pos.x <= pBarrier->pos.x + (BARRIER_WIDTH / 2.0f) + 50.0f &&
							pBarrier->pos.y - (BARRIER_HEIGHT / 2.0f) - 65.0f <= g_aBoss[nCntBoss].pos.y && g_aBoss[nCntBoss].pos.y <= pBarrier->pos.y + (BARRIER_HEIGHT / 2.0f) + 65.0f)
						{
							if (pBarrier->state == BARRIERSTATE_NORMAL && g_aBoss[nCntBoss].state != BOSSSTATE_DEATH && pBarrier->ntype == 0)
							{
								HitBarrier(1, nCntBarrier);
								HitBoss(nCntBoss, 3, true);
							}
						}
					}
					else if (g_aBoss[nCntBoss].type == BOSS_ENGINE && g_aBoss[nCntBoss].state != BOSSSTATE_DEATH && pBarrier->ntype == 0)
					{
						if (nCntBoss < 3)
						{
							if (pBarrier->pos.x - (BARRIER_WIDTH / 2.0f) - 40.0f <= g_aBoss[nCntBoss].pos.x && g_aBoss[nCntBoss].pos.x <= pBarrier->pos.x + (BARRIER_WIDTH / 2.0f) + 40.0f &&
								pBarrier->pos.y - (BARRIER_HEIGHT / 2.0f) - 40.0f <= g_aBoss[nCntBoss].pos.y && g_aBoss[nCntBoss].pos.y <= pBarrier->pos.y + (BARRIER_HEIGHT / 2.0f) + 40.0f)
							{
								if (pBarrier->state == BARRIERSTATE_NORMAL && g_aBoss[nCntBoss].state != BOSSSTATE_DEATH && pBarrier->ntype == 0)
								{
									HitBarrier(1, nCntBarrier);
									HitBoss(nCntBoss, 3, true);
								}
							}
						}
						else if (pBarrier->pos.x - (BARRIER_WIDTH / 2.0f) - 25.0f <= g_aBoss[nCntBoss].pos.x && g_aBoss[nCntBoss].pos.x <= pBarrier->pos.x + (BARRIER_WIDTH / 2.0f) + 25.0f &&
							     pBarrier->pos.y - (BARRIER_HEIGHT / 2.0f) - 25.0f <= g_aBoss[nCntBoss].pos.y && g_aBoss[nCntBoss].pos.y <= pBarrier->pos.y + (BARRIER_HEIGHT / 2.0f) + 25.0f)
						{
							if (pBarrier->state == BARRIERSTATE_NORMAL && g_aBoss[nCntBoss].state != BOSSSTATE_DEATH && pBarrier->ntype == 0)
							{
								HitBarrier(1, nCntBarrier);
								HitBoss(nCntBoss, 3, true);
							}
						}
					}
				}
			}

			//自機に敵がぶつかったとき
			if (g_aBoss[nCntBoss].type == BOSS_XU_44 || g_aBoss[nCntBoss].type == BOSS_XU_80)
			{
				if (pAircraft->pos.x - (AIRCRAFT_WIDTH / 2.0f) - 50.0f <= g_aBoss[nCntBoss].pos.x && g_aBoss[nCntBoss].pos.x <= pAircraft->pos.x + (AIRCRAFT_WIDTH / 2.0f) + 50.0f &&
					pAircraft->pos.y - (AIRCRAFT_HEIGHT / 2.0f) - 65.0f <= g_aBoss[nCntBoss].pos.y && g_aBoss[nCntBoss].pos.y <= pAircraft->pos.y + (AIRCRAFT_HEIGHT / 2.0f) + 65.0f)
				{
					if (pAircraft->state == AIRCRAFTSTATE_NORMAL && g_aBoss[nCntBoss].state != BOSSSTATE_DEATH)
					{
						HitAircraft();
						HitBoss(nCntBoss, 1, false);
					}
				}
			}
			else if (g_aBoss[nCntBoss].type == BOSS_ENGINE && g_aBoss[nCntBoss].state != BOSSSTATE_DEATH)
			{
				if (nCntBoss < 3)
				{
					if (pAircraft->pos.x - (AIRCRAFT_WIDTH / 2.0f) - 40.0f <= g_aBoss[nCntBoss].pos.x && g_aBoss[nCntBoss].pos.x <= pAircraft->pos.x + (AIRCRAFT_WIDTH / 2.0f) + 40.0f &&
						pAircraft->pos.y - (AIRCRAFT_HEIGHT / 2.0f) - 40.0f <= g_aBoss[nCntBoss].pos.y && g_aBoss[nCntBoss].pos.y <= pAircraft->pos.y + (AIRCRAFT_HEIGHT / 2.0f) + 40.0f)
					{
						if (pAircraft->state == AIRCRAFTSTATE_NORMAL)
						{
							HitAircraft();
							HitBoss(nCntBoss, 1, false);
						}
					}
				}
				else if (pAircraft->pos.x - (AIRCRAFT_WIDTH / 2.0f) - 25.0f <= g_aBoss[nCntBoss].pos.x && g_aBoss[nCntBoss].pos.x <= pAircraft->pos.x + (AIRCRAFT_WIDTH / 2.0f) + 25.0f &&
						 pAircraft->pos.y - (AIRCRAFT_HEIGHT / 2.0f) - 25.0f <= g_aBoss[nCntBoss].pos.y && g_aBoss[nCntBoss].pos.y <= pAircraft->pos.y + (AIRCRAFT_HEIGHT / 2.0f) + 25.0f)
				{
					if (pAircraft->state == AIRCRAFTSTATE_NORMAL)
					{
						HitAircraft();
						HitBoss(nCntBoss, 1, false);
					}
				}
			}
			pVtx += 4;
		}
		else
		{
			pVtx += 8;
		}
	}
	g_pVtxBuffBoss->Unlock();
}

//描画処理
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	int nCntBoss;
	pDevice = GetDevice();                        //デバイスを取得する

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBoss, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		//使用中のみ描画
		if (g_aBoss[nCntBoss].bUse == true)
		{
			pDevice->SetTexture(0, g_apTextureBoss[g_aBoss[nCntBoss].type]); //テクスチャの設定
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntBoss * 8,
				2);
			if (g_aBoss[nCntBoss].type == BOSS_ENGINE && g_aBoss[nCntBoss].state != BOSSSTATE_DEATH || g_aBoss[nCntBoss].type != BOSS_ENGINE)
			{
				pDevice->SetTexture(0, g_apTextureBoss[g_aBoss[nCntBoss].type + MAX_BOSS_TYPE]); //テクスチャの設定
				//ポリゴン描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
					nCntBoss * 8 + 4,
					2);
			}
		}
	}
}

//敵の設定処理
void SetBoss(D3DXVECTOR3 pos, BOSSTYPE bosstype)
{
	int nCntBoss = 0;
	VERTEX_2D *pVtx;
	g_pVtxBuffBoss->Lock(0, 0, (void **)&pVtx, 0);
	g_aBoss[nCntBoss].pos = pos;
	if (bosstype == BOSS_XU_44 || bosstype == BOSS_XU_80)
	{
		pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 50.0f, g_aBoss[nCntBoss].pos.y - 65.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 50.0f, g_aBoss[nCntBoss].pos.y - 65.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 50.0f, g_aBoss[nCntBoss].pos.y + 65.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 50.0f, g_aBoss[nCntBoss].pos.y + 65.0f, 0.0f);
	}
	else if (bosstype == BOSS_SEAGULL)
	{
		pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 500.0f, g_aBoss[nCntBoss].pos.y - 100.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 500.0f, g_aBoss[nCntBoss].pos.y - 100.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 500.0f, g_aBoss[nCntBoss].pos.y + 100.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 500.0f, g_aBoss[nCntBoss].pos.y + 100.0f, 0.0f);
	}
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//ステータス設定
	g_aBoss[nCntBoss].type = bosstype;
	g_aBoss[nCntBoss].nfire = 0;
	g_aBoss[nCntBoss].nTime = 0;
	g_aBoss[nCntBoss].nCounterState = 0;
	g_aBoss[nCntBoss].state = BOSSSTATE_NORMAL;
	g_aBoss[nCntBoss].fSmall = 0.0f;
	g_aBoss[nCntBoss].bUse = true;
	g_aBoss[nCntBoss].bMissile = false;
	g_aBoss[nCntBoss].bLeft = true;
	g_aBoss[nCntBoss].bUp = true;

	switch (bosstype)
	{
	case BOSS_XU_44:
		g_aBoss[nCntBoss].nScore = 3000;
		g_aBoss[nCntBoss].nLife = 450;
		break;
	case BOSS_SEAGULL:
		g_aBoss[nCntBoss].nScore = 0;
		g_aBoss[nCntBoss].nLife = 8;
		break;
	case BOSS_XU_80:
		g_aBoss[nCntBoss].nScore = 10000;
		g_aBoss[nCntBoss].nLife = 500;
		break;
	}
	pVtx += 4;
	if (bosstype == BOSS_XU_44 || bosstype == BOSS_XU_80)
	{
		pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 80.0f - 50.0f, g_aBoss[nCntBoss].pos.y + 80.0f - 65.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 80.0f + 50.0f, g_aBoss[nCntBoss].pos.y + 80.0f - 65.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 80.0f - 50.0f, g_aBoss[nCntBoss].pos.y + 80.0f + 65.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 80.0f + 50.0f, g_aBoss[nCntBoss].pos.y + 80.0f + 65.0f, 0.0f);
	}
	else if (bosstype == BOSS_SEAGULL)
	{
		pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 300.0f - 200.0f, g_aBoss[nCntBoss].pos.y + 300.0f - 50.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 300.0f + 200.0f, g_aBoss[nCntBoss].pos.y + 300.0f - 50.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 300.0f - 200.0f, g_aBoss[nCntBoss].pos.y + 300.0f + 50.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 300.0f + 200.0f, g_aBoss[nCntBoss].pos.y + 300.0f + 50.0f, 0.0f);
	}
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 120);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 120);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 120);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 120);
	if (bosstype == BOSS_SEAGULL)
	{
		for (nCntBoss = 1; nCntBoss < 9; nCntBoss++)
		{
			pVtx += 4;
			switch (nCntBoss)
			{
			case 1:
				g_aBoss[nCntBoss].pos = D3DXVECTOR3(pos.x - 60.0f, pos.y + 90.0f, pos.z);
				break;
			case 2:
				g_aBoss[nCntBoss].pos = D3DXVECTOR3(pos.x + 60.0f, pos.y + 90.0f, pos.z);
				break;
			case 3:
				g_aBoss[nCntBoss].pos = D3DXVECTOR3(pos.x - 200.0f, pos.y + 90.0f, pos.z);
				break;
			case 4:
				g_aBoss[nCntBoss].pos = D3DXVECTOR3(pos.x + 200.0f, pos.y + 90.0f, pos.z);
				break;
			case 5:
				g_aBoss[nCntBoss].pos = D3DXVECTOR3(pos.x - 340.0f, pos.y + 90.0f, pos.z);
				break;
			case 6:
				g_aBoss[nCntBoss].pos = D3DXVECTOR3(pos.x + 340.0f, pos.y + 90.0f, pos.z);
				break;
			case 7:
				g_aBoss[nCntBoss].pos = D3DXVECTOR3(pos.x - 460.0f, pos.y + 90.0f, pos.z);
				break;
			case 8:
				g_aBoss[nCntBoss].pos = D3DXVECTOR3(pos.x + 460.0f, pos.y + 90.0f, pos.z);
				break;
			}
			if(nCntBoss < 3)
			{
				pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 40.0f, g_aBoss[nCntBoss].pos.y - 40.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 40.0f, g_aBoss[nCntBoss].pos.y - 40.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 40.0f, g_aBoss[nCntBoss].pos.y + 40.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 40.0f, g_aBoss[nCntBoss].pos.y + 40.0f, 0.0f);
			}
			else
			{
				pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 25.0f, g_aBoss[nCntBoss].pos.y - 25.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 25.0f, g_aBoss[nCntBoss].pos.y - 25.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 25.0f, g_aBoss[nCntBoss].pos.y + 25.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 25.0f, g_aBoss[nCntBoss].pos.y + 25.0f, 0.0f);
			}
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			
			//ステータス設定
			g_aBoss[nCntBoss].type = BOSS_ENGINE;
			g_aBoss[nCntBoss].nfire = 0;
			g_aBoss[nCntBoss].nCounterState = 0;
			g_aBoss[nCntBoss].state = BOSSSTATE_NORMAL;
			g_aBoss[nCntBoss].bUse = true;
			g_aBoss[nCntBoss].bMissile = false;
			g_aBoss[nCntBoss].bLeft = true;
			g_aBoss[nCntBoss].bUp = true;
			g_aBoss[nCntBoss].nBulletTime = 0;
			g_aBoss[nCntBoss].nLaserTime = 0;
			g_aBoss[nCntBoss].nScore = 900;
			g_aBoss[nCntBoss].nLife = 80;
			g_aBoss[nCntBoss].fSmall = 0.0f;
			g_aBoss[nCntBoss].nTime = 0;
			g_aBoss[nCntBoss].bBarrier = false;
			pVtx += 4;

			if (nCntBoss < 3)
			{
				pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 40.0f, g_aBoss[nCntBoss].pos.y + 90.0f - 50.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 40.0f, g_aBoss[nCntBoss].pos.y + 90.0f - 50.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 40.0f, g_aBoss[nCntBoss].pos.y + 90.0f + 50.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 40.0f, g_aBoss[nCntBoss].pos.y + 90.0f + 50.0f, 0.0f);
			}
			else
			{
				pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 25.0f, g_aBoss[nCntBoss].pos.y + 65.0f - 40.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 25.0f, g_aBoss[nCntBoss].pos.y + 65.0f - 40.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 25.0f, g_aBoss[nCntBoss].pos.y + 65.0f + 40.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + 25.0f, g_aBoss[nCntBoss].pos.y + 65.0f + 40.0f, 0.0f);
			}
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 200);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 200);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 200);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 200);
		}
	}
	g_pVtxBuffBoss->Unlock();
}

//敵の取得
Boss *GetBoss(void)
{
	return &g_aBoss[0];
}

//敵のダメージ処理
void HitBoss(int nCntBoss, int nDamage, bool bSpecial)
{
	//体力減らす
	g_aBoss[nCntBoss].nLife -= nDamage;

	//やられた
	if (g_aBoss[nCntBoss].nLife <= 0)
	{
		SetDeath(g_aBoss[nCntBoss].pos, true);
		if (g_aBoss[nCntBoss].type == BOSS_XU_44)
		{
			//プライズ設定
			SetPrize(g_aBoss[nCntBoss].pos);
		}
		else
		{
			//エンジン
			if (g_aBoss[nCntBoss].type == BOSS_ENGINE)
			{
				int nCntBoss2;
				for (nCntBoss2 = 0; nCntBoss2 < MAX_BOSS; nCntBoss2++)
				{
					if (g_aBoss[nCntBoss2].type == BOSS_SEAGULL)
					{
						//本体の体力減らす
						g_aBoss[nCntBoss2].nLife--;
						break;
					}
				}

				//本体の体力0
				if (g_aBoss[nCntBoss2].nLife <= 0)
				{
					//プライズ設定など
					SetDeath(g_aBoss[nCntBoss2].pos, true);
					g_aBoss[nCntBoss2].state = BOSSSTATE_DEATH;
					SetPrize(g_aBoss[nCntBoss2].pos);
					g_bseagulldeath = true;
				}
			 }
		}

		//ステータス設定
		g_aBoss[nCntBoss].state = BOSSSTATE_DEATH;
		g_aBoss[nCntBoss].nCounterState = 10;

		//スコア、撃墜数プラス
		if (bSpecial == true)
		{
			AddScore(g_aBoss[nCntBoss].nScore * 2);
		}
		else
		{
			AddScore(g_aBoss[nCntBoss].nScore);
		}
		AddKill();
	}
	else
	{
		//ステータス設定
		g_aBoss[nCntBoss].state = BOSSSTATE_DAMAGE;
		g_aBoss[nCntBoss].nCounterState = 5;

		//スコア加算
		if (bSpecial == true)
		{
			AddScore(g_aBoss[nCntBoss].nScore / 50);
		}
		else
		{
			AddScore(g_aBoss[nCntBoss].nScore / 100);
		}

		//特定の相手、特定の状態
		if (g_aBoss[nCntBoss].type == BOSS_XU_80 && g_aBoss[nCntBoss].bBarrier == false && g_aBoss[nCntBoss].nLife <= 250)
		{
			//バリア張る
			SetBarrier(g_aBoss[nCntBoss].pos, 90, 1);
			g_aBoss[nCntBoss].bBarrier = true;
			g_aBoss[nCntBoss].state = BOSSSTATE_BARRIER;
		}

		//赤くする
		VERTEX_2D *pVtx;
		g_pVtxBuffBoss->Lock(0, 0, (void **)&pVtx, 0);
		pVtx += nCntBoss * 8;
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		g_pVtxBuffBoss->Unlock();
	}
}

//リセット
void ResetBoss(void)
{
	int nCntBoss;
	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		g_aBoss[nCntBoss].bUse = false;
	}
}