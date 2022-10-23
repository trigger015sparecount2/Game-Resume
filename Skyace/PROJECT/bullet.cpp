//--------------------------
//Author:三上航世
//弾丸やレールガンの処理(bullet.cpp)
//--------------------------
#include "bullet.h"
#include "enemy.h"
#include "boss.h"
#include "aircraft.h"
#include "barrier.h"
#include "sound.h"

//マクロ定義
#define MAX_BULLET (128)   //弾の最大数
#define BULLET_WIDTH (12)  //弾の幅
#define BULLET_HEIGHT (40) //弾の高さ
//弾の構造体定義
typedef struct
{
	D3DXVECTOR3 pos;  //位置
	D3DXVECTOR3 move; //移動量
	int ntype;        //弾丸の種類
	bool bUav;        //UAVが撃ったか否か
	bool bUse;        //使用しているかどうか
} Bullet;
//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBullet[3] = {}; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL; //頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];

//初期化処理
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;
	pDevice = GetDevice();
	//弾の初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].ntype = 0;
		g_aBullet[nCntBullet].bUav = false;
		g_aBullet[nCntBullet].bUse = false;
	}
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/bluebullet.png",
		&g_pTextureBullet[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/railgunbullet.png",
		&g_pTextureBullet[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/redbullet.png",
		&g_pTextureBullet[2]);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);
	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
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
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//弾の終了処理
void UninitBullet(void)
{
	int nCntBullet;
	//テクスチャの破棄
	for (nCntBullet = 0; nCntBullet < 2; nCntBullet++)
	{
		if (g_pTextureBullet[nCntBullet] != NULL)
		{
			g_pTextureBullet[nCntBullet]->Release();
			g_pTextureBullet[nCntBullet] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//弾の更新処理
void UpdateBullet(void)
{
	int nCntBullet;
	VERTEX_2D *pVtx;
	g_pVtxBuffBullet->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			int nCntEnemy;
			int nCntBoss;
			int nCntBarrier;
			Enemy *pEnemy;
			Boss *pBoss;
			Aircraft *pAircraft;
			Barrier *pBarrier;
			pEnemy = GetEnemy();
			pBoss = GetBoss();
			pAircraft = GetAircraft();
			pBarrier = GetBarrier();
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (BULLET_WIDTH / 2), g_aBullet[nCntBullet].pos.y - (BULLET_HEIGHT / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (BULLET_WIDTH / 2), g_aBullet[nCntBullet].pos.y - (BULLET_HEIGHT / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (BULLET_WIDTH / 2), g_aBullet[nCntBullet].pos.y + (BULLET_HEIGHT / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (BULLET_WIDTH / 2), g_aBullet[nCntBullet].pos.y + (BULLET_HEIGHT / 2), 0.0f);
			if (g_aBullet[nCntBullet].pos.y <= -(BULLET_HEIGHT / 2) || g_aBullet[nCntBullet].pos.y >= 740.0f)
			{
				g_aBullet[nCntBullet].bUse = false;
			}
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (pEnemy->bUse == true)
				{
					if (pEnemy->pos.x - (ENEMY_BLOCK / 2) - (BULLET_WIDTH / 2) <= g_aBullet[nCntBullet].pos.x && g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + (ENEMY_BLOCK / 2) + (BULLET_WIDTH / 2) &&
						pEnemy->pos.y - (ENEMY_BLOCK / 2) - (BULLET_HEIGHT / 2) <= g_aBullet[nCntBullet].pos.y && g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y + (ENEMY_BLOCK / 2) + (BULLET_HEIGHT / 2))
					{
						if (g_aBullet[nCntBullet].ntype == 0)
						{
							HitEnemy(nCntEnemy, 1, g_aBullet[nCntBullet].bUav);
							g_aBullet[nCntBullet].bUse = false;
						}
						else if(g_aBullet[nCntBullet].ntype == 1)
						{
							HitEnemy(nCntEnemy, 50, true);
						}
					}
				}
			}
			for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++, pBoss++)
			{
				if (pBoss->bUse == true && pBoss->state != BOSSSTATE_DEATH && pBoss->state != BOSSSTATE_BARRIER)
				{
					if (pBoss->type == BOSS_XU_44 || pBoss->type == BOSS_XU_80)
					{
						if (pBoss->pos.x - 50 - (BULLET_WIDTH / 2) <= g_aBullet[nCntBullet].pos.x && g_aBullet[nCntBullet].pos.x <= pBoss->pos.x + 50 + (BULLET_WIDTH / 2) &&
							pBoss->pos.y - 65 - (BULLET_HEIGHT / 2) <= g_aBullet[nCntBullet].pos.y && g_aBullet[nCntBullet].pos.y <= pBoss->pos.y + 65 + (BULLET_HEIGHT / 2))
						{
							if (g_aBullet[nCntBullet].ntype == 0)
							{
								HitBoss(nCntBoss, 1, g_aBullet[nCntBullet].bUav);
								g_aBullet[nCntBullet].bUse = false;
							}
							else if (g_aBullet[nCntBullet].ntype == 1)
							{
								HitBoss(nCntBoss, 35, true);
							}
						}
					}
					else if (pBoss->type == BOSS_ENGINE)
					{
						if (pBoss->pos.x - 35 - (BULLET_WIDTH / 2) <= g_aBullet[nCntBullet].pos.x && g_aBullet[nCntBullet].pos.x <= pBoss->pos.x + 35 + (BULLET_WIDTH / 2) &&
							pBoss->pos.y - 35 - (BULLET_HEIGHT / 2) <= g_aBullet[nCntBullet].pos.y && g_aBullet[nCntBullet].pos.y <= pBoss->pos.y + 35 + (BULLET_HEIGHT / 2))
						{
							if (g_aBullet[nCntBullet].ntype == 0)
							{
								HitBoss(nCntBoss, 1, g_aBullet[nCntBullet].bUav);
								g_aBullet[nCntBullet].bUse = false;
							}
							else if (g_aBullet[nCntBullet].ntype == 1)
							{
								HitBoss(nCntBoss, 35, true);
							}
						}
					}
				}
			}
			for (nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++, pBarrier++)
			{
				if (pBarrier->bUse == true)
				{
					if (pBarrier->pos.x - (BARRIER_WIDTH / 2) - (BULLET_WIDTH / 2) <= g_aBullet[nCntBullet].pos.x && g_aBullet[nCntBullet].pos.x <= pBarrier->pos.x + (BARRIER_WIDTH / 2) + (BULLET_WIDTH / 2) &&
						pBarrier->pos.y - (BARRIER_HEIGHT / 2) - (BULLET_HEIGHT / 2) <= g_aBullet[nCntBullet].pos.y && g_aBullet[nCntBullet].pos.y <= pBarrier->pos.y + (BARRIER_HEIGHT / 2) + (BULLET_HEIGHT / 2))
					{
						if (g_aBullet[nCntBullet].ntype == 2 && pBarrier->state == BARRIERSTATE_NORMAL && pBarrier->ntype == 0)
						{
							HitBarrier(1, nCntBarrier);
							g_aBullet[nCntBullet].bUse = false;
						}
						else if (pBarrier->ntype == 1)
						{
							if (g_aBullet[nCntBullet].ntype == 0)
							{
								HitBarrier(1, nCntBarrier);
								g_aBullet[nCntBullet].bUse = false;
							}
							else if(g_aBullet[nCntBullet].ntype == 1)
							{
								HitBarrier(10, nCntBarrier);
							}
						}
					}
				}
			}
			if (pAircraft->pos.x - (AIRCRAFT_WIDTH / 2) - (BULLET_WIDTH / 2) <= g_aBullet[nCntBullet].pos.x && g_aBullet[nCntBullet].pos.x <= pAircraft->pos.x + (AIRCRAFT_WIDTH / 2) + (BULLET_WIDTH / 2) &&
				pAircraft->pos.y - (AIRCRAFT_HEIGHT / 2) - (BULLET_HEIGHT / 2) <= g_aBullet[nCntBullet].pos.y && g_aBullet[nCntBullet].pos.y <= pAircraft->pos.y + (AIRCRAFT_HEIGHT / 2) + (BULLET_HEIGHT / 2))
			{
				if (g_aBullet[nCntBullet].ntype == 2 && pAircraft->state == AIRCRAFTSTATE_NORMAL)
				{
					HitAircraft();
					g_aBullet[nCntBullet].bUse = false;
				}
			}
		}
		pVtx += 4;
	}
	g_pVtxBuffBullet->Unlock();
}

//描画処理
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	int nCntBullet;
	pDevice = GetDevice();                        //デバイスを取得する
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureBullet[g_aBullet[nCntBullet].ntype]);    //テクスチャの設定
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntBullet * 4,
				2);
		}
	}
}

//弾の設定処理
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int ntype, bool bUav)
{
	int nCntBullet;
	VERTEX_2D *pVtx;
	g_pVtxBuffBullet->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			g_aBullet[nCntBullet].pos = pos;
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (BULLET_WIDTH / 2), g_aBullet[nCntBullet].pos.y - (BULLET_HEIGHT / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (BULLET_WIDTH / 2), g_aBullet[nCntBullet].pos.y - (BULLET_HEIGHT / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (BULLET_WIDTH / 2), g_aBullet[nCntBullet].pos.y + (BULLET_HEIGHT / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (BULLET_WIDTH / 2), g_aBullet[nCntBullet].pos.y + (BULLET_HEIGHT / 2), 0.0f);
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].bUse = true;
			g_aBullet[nCntBullet].bUav = bUav;
			g_aBullet[nCntBullet].ntype = ntype;
			if (ntype != 1)
			{
				PlaySound(SOUND_LABEL_SHOT000);
			}
			else
			{
				PlaySound(SOUND_LABEL_RAILGUN000);
			}
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffBullet->Unlock();
}

void ResetBullet(void)
{
	int nCntBullet;
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].bUse = false;
	}
}