//--------------------------
//Author:三上航世
//爆発の処理(explosion.cpp)
//--------------------------
#include "explosion.h"
#include "enemy.h"
#include "boss.h"
#include "barrier.h"

//マクロ定義
#define EXPLOSION_BLOCK (400)  //爆発の大きさ
#define EXPLOSION_LIFE (270)   //爆発の長さ
//爆発の構造体定義
typedef struct
{
	D3DXVECTOR3 pos; //位置
	int nTime;       //寿命
	float fBig;      //大きさ
	int nClear;      //透明度
	bool bUse;       //使用しているかどうか
} Explosion;
//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL; //頂点バッファへのポインタ
Explosion g_aExplosion;

//初期化処理
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	//爆発の初期化
	g_aExplosion.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aExplosion.nTime = 0;
	g_aExplosion.fBig = 0.0f;
	g_aExplosion.nClear = 0;
	g_aExplosion.bUse = false;
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/explosion.png",
		&g_pTextureExplosion);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);
	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void **)&pVtx, 0);
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
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 180);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 180);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 180);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 180);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//爆発の終了処理
void UninitExplosion(void)
{
	//テクスチャの破棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//爆発の更新処理
void UpdateExplosion(void)
{
	VERTEX_2D *pVtx;
	if (g_aExplosion.bUse == true)
	{
		g_aExplosion.nTime++;
		g_aExplosion.pos.y += 0.75f;
		if (g_aExplosion.nTime == EXPLOSION_LIFE)
		{
			g_aExplosion.bUse = false;
		}
		if (g_aExplosion.nTime <= 20)
		{
			g_aExplosion.fBig += 0.05f;
		}
		else if (g_aExplosion.nTime > EXPLOSION_LIFE - 160)
		{
			g_aExplosion.nClear++;
		}
		if ((g_aExplosion.nTime + 1) % 10 == 0)
		{
			int nCntEnemy;
			int nCntBoss;
			int nCntBarrier;
			Enemy *pEnemy;
			pEnemy = GetEnemy();
			Boss *pBoss;
			pBoss = GetBoss();
			Barrier *pBarrier;
			pBarrier = GetBarrier();
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (pEnemy->bUse == true)
				{
					//当たり判定
					if (pEnemy->pos.x - (ENEMY_BLOCK / 2) - (EXPLOSION_BLOCK / 2) <= g_aExplosion.pos.x && g_aExplosion.pos.x <= pEnemy->pos.x + (ENEMY_BLOCK / 2) + (EXPLOSION_BLOCK / 2) &&
						pEnemy->pos.y - (ENEMY_BLOCK / 2) - (EXPLOSION_BLOCK / 2) <= g_aExplosion.pos.y && g_aExplosion.pos.y <= pEnemy->pos.y + (ENEMY_BLOCK / 2) + (EXPLOSION_BLOCK / 2))
					{
						HitEnemy(nCntEnemy, 1, true);
					}
				}
			}
			for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++, pBoss++)
			{
				if (pBoss->bUse == true && pBoss->state != BOSSSTATE_BARRIER && pBoss->state != BOSSSTATE_DEATH)
				{
					if (pBoss->type == BOSS_XU_44 || pBoss->type == BOSS_XU_80)
					{
						if (pBoss->pos.x - 50 - (EXPLOSION_BLOCK / 2) <= g_aExplosion.pos.x && g_aExplosion.pos.x <= pBoss->pos.x + 50 + (EXPLOSION_BLOCK / 2) &&
							pBoss->pos.y - 65 - (EXPLOSION_BLOCK / 2) <= g_aExplosion.pos.y && g_aExplosion.pos.y <= pBoss->pos.y + 65 + (EXPLOSION_BLOCK / 2))
						{
							HitBoss(nCntBoss, 1, true);
						}
					}
					else if (pBoss->type == BOSS_ENGINE)
					{
						if (pBoss->pos.x - 35 - (EXPLOSION_BLOCK / 2) <= g_aExplosion.pos.x && g_aExplosion.pos.x <= pBoss->pos.x + 35 + (EXPLOSION_BLOCK / 2) &&
							pBoss->pos.y - 35 - (EXPLOSION_BLOCK / 2) <= g_aExplosion.pos.y && g_aExplosion.pos.y <= pBoss->pos.y + 35 + (EXPLOSION_BLOCK / 2))
						{
							HitBoss(nCntBoss, 1, true);
						}
					}
				}
			}
			for (nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++, pBarrier++)
			{
				if (pBarrier->bUse == true && pBarrier->ntype == 1)
				{
					if (pBarrier->pos.x - BARRIER_WIDTH - (EXPLOSION_BLOCK / 2) <= g_aExplosion.pos.x && g_aExplosion.pos.x <= pBarrier->pos.x + BARRIER_WIDTH + (EXPLOSION_BLOCK / 2) &&
						pBarrier->pos.y - BARRIER_HEIGHT - (EXPLOSION_BLOCK / 2) <= g_aExplosion.pos.y && g_aExplosion.pos.y <= pBarrier->pos.y + BARRIER_HEIGHT + (EXPLOSION_BLOCK / 2))
					{
						HitBarrier(1, nCntBarrier);
					}
				}
			}
		}
		g_pVtxBuffExplosion->Lock(0, 0, (void **)&pVtx, 0);
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion.pos.x - (EXPLOSION_BLOCK / 2) * g_aExplosion.fBig, g_aExplosion.pos.y - (EXPLOSION_BLOCK / 2) * g_aExplosion.fBig, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion.pos.x + (EXPLOSION_BLOCK / 2) * g_aExplosion.fBig, g_aExplosion.pos.y - (EXPLOSION_BLOCK / 2) * g_aExplosion.fBig, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion.pos.x - (EXPLOSION_BLOCK / 2) * g_aExplosion.fBig, g_aExplosion.pos.y + (EXPLOSION_BLOCK / 2) * g_aExplosion.fBig, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion.pos.x + (EXPLOSION_BLOCK / 2) * g_aExplosion.fBig, g_aExplosion.pos.y + (EXPLOSION_BLOCK / 2) * g_aExplosion.fBig, 0.0f);
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 180 - g_aExplosion.nClear);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 180 - g_aExplosion.nClear);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 180 - g_aExplosion.nClear);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 180 - g_aExplosion.nClear);
		g_pVtxBuffExplosion->Unlock();
	}
}

//描画処理
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	pDevice = GetDevice();                        //デバイスを取得する
												  //頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureExplosion);    //テクスチャの設定
	if (g_aExplosion.bUse == true)
	{
		//ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
	}
}

//爆発の設定処理
void SetExplosion(D3DXVECTOR3 pos)
{
	VERTEX_2D *pVtx;
	g_pVtxBuffExplosion->Lock(0, 0, (void **)&pVtx, 0);
	g_aExplosion.pos = pos;
	pVtx[0].pos = D3DXVECTOR3(g_aExplosion.pos.x - (EXPLOSION_BLOCK / 2), g_aExplosion.pos.y - (EXPLOSION_BLOCK / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aExplosion.pos.x + (EXPLOSION_BLOCK / 2), g_aExplosion.pos.y - (EXPLOSION_BLOCK / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aExplosion.pos.x - (EXPLOSION_BLOCK / 2), g_aExplosion.pos.y + (EXPLOSION_BLOCK / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aExplosion.pos.x + (EXPLOSION_BLOCK / 2), g_aExplosion.pos.y + (EXPLOSION_BLOCK / 2), 0.0f);
	g_aExplosion.fBig = 0.0f;
	g_aExplosion.nTime = 0;
	g_aExplosion.nClear = 0;
	g_aExplosion.bUse = true;
	g_pVtxBuffExplosion->Unlock();
}

void ResetExplosion(void)
{
	g_aExplosion.bUse = false;
}