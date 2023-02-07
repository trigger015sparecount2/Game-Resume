//--------------------------
//Author:三上航世
//バリアの処理(barrier.cpp)
//--------------------------
#include "barrier.h"
#include "aircraft.h"
#include "boss.h"
#include "sound.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBarrier = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBarrier = NULL; //頂点バッファへのポインタ
Barrier g_aBarrier[MAX_BARRIER];

//初期化処理
void InitBarrier(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	int nCntBarrier;
	//バリアの初期化
	for (nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
	{
		g_aBarrier[nCntBarrier].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBarrier[nCntBarrier].state = BARRIERSTATE_NORMAL;
		g_aBarrier[nCntBarrier].nLife = 0;
		g_aBarrier[nCntBarrier].bBlight = true;
		g_aBarrier[nCntBarrier].nClear = 0;
		g_aBarrier[nCntBarrier].fBig = 0.0f;
		g_aBarrier[nCntBarrier].nDTime = 0;
		g_aBarrier[nCntBarrier].nDClear = 0;
		g_aBarrier[nCntBarrier].ntype = 0;
		g_aBarrier[nCntBarrier].bUse = false;
	}
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/barrier.png",
		&g_pTextureBarrier);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BARRIER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBarrier,
		NULL);
	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBarrier->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
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
	g_pVtxBuffBarrier->Unlock();
}

//バリアの終了処理
void UninitBarrier(void)
{
	//テクスチャの破棄
	if (g_pTextureBarrier != NULL)
	{
		g_pTextureBarrier->Release();
		g_pTextureBarrier = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBarrier != NULL)
	{
		g_pVtxBuffBarrier->Release();
		g_pVtxBuffBarrier = NULL;
	}
}

//バリアの更新処理
void UpdateBarrier(void)
{
	int nCntBarrier;
	VERTEX_2D *pVtx;
	g_pVtxBuffBarrier->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
	{
		if (g_aBarrier[nCntBarrier].bUse == true)
		{
			switch (g_aBarrier[nCntBarrier].state)
			{
			case BARRIERSTATE_NORMAL:	//普通
				if (g_aBarrier[nCntBarrier].ntype == 0)	//自分
				{
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 60 + g_aBarrier[nCntBarrier].nClear);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 60 + g_aBarrier[nCntBarrier].nClear);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 60 + g_aBarrier[nCntBarrier].nClear);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 60 + g_aBarrier[nCntBarrier].nClear);
				}
				else	//相手
				{
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 2 + g_aBarrier[nCntBarrier].nClear);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 2 + g_aBarrier[nCntBarrier].nClear);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 2 + g_aBarrier[nCntBarrier].nClear);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 2 + g_aBarrier[nCntBarrier].nClear);
				}
				break;
			case BARRIERSTATE_DAMAGE:	//ダメージ受けた
				g_aBarrier[nCntBarrier].nDTime--;
				g_aBarrier[nCntBarrier].nDClear--;
				if (g_aBarrier[nCntBarrier].nDTime <= 475)
				{
					if (g_aBarrier[nCntBarrier].ntype == 0)	//自分
					{
						g_aBarrier[nCntBarrier].state = BARRIERSTATE_NODAMAGE;	//無敵
					}
					else	//敵
					{
						g_aBarrier[nCntBarrier].state = BARRIERSTATE_NORMAL;	//普通
					}
				}
				break;
			case BARRIERSTATE_NODAMAGE:	//無敵
				g_aBarrier[nCntBarrier].nDTime--;
				g_aBarrier[nCntBarrier].nDClear--;
				//時間に余裕ある
				if (g_aBarrier[nCntBarrier].nDTime > 140)
				{
					//点滅
					if (g_aBarrier[nCntBarrier].nDClear > 10)
					{
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 60 + g_aBarrier[nCntBarrier].nClear);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 60 + g_aBarrier[nCntBarrier].nClear);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 60 + g_aBarrier[nCntBarrier].nClear);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 60 + g_aBarrier[nCntBarrier].nClear);
					}
					else
					{
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
						if (g_aBarrier[nCntBarrier].nDClear < 0)
						{
							g_aBarrier[nCntBarrier].nDClear = 20;
						}
					}
				}
				//もうすぐ終わる
				else
				{
					//点滅
					if (g_aBarrier[nCntBarrier].nDClear > 5)
					{
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 60 + g_aBarrier[nCntBarrier].nClear);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 60 + g_aBarrier[nCntBarrier].nClear);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 60 + g_aBarrier[nCntBarrier].nClear);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 60 + g_aBarrier[nCntBarrier].nClear);
					}
					else
					{
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
						if (g_aBarrier[nCntBarrier].nDClear < 0)
						{
							g_aBarrier[nCntBarrier].nDClear = 10;
						}
					}
				}

				//時間終わり
				if (g_aBarrier[nCntBarrier].nDTime <= 0)
				{
					g_aBarrier[nCntBarrier].state = BARRIERSTATE_NORMAL;
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 60 + g_aBarrier[nCntBarrier].nClear);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 60 + g_aBarrier[nCntBarrier].nClear);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 60 + g_aBarrier[nCntBarrier].nClear);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aBarrier[nCntBarrier].nLife * 60 + g_aBarrier[nCntBarrier].nClear);
				}
				break;
			}

			//自分
			if (g_aBarrier[nCntBarrier].ntype == 0)
			{
				Aircraft *pAircraft;
				pAircraft = GetAircraft();
				g_aBarrier[nCntBarrier].pos = pAircraft->pos;
			}
			//敵
			else
			{
				Boss *pBoss;
				pBoss = GetBoss();
				int nCntBoss;
				for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++,pBoss++)
				{
					g_aBarrier[nCntBarrier].pos = pBoss->pos;
					break;
				}
			}

			//明るくする
			if (g_aBarrier[nCntBarrier].bBlight == true)
			{
				g_aBarrier[nCntBarrier].nClear += 2;
				if (g_aBarrier[nCntBarrier].nClear >= 40)
				{
					g_aBarrier[nCntBarrier].bBlight = false;
				}
			}
			//暗くする
			else
			{
				g_aBarrier[nCntBarrier].nClear -= 2;
				if (g_aBarrier[nCntBarrier].nClear <= 0)
				{
					g_aBarrier[nCntBarrier].bBlight = true;
				}
			}

			//大きくなる
			g_aBarrier[nCntBarrier].fBig += 0.05f;
			if (g_aBarrier[nCntBarrier].fBig >= 1.0f)
			{
				g_aBarrier[nCntBarrier].fBig = 1.0f;
			}
			pVtx[0].pos = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x - (BARRIER_WIDTH / 2.0f) * g_aBarrier[nCntBarrier].fBig, g_aBarrier[nCntBarrier].pos.y - (BARRIER_HEIGHT / 2.0f) * g_aBarrier[nCntBarrier].fBig, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + (BARRIER_WIDTH / 2.0f) * g_aBarrier[nCntBarrier].fBig, g_aBarrier[nCntBarrier].pos.y - (BARRIER_HEIGHT / 2.0f) * g_aBarrier[nCntBarrier].fBig, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x - (BARRIER_WIDTH / 2.0f) * g_aBarrier[nCntBarrier].fBig, g_aBarrier[nCntBarrier].pos.y + (BARRIER_HEIGHT / 2.0f) * g_aBarrier[nCntBarrier].fBig, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + (BARRIER_WIDTH / 2.0f) * g_aBarrier[nCntBarrier].fBig, g_aBarrier[nCntBarrier].pos.y + (BARRIER_HEIGHT / 2.0f) * g_aBarrier[nCntBarrier].fBig, 0.0f);
		}
		pVtx += 4;
	}
	g_pVtxBuffBarrier->Unlock();
}

//描画処理
void DrawBarrier(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	pDevice = GetDevice();                        //デバイスを取得する
	int nCntBarrier;

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBarrier, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureBarrier);    //テクスチャの設定
	for (nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
	{
		//使用中のみ描画
		if (g_aBarrier[nCntBarrier].bUse == true)
		{
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntBarrier * 4,
				2);
		}
	}
}

//バリアの設定処理
void SetBarrier(D3DXVECTOR3 pos, int nLife, int ntype)
{
	int nCntBarrier;
	for (nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
	{
		if (g_aBarrier[nCntBarrier].bUse == false)
		{
			//ステータス設定
			g_aBarrier[nCntBarrier].pos = pos;
			g_aBarrier[nCntBarrier].fBig = 0.0f;
			g_aBarrier[nCntBarrier].bUse = true;
			g_aBarrier[nCntBarrier].bBlight = true;
			g_aBarrier[nCntBarrier].nLife = nLife;
			g_aBarrier[nCntBarrier].nClear = 0;
			g_aBarrier[nCntBarrier].nDTime = 0;
			g_aBarrier[nCntBarrier].nDClear = 0;
			g_aBarrier[nCntBarrier].ntype = ntype;
			g_aBarrier[nCntBarrier].state = BARRIERSTATE_NORMAL;

			VERTEX_2D *pVtx;
			g_pVtxBuffBarrier->Lock(0, 0, (void **)&pVtx, 0);
			pVtx += nCntBarrier * 4;
			pVtx[0].pos = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x - (BARRIER_WIDTH / 2.0f) * g_aBarrier[nCntBarrier].fBig, g_aBarrier[nCntBarrier].pos.y - (BARRIER_HEIGHT / 2.0f) * g_aBarrier[nCntBarrier].fBig, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + (BARRIER_WIDTH / 2.0f) * g_aBarrier[nCntBarrier].fBig, g_aBarrier[nCntBarrier].pos.y - (BARRIER_HEIGHT / 2.0f) * g_aBarrier[nCntBarrier].fBig, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x - (BARRIER_WIDTH / 2.0f) * g_aBarrier[nCntBarrier].fBig, g_aBarrier[nCntBarrier].pos.y + (BARRIER_HEIGHT / 2.0f) * g_aBarrier[nCntBarrier].fBig, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBarrier[nCntBarrier].pos.x + (BARRIER_WIDTH / 2.0f) * g_aBarrier[nCntBarrier].fBig, g_aBarrier[nCntBarrier].pos.y + (BARRIER_HEIGHT / 2.0f) * g_aBarrier[nCntBarrier].fBig, 0.0f);
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			PlaySound(SOUND_LABEL_BARRIER000);
			g_pVtxBuffBarrier->Unlock();
			break;
		}
	}
}

//ポインタ
Barrier *GetBarrier(void)
{
	return &g_aBarrier[0];
}

//バリアに当たった
void HitBarrier(int nDamage, int nCntBarrier)
{
	//使われている
	if (g_aBarrier[nCntBarrier].bUse == true)
	{
		//体力減らす
		g_aBarrier[nCntBarrier].nLife -= nDamage;

		//体力0
		if (g_aBarrier[nCntBarrier].nLife <= 0)
		{
			//自分
			if (g_aBarrier[nCntBarrier].ntype == 0)
			{
				Aircraft *pAircraft;
				pAircraft = GetAircraft();
				pAircraft->state = AIRCRAFTSTATE_NORMAL;
			}
			//敵
			else
			{
				Boss * pBoss;
				pBoss = GetBoss();
				int nCntBoss;
				for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++,pBoss++)
				{
					if (pBoss->type == BOSS_XU_80)
					{
						pBoss->state = BOSSSTATE_NORMAL;
					}
				}
			}
			g_aBarrier[nCntBarrier].bUse = false;
		}

		//バリアのステータス設定
		g_aBarrier[nCntBarrier].nDTime = 480;
		g_aBarrier[nCntBarrier].nDClear = 20;
		g_aBarrier[nCntBarrier].state = BARRIERSTATE_DAMAGE;

		//赤くする
		VERTEX_2D *pVtx;
		g_pVtxBuffBarrier->Lock(0, 0, (void **)&pVtx, 0);
		pVtx += nCntBarrier * 4;
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		g_pVtxBuffBarrier->Unlock();
	}
}

//リセット
void ResetBarrier(void)
{
	int nCntBarrier;
	for (nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++)
	{
		g_aBarrier[nCntBarrier].bUse = false;
	}
}