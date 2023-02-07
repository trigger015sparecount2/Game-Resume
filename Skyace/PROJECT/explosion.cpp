//--------------------------
//Author:三上航世
//爆発の処理(explosion.cpp)
//--------------------------
#include "explosion.h"
#include "enemy.h"
#include "boss.h"
#include "barrier.h"

//マクロ定義
#define EXPLOSION_BLOCK (400.0f)  //爆発の大きさ
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
	//使用中
	if (g_aExplosion.bUse == true)
	{
		//時間進める
		g_aExplosion.nTime++;
		g_aExplosion.pos.y += 0.75f;

		//時間進んだ
		if (g_aExplosion.nTime == EXPLOSION_LIFE)
		{
			//消す
			g_aExplosion.bUse = false;
		}

		//時間進んでない
		if (g_aExplosion.nTime <= 20)
		{
			//どんどん大きくする
			g_aExplosion.fBig += 0.05f;
		}
		else if (g_aExplosion.nTime > EXPLOSION_LIFE - 160)
		{
			//どんどん薄くする
			g_aExplosion.nClear++;
		}

		//10フレームに1フレーム当たり判定
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

			//雑魚敵
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				//使われてる
				if (pEnemy->bUse == true)
				{
					//当たり判定
					if (pEnemy->pos.x - (ENEMY_BLOCK / 2.0f) - (EXPLOSION_BLOCK / 2.0f) <= g_aExplosion.pos.x &&
						g_aExplosion.pos.x <= pEnemy->pos.x + (ENEMY_BLOCK / 2.0f) + (EXPLOSION_BLOCK / 2.0f) &&
						pEnemy->pos.y - (ENEMY_BLOCK / 2.0f) - (EXPLOSION_BLOCK / 2.0f) <= g_aExplosion.pos.y &&
						g_aExplosion.pos.y <= pEnemy->pos.y + (ENEMY_BLOCK / 2.0f) + (EXPLOSION_BLOCK / 2.0f))
					{
						HitEnemy(nCntEnemy, 1, true);
					}
				}
			}

			//ボス敵
			for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++, pBoss++)
			{
				//使われてる、状態ノーマル、やられてない
				if (pBoss->bUse == true && pBoss->state != BOSSSTATE_BARRIER && pBoss->state != BOSSSTATE_DEATH)
				{
					//特定の敵
					if (pBoss->type == BOSS_XU_44 || pBoss->type == BOSS_XU_80)
					{
						//当たり判定
						if (pBoss->pos.x - 50.0f - (EXPLOSION_BLOCK / 2.0f) <= g_aExplosion.pos.x &&
							g_aExplosion.pos.x <= pBoss->pos.x + 50.0f + (EXPLOSION_BLOCK / 2.0f) &&
							pBoss->pos.y - 65.0f - (EXPLOSION_BLOCK / 2.0f) <= g_aExplosion.pos.y &&
							g_aExplosion.pos.y <= pBoss->pos.y + 65.0f + (EXPLOSION_BLOCK / 2.0f))
						{
							HitBoss(nCntBoss, 1, true);
						}
					}
					//特定の敵
					else if (pBoss->type == BOSS_ENGINE)
					{
						//当たり判定
						if (pBoss->pos.x - 35.0f - (EXPLOSION_BLOCK / 2.0f) <= g_aExplosion.pos.x &&
							g_aExplosion.pos.x <= pBoss->pos.x + 35.0f + (EXPLOSION_BLOCK / 2.0f) &&
							pBoss->pos.y - 35.0f - (EXPLOSION_BLOCK / 2.0f) <= g_aExplosion.pos.y &&
							g_aExplosion.pos.y <= pBoss->pos.y + 35.0f + (EXPLOSION_BLOCK / 2.0f))
						{
							HitBoss(nCntBoss, 1, true);
						}
					}
				}
			}

			//バリア
			for (nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++, pBarrier++)
			{
				//使われてる、敵のバリア
				if (pBarrier->bUse == true && pBarrier->ntype == 1)
				{
					//当たり判定
					if (pBarrier->pos.x - BARRIER_WIDTH - (EXPLOSION_BLOCK / 2.0f) <= g_aExplosion.pos.x &&
						g_aExplosion.pos.x <= pBarrier->pos.x + BARRIER_WIDTH + (EXPLOSION_BLOCK / 2.0f) &&
						pBarrier->pos.y - BARRIER_HEIGHT - (EXPLOSION_BLOCK / 2.0f) <= g_aExplosion.pos.y &&
						g_aExplosion.pos.y <= pBarrier->pos.y + BARRIER_HEIGHT + (EXPLOSION_BLOCK / 2.0f))
					{
						HitBarrier(1, nCntBarrier);
					}
				}
			}
		}

		//位置や色変更
		g_pVtxBuffExplosion->Lock(0, 0, (void **)&pVtx, 0);
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion.pos.x - (EXPLOSION_BLOCK / 2.0f) * g_aExplosion.fBig, g_aExplosion.pos.y - (EXPLOSION_BLOCK / 2.0f) * g_aExplosion.fBig, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion.pos.x + (EXPLOSION_BLOCK / 2.0f) * g_aExplosion.fBig, g_aExplosion.pos.y - (EXPLOSION_BLOCK / 2.0f) * g_aExplosion.fBig, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion.pos.x - (EXPLOSION_BLOCK / 2.0f) * g_aExplosion.fBig, g_aExplosion.pos.y + (EXPLOSION_BLOCK / 2.0f) * g_aExplosion.fBig, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion.pos.x + (EXPLOSION_BLOCK / 2.0f) * g_aExplosion.fBig, g_aExplosion.pos.y + (EXPLOSION_BLOCK / 2.0f) * g_aExplosion.fBig, 0.0f);
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
	
	//使用中
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
	pVtx[0].pos = D3DXVECTOR3(g_aExplosion.pos.x - (EXPLOSION_BLOCK / 2.0f), g_aExplosion.pos.y - (EXPLOSION_BLOCK / 2.0f), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aExplosion.pos.x + (EXPLOSION_BLOCK / 2.0f), g_aExplosion.pos.y - (EXPLOSION_BLOCK / 2.0f), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aExplosion.pos.x - (EXPLOSION_BLOCK / 2.0f), g_aExplosion.pos.y + (EXPLOSION_BLOCK / 2.0f), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aExplosion.pos.x + (EXPLOSION_BLOCK / 2.0f), g_aExplosion.pos.y + (EXPLOSION_BLOCK / 2.0f), 0.0f);
	g_aExplosion.fBig = 0.0f;
	g_aExplosion.nTime = 0;
	g_aExplosion.nClear = 0;
	g_aExplosion.bUse = true;
	g_pVtxBuffExplosion->Unlock();
}

//リセット
void ResetExplosion(void)
{
	g_aExplosion.bUse = false;
}