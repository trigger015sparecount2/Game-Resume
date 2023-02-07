//--------------------------
//Author:三上航世
//敵の処理(enemy.cpp)
//--------------------------
#include "enemy.h"
#include "aircraft.h"
#include "barrier.h"
#include "ui.h"
#include "prize.h"
#include "sound.h"
#include "death.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureEnemy[4 * 2] = {}; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL; //頂点バッファへのポインタ
Enemy g_aEnemy[MAX_ENEMY]; //敵

//初期化処理
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;
	pDevice = GetDevice();

	//敵の初期化
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		int nRoll;
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].type = ENEMY_XU_5;
		g_aEnemy[nCntEnemy].nLife = 0;
		g_aEnemy[nCntEnemy].nScore = 0;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].bPrize = false;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].bMissile = false;
		for (nRoll = 0; nRoll < 4; nRoll++)
		{
			g_aEnemy[nCntEnemy].fRoll[nRoll] = 0.0f;
		}
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/XU-5.png",
		&g_apTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/XU-17.png",
		&g_apTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/XU-37.png",
		&g_apTextureEnemy[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/XU-57.png",
		&g_apTextureEnemy[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/XU-5shadow.png",
		&g_apTextureEnemy[4]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/XU-17shadow.png",
		&g_apTextureEnemy[5]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/XU-37shadow.png",
		&g_apTextureEnemy[6]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/XU-57shadow.png",
		&g_apTextureEnemy[7]);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY * 2; nCntEnemy++)
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
		if (nCntEnemy % 2 == 0)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else
		{
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
	g_pVtxBuffEnemy->Unlock();
}

//敵の終了処理
void UninitEnemy(void)
{
	int nCntEnemy;
	//テクスチャの破棄
	for (nCntEnemy = 0; nCntEnemy < 4 * 2; nCntEnemy++)
	{
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//敵の更新処理
void UpdateEnemy(void)
{
	int nCntEnemy;
	VERTEX_2D *pVtx;
	g_pVtxBuffEnemy->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//使ってる
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			Aircraft *pAircraft;
			pAircraft = GetAircraft();
			Barrier *pBarrier;
			pBarrier = GetBarrier();
			int nCntBarrier;

			//状態によって処理を分ける
			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_NORMAL:	//通常
				break;
			case ENEMYSTATE_DAMAGE:	//ダメ―ジくらった
				//カウント進める
				g_aEnemy[nCntEnemy].nCounterState--;

				//カウント進んだら、色元に戻す
				if (g_aEnemy[nCntEnemy].nCounterState < 0)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
				break;
			}

			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;
			//敵が画面外に行ったとき
			if (g_aEnemy[nCntEnemy].pos.x + (ENEMY_BLOCK / 2.0f) < 0.0f || g_aEnemy[nCntEnemy].pos.x - (ENEMY_BLOCK / 2.0f) > 1280.0f ||
				g_aEnemy[nCntEnemy].pos.y + (ENEMY_BLOCK / 2.0f) < 0.0f || g_aEnemy[nCntEnemy].pos.y - (ENEMY_BLOCK / 2.0f) > 720.0f)
			{
				//消す
				g_aEnemy[nCntEnemy].bUse = false;
			}

			//位置設定
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_BLOCK / 2.0f) * sinf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[0]), g_aEnemy[nCntEnemy].pos.y + (ENEMY_BLOCK / 2.0f) * cosf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[0]), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_BLOCK / 2.0f) * sinf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[1]), g_aEnemy[nCntEnemy].pos.y + (ENEMY_BLOCK / 2.0f) * cosf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[1]), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_BLOCK / 2.0f) * sinf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[2]), g_aEnemy[nCntEnemy].pos.y + (ENEMY_BLOCK / 2.0f) * cosf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[2]), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_BLOCK / 2.0f) * sinf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[3]), g_aEnemy[nCntEnemy].pos.y + (ENEMY_BLOCK / 2.0f) * cosf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[3]), 0.0f);
			pVtx += 4;
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 80.0f + (SHADOW_ENEMY_BLOCK / 2.0f) * sinf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[0]), g_aEnemy[nCntEnemy].pos.y + 80.0f + (SHADOW_ENEMY_BLOCK / 2.0f) * cosf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[0]), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 80.0f + (SHADOW_ENEMY_BLOCK / 2.0f) * sinf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[1]), g_aEnemy[nCntEnemy].pos.y + 80.0f + (SHADOW_ENEMY_BLOCK / 2.0f) * cosf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[1]), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 80.0f + (SHADOW_ENEMY_BLOCK / 2.0f) * sinf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[2]), g_aEnemy[nCntEnemy].pos.y + 80.0f + (SHADOW_ENEMY_BLOCK / 2.0f) * cosf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[2]), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 80.0f + (SHADOW_ENEMY_BLOCK / 2.0f) * sinf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[3]), g_aEnemy[nCntEnemy].pos.y + 80.0f + (SHADOW_ENEMY_BLOCK / 2.0f) * cosf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[3]), 0.0f);
			
			for (nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
			{
				//バリア使ってる
				if (pBarrier->bUse == true)
				{
					//バリアに敵がぶつかったとき
					if (pBarrier->pos.x - (BARRIER_WIDTH / 2.0f) - (ENEMY_BLOCK / 2.0f) <= g_aEnemy[nCntEnemy].pos.x && 
						g_aEnemy[nCntEnemy].pos.x <= pBarrier->pos.x + (BARRIER_WIDTH / 2.0f) + (ENEMY_BLOCK / 2.0f) &&
						pBarrier->pos.y - (BARRIER_HEIGHT / 2.0f) - (ENEMY_BLOCK / 2.0f) <= g_aEnemy[nCntEnemy].pos.y &&
						g_aEnemy[nCntEnemy].pos.y <= pBarrier->pos.y + (BARRIER_HEIGHT / 2.0f) + (ENEMY_BLOCK / 2.0f))
					{
						//バリアの状態ノーマル、自機のバリア
						if (pBarrier->state == BARRIERSTATE_NORMAL && pBarrier->ntype == 0)
						{
							//バリア、敵にダメージ
							HitBarrier(1, nCntBarrier);
							HitEnemy(nCntEnemy, 3, true);
						}
					}
				}
			}

			//自機が敵にぶつかったとき
			if (pAircraft->pos.x - (AIRCRAFT_WIDTH / 2.0f) - (ENEMY_BLOCK / 2.0f) <= g_aEnemy[nCntEnemy].pos.x &&
				g_aEnemy[nCntEnemy].pos.x <= pAircraft->pos.x + (AIRCRAFT_WIDTH / 2.0f) + (ENEMY_BLOCK / 2.0f) &&
				pAircraft->pos.y - (AIRCRAFT_HEIGHT / 2.0f) - (ENEMY_BLOCK / 2.0f) <= g_aEnemy[nCntEnemy].pos.y && 
				g_aEnemy[nCntEnemy].pos.y <= pAircraft->pos.y + (AIRCRAFT_HEIGHT / 2.0f) + (ENEMY_BLOCK / 2.0f))
			{
				//自機の状態ノーマル
				if (pAircraft->state == AIRCRAFTSTATE_NORMAL)
				{
					//自機、敵にダメージ
					HitAircraft();
					HitEnemy(nCntEnemy, 1, false);
				}
			}
			pVtx += 4;
		}
		else
		{
			pVtx += 8;
		}
	}
	g_pVtxBuffEnemy->Unlock();
}

//描画処理
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	int nCntEnemy;
	pDevice = GetDevice();                        //デバイスを取得する
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//使用中のみ描画
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].type]); //テクスチャの設定
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntEnemy * 8,
				2);
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].type + MAX_ENEMY_TYPE]); //テクスチャの設定
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntEnemy * 8 + 4,
				2);
		}
	}
}

//敵の設定処理
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE enemytype, D3DXVECTOR3 move, bool bPrize, WAY way)
{
	int nCntEnemy;
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//使ってない
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			//ステータス設定
			VERTEX_2D *pVtx;
			g_pVtxBuffEnemy->Lock(0, 0, (void **)&pVtx, 0);
			pVtx += nCntEnemy * 8;
			g_aEnemy[nCntEnemy].pos = pos;
			switch (way)
			{
			case WAY_UP:
				g_aEnemy[nCntEnemy].fRoll[0] = -0.75f;
				g_aEnemy[nCntEnemy].fRoll[1] = 0.75f;
				g_aEnemy[nCntEnemy].fRoll[2] = -0.25f;
				g_aEnemy[nCntEnemy].fRoll[3] = 0.25f;
				break;
			case WAY_RIGHT:
				g_aEnemy[nCntEnemy].fRoll[0] = 0.75f;
				g_aEnemy[nCntEnemy].fRoll[1] = 0.25f;
				g_aEnemy[nCntEnemy].fRoll[2] = -0.75f;
				g_aEnemy[nCntEnemy].fRoll[3] = -0.25f;
				break;
			case WAY_DOWN:
				g_aEnemy[nCntEnemy].fRoll[0] = 0.25f;
				g_aEnemy[nCntEnemy].fRoll[1] = -0.25f;
				g_aEnemy[nCntEnemy].fRoll[2] = 0.75f;
				g_aEnemy[nCntEnemy].fRoll[3] = -0.75f;
				break;
			case WAY_LEFT:
				g_aEnemy[nCntEnemy].fRoll[0] = -0.25f;
				g_aEnemy[nCntEnemy].fRoll[1] = -0.75f;
				g_aEnemy[nCntEnemy].fRoll[2] = 0.25f;
				g_aEnemy[nCntEnemy].fRoll[3] = 0.75f;
				break;
			}
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_BLOCK / 2.0f) * sinf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[0]), g_aEnemy[nCntEnemy].pos.y + (ENEMY_BLOCK / 2.0f) * cosf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[0]), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_BLOCK / 2.0f) * sinf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[1]), g_aEnemy[nCntEnemy].pos.y + (ENEMY_BLOCK / 2.0f) * cosf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[1]), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_BLOCK / 2.0f) * sinf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[2]), g_aEnemy[nCntEnemy].pos.y + (ENEMY_BLOCK / 2.0f) * cosf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[2]), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_BLOCK / 2.0f) * sinf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[3]), g_aEnemy[nCntEnemy].pos.y + (ENEMY_BLOCK / 2.0f) * cosf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[3]), 0.0f);
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			g_aEnemy[nCntEnemy].type = enemytype;
			g_aEnemy[nCntEnemy].move = move;
			g_aEnemy[nCntEnemy].bPrize = bPrize;
			g_aEnemy[nCntEnemy].nCounterState = 0;
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
			g_aEnemy[nCntEnemy].bUse = true;
			g_aEnemy[nCntEnemy].bMissile = false;
			switch (enemytype)
			{
			case ENEMY_XU_5:
				g_aEnemy[nCntEnemy].nScore = 200;
				g_aEnemy[nCntEnemy].nLife = 15;
				break;
			case ENEMY_XU_17:
				g_aEnemy[nCntEnemy].nScore = 150;
				g_aEnemy[nCntEnemy].nLife = 11;
				break;
			case ENEMY_XU_37:
				g_aEnemy[nCntEnemy].nScore = 250;
				g_aEnemy[nCntEnemy].nLife = 10;
				break;
			case ENEMY_XU_57:
				g_aEnemy[nCntEnemy].nScore = 300;
				g_aEnemy[nCntEnemy].nLife = 13;
				break;
			}
			pVtx += 4;
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 80.0f + (SHADOW_ENEMY_BLOCK / 2.0f) * sinf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[0]), g_aEnemy[nCntEnemy].pos.y + 80.0f + (SHADOW_ENEMY_BLOCK / 2.0f) * cosf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[0]), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 80.0f + (SHADOW_ENEMY_BLOCK / 2.0f) * sinf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[1]), g_aEnemy[nCntEnemy].pos.y + 80.0f + (SHADOW_ENEMY_BLOCK / 2.0f) * cosf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[1]), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 80.0f + (SHADOW_ENEMY_BLOCK / 2.0f) * sinf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[2]), g_aEnemy[nCntEnemy].pos.y + 80.0f + (SHADOW_ENEMY_BLOCK / 2.0f) * cosf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[2]), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 80.0f + (SHADOW_ENEMY_BLOCK / 2.0f) * sinf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[3]), g_aEnemy[nCntEnemy].pos.y + 80.0f + (SHADOW_ENEMY_BLOCK / 2.0f) * cosf(D3DX_PI * g_aEnemy[nCntEnemy].fRoll[3]), 0.0f);
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 120);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 120);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 120);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 120);
			g_pVtxBuffEnemy->Unlock();
			break;
		}
	}
}

//敵の取得
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//敵のダメージ処理
void HitEnemy(int nCntEnemy, int nDamage, bool bSpecial)
{
	//体力減らす
	g_aEnemy[nCntEnemy].nLife -= nDamage;

	//倒れた
	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		//煙出す、消す、プライズ出す、スコア加算、撃墜数加算
		SetDeath(g_aEnemy[nCntEnemy].pos, true);
		g_aEnemy[nCntEnemy].bUse = false;
		if (g_aEnemy[nCntEnemy].bPrize == true)
		{
			SetPrize(g_aEnemy[nCntEnemy].pos);
		}
		if (bSpecial == true)
		{
			AddScore(g_aEnemy[nCntEnemy].nScore * 2);
		}
		else
		{
			AddScore(g_aEnemy[nCntEnemy].nScore);
		}
		AddKill();
	}
	//まだやられてない
	else
	{
		//ダメージ食らった、カウント設定
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCounterState = 5;

		//スコア加算
		if (bSpecial == true)
		{
			AddScore(g_aEnemy[nCntEnemy].nScore / 10);
		}
		else
		{
			AddScore(g_aEnemy[nCntEnemy].nScore / 20);
		}
		VERTEX_2D *pVtx;
		g_pVtxBuffEnemy->Lock(0, 0, (void **)&pVtx, 0);
		pVtx += nCntEnemy * 8;
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		g_pVtxBuffEnemy->Unlock();
	}
}

//敵リセット
void ResetEnemy(void)
{
	int nCntEnemy;
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].bUse = false;
	}
}