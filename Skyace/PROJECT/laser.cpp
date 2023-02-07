//--------------------------
//Author:三上航世
//レーザーの処理(laser.cpp)
//--------------------------
#include "laser.h"
#include "enemy.h"
#include "aircraft.h"
#include "boss.h"
#include "barrier.h"
#include "sound.h"

//マクロ定義
#define MAX_LASER (2)   //レーザーの最大数
#define LASER_WIDTH (24.0f)  //レーザーの幅
#define LASER_HEIGHT (900.0f) //レーザーの長さ

//レーザーの構造体定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int ntype;			//敵か味方か
	int nLife;			//寿命
	float fLength;		//長さ
	int nClear;			//透明度
	float fWidth;		//太さ
	int nGather;		//最初の縮小
	bool bDamage;		//当たり判定
	bool bUse;			//使用しているかどうか
} Laser;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureLaser[3] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLaser = NULL;	//頂点バッファへのポインタ
Laser g_aLaser[MAX_LASER];						//最大レーザー

//初期化処理
void InitLaser(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntLaser;
	pDevice = GetDevice();

	//レーザーの初期化
	for (nCntLaser = 0; nCntLaser < MAX_LASER; nCntLaser++)
	{
		g_aLaser[nCntLaser].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aLaser[nCntLaser].nLife = 0;
		g_aLaser[nCntLaser].ntype = 0;
		g_aLaser[nCntLaser].fLength = 0.0f;
		g_aLaser[nCntLaser].fWidth = 0.0f;
		g_aLaser[nCntLaser].nGather = 180;
		g_aLaser[nCntLaser].bUse = false;
		g_aLaser[nCntLaser].bDamage = false;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/bluelaser.png",
		&g_pTextureLaser[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/redlaser.png",
		&g_pTextureLaser[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/レーザー収束.png",
		&g_pTextureLaser[2]);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LASER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLaser,
		NULL);

	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffLaser->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntLaser = 0; nCntLaser < MAX_LASER; nCntLaser++)
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
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 180);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 180);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 180);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 180);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffLaser->Unlock();
}

//レーザーの終了処理
void UninitLaser(void)
{
	int nCntLaser;
	//テクスチャの破棄
	for (nCntLaser = 0; nCntLaser < MAX_LASER; nCntLaser++)
	{
		if (g_pTextureLaser[nCntLaser] != NULL)
		{
			g_pTextureLaser[nCntLaser]->Release();
			g_pTextureLaser[nCntLaser] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffLaser != NULL)
	{
		g_pVtxBuffLaser->Release();
		g_pVtxBuffLaser = NULL;
	}
}

//レーザーの更新処理
void UpdateLaser(void)
{
	int nCntLaser;
	VERTEX_2D *pVtx;
	g_pVtxBuffLaser->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntLaser = 0; nCntLaser < MAX_LASER; nCntLaser++)
	{
		//使用中
		if (g_aLaser[nCntLaser].bUse == true)
		{
			int nCntEnemy;
			Enemy *pEnemy;
			pEnemy = GetEnemy();
			Aircraft *pAircraft;
			pAircraft = GetAircraft();
			int nCntBoss;
			Boss *pBoss;
			pBoss = GetBoss();
			Barrier *pBarrier;
			pBarrier = GetBarrier();
			int nCntBarrier;

			//寿命減らす
			g_aLaser[nCntLaser].nLife--;

			//自分のレーザー
			if (g_aLaser[nCntLaser].ntype == 0)
			{
				//場所設定
				g_aLaser[nCntLaser].pos = pAircraft->pos;
			}
			else
			{
				for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++,pBoss++)
				{
					//ボスの場所に設定
					if (pBoss->type == BOSS_SEAGULL || pBoss->type == BOSS_XU_80)
					{
						g_aLaser[nCntLaser].pos = pBoss->pos;
						break;
					}
				}
			}

			//敵のレーザー、寿命一定以上ある
			if (g_aLaser[nCntLaser].ntype == 1 && g_aLaser[nCntLaser].nLife >= 240)
			{
				//最初の収束させる
				g_aLaser[nCntLaser].nGather -= 3;

				//寿命が一定になると、音鳴らす
				if (g_aLaser[nCntLaser].nLife == 240)
				{
					PlaySound(SOUND_LABEL_LASER000);
				}
			}

			//発射してすぐ、自機の物
			if (g_aLaser[nCntLaser].nLife >= 465 && g_aLaser[nCntLaser].ntype == 0)
			{
				g_aLaser[nCntLaser].fLength += LASER_HEIGHT / 30.0f;
			}
			//発射してすぐ、敵のもの、最初の収束終わってる
			else if (g_aLaser[nCntLaser].nLife >= 225 && g_aLaser[nCntLaser].ntype == 1 && g_aLaser[nCntLaser].nGather <= 0)
			{
				g_aLaser[nCntLaser].fLength += LASER_HEIGHT / 30.0f;
			}
			if ((g_aLaser[nCntLaser].nLife + 1) % 15 == 0) //15フレームごとにダメージ
			{
				g_aLaser[nCntLaser].bDamage = true;
			}
			//ダメージ与えない
			else
			{
				g_aLaser[nCntLaser].bDamage = false;
			}

			if (g_aLaser[nCntLaser].nLife == 0) //8秒か約3秒で照射終了
			{
				//消す
				g_aLaser[nCntLaser].bUse = false;
			}

			if (g_aLaser[nCntLaser].nLife < 60) //終わりかけに色が薄くなる
			{
				g_aLaser[nCntLaser].nClear += 2;
			}

			if (g_aLaser[nCntLaser].nLife < 30 && g_aLaser[nCntLaser].nLife % 3 == 0) //終わりかけに細くなる
			{
				g_aLaser[nCntLaser].fWidth -= 1.0f;
				//敵のレーザーはさらに細くする
				if (g_aLaser[nCntLaser].ntype == 1)
				{
					g_aLaser[nCntLaser].fWidth -= 9.0f;
				}
			}

			//自機のもの
			if (g_aLaser[nCntLaser].ntype == 0)
			{
				//長さの分posを移動させる
				g_aLaser[nCntLaser].pos.y -= g_aLaser[nCntLaser].fLength;
			}
			//敵のもの
			else
			{
				//長さの分posを移動させる
				g_aLaser[nCntLaser].pos.y += g_aLaser[nCntLaser].fLength;
			}

			//敵のものかつ収束中
			if (g_aLaser[nCntLaser].ntype == 1 && g_aLaser[nCntLaser].nGather > 0)
			{
				pVtx[0].pos = D3DXVECTOR3(g_aLaser[nCntLaser].pos.x - g_aLaser[nCntLaser].nGather, g_aLaser[nCntLaser].pos.y - g_aLaser[nCntLaser].nGather, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aLaser[nCntLaser].pos.x + g_aLaser[nCntLaser].nGather, g_aLaser[nCntLaser].pos.y - g_aLaser[nCntLaser].nGather, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aLaser[nCntLaser].pos.x - g_aLaser[nCntLaser].nGather, g_aLaser[nCntLaser].pos.y + g_aLaser[nCntLaser].nGather, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aLaser[nCntLaser].pos.x + g_aLaser[nCntLaser].nGather, g_aLaser[nCntLaser].pos.y + g_aLaser[nCntLaser].nGather, 0.0f);
			}
			//自分のもの、または敵のレーザー発射中
			else
			{
				pVtx[0].pos = D3DXVECTOR3(g_aLaser[nCntLaser].pos.x - (g_aLaser[nCntLaser].fWidth / 2.0f), g_aLaser[nCntLaser].pos.y - g_aLaser[nCntLaser].fLength, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aLaser[nCntLaser].pos.x + (g_aLaser[nCntLaser].fWidth / 2.0f), g_aLaser[nCntLaser].pos.y - g_aLaser[nCntLaser].fLength, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aLaser[nCntLaser].pos.x - (g_aLaser[nCntLaser].fWidth / 2.0f), g_aLaser[nCntLaser].pos.y + g_aLaser[nCntLaser].fLength, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aLaser[nCntLaser].pos.x + (g_aLaser[nCntLaser].fWidth / 2.0f), g_aLaser[nCntLaser].pos.y + g_aLaser[nCntLaser].fLength, 0.0f);
			}

			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 180 - g_aLaser[nCntLaser].nClear);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 180 - g_aLaser[nCntLaser].nClear);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 180 - g_aLaser[nCntLaser].nClear);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 180 - g_aLaser[nCntLaser].nClear);

			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				//使われてる
				if (pEnemy->bUse == true)
				{
					//敵への当たり判定
					if (pEnemy->pos.x - (ENEMY_BLOCK / 2.0f) - (g_aLaser[nCntLaser].fWidth / 2.0f) <= g_aLaser[nCntLaser].pos.x &&
						g_aLaser[nCntLaser].pos.x <= pEnemy->pos.x + (ENEMY_BLOCK / 2.0f) + (g_aLaser[nCntLaser].fWidth / 2.0f) &&
						pEnemy->pos.y - (ENEMY_BLOCK / 2.0f) - g_aLaser[nCntLaser].fLength <= g_aLaser[nCntLaser].pos.y &&
						g_aLaser[nCntLaser].pos.y <= pEnemy->pos.y + (ENEMY_BLOCK / 2.0f) + g_aLaser[nCntLaser].fLength)
					{
						//自機のもの、レーザーにダメージ判定中
						if (g_aLaser[nCntLaser].ntype == 0 && g_aLaser[nCntLaser].bDamage == true)
						{
							//ダメージ与える
							HitEnemy(nCntEnemy, 2, true);
						}
					}
				}
			}

			for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++, pBoss++)
			{
				//使われてる、やられてない、バリアじゃない
				if (pBoss->bUse == true && pBoss->state != BOSSSTATE_DEATH && pBoss->state != BOSSSTATE_BARRIER)
				{
					//特定の敵
					if (pBoss->type == BOSS_XU_44 || pBoss->type == BOSS_XU_80)
					{
						//当たり判定
						if (pBoss->pos.x - 50.0f - (g_aLaser[nCntLaser].fWidth / 2.0f) <= g_aLaser[nCntLaser].pos.x &&
							g_aLaser[nCntLaser].pos.x <= pBoss->pos.x + 50.0f + (g_aLaser[nCntLaser].fWidth / 2.0f) &&
							pBoss->pos.y - 65.0f - g_aLaser[nCntLaser].fLength <= g_aLaser[nCntLaser].pos.y &&
							g_aLaser[nCntLaser].pos.y <= pBoss->pos.y + 65.0f + g_aLaser[nCntLaser].fLength)
						{
							//自機のもの、レーザーにダメージ判定中
							if (g_aLaser[nCntLaser].ntype == 0 && g_aLaser[nCntLaser].bDamage == true)
							{
								//ダメージ与える
								HitBoss(nCntBoss, 2, true);
							}
						}
					}
					//特定の敵
					else if (pBoss->type == BOSS_ENGINE)
					{
						//当たり判定
						if (pBoss->pos.x - 35.0f - (g_aLaser[nCntLaser].fWidth / 2.0f) <= g_aLaser[nCntLaser].pos.x &&
							g_aLaser[nCntLaser].pos.x <= pBoss->pos.x + 35.0f + (g_aLaser[nCntLaser].fWidth / 2.0f) &&
							pBoss->pos.y - 35.0f - g_aLaser[nCntLaser].fLength <= g_aLaser[nCntLaser].pos.y &&
							g_aLaser[nCntLaser].pos.y <= pBoss->pos.y + 35.0f + g_aLaser[nCntLaser].fLength)
						{
							//自機のもの、レーザーダメージ判定中
							if (g_aLaser[nCntLaser].ntype == 0 && g_aLaser[nCntLaser].bDamage == true)
							{
								//ダメージ与える
								HitBoss(nCntBoss, 2, true);
							}
						}
					}
				}
			}

			for (nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++, pBarrier++)
			{
				//バリアへの当たり判定
				if (pBarrier->bUse == true)
				{
					//当たり判定
					if (pBarrier->pos.x - (BARRIER_WIDTH / 2.0f) - (g_aLaser[nCntLaser].fWidth / 2.0f) <= g_aLaser[nCntLaser].pos.x &&
						g_aLaser[nCntLaser].pos.x <= pBarrier->pos.x + (BARRIER_WIDTH / 2.0f) + (g_aLaser[nCntLaser].fWidth / 2.0f) &&
						pBarrier->pos.y - (BARRIER_HEIGHT / 2.0f) - g_aLaser[nCntLaser].fLength <= g_aLaser[nCntLaser].pos.y &&
						g_aLaser[nCntLaser].pos.y <= pBarrier->pos.y + (BARRIER_HEIGHT / 2.0f) + g_aLaser[nCntLaser].fLength)
					{
						//敵のレーザー、バリアの状態ノーマル、自機のバリア
						if (g_aLaser[nCntLaser].ntype == 1 && pBarrier->state == BARRIERSTATE_NORMAL && pBarrier->ntype == 0)
						{
							HitBarrier(1,nCntBarrier);
						}
						//自機のレーザー、敵のバリア、レーザー当たり判定中
						else if (g_aLaser[nCntLaser].ntype == 0 && pBarrier->ntype == 1 && g_aLaser[nCntLaser].bDamage == true)
						{
							HitBarrier(2, nCntBarrier);
						}
					}
				}
			}

			//自機との当たり判定
			if (pAircraft->pos.x - (AIRCRAFT_WIDTH / 2.0f) - (g_aLaser[nCntLaser].fWidth / 2.0f) <= g_aLaser[nCntLaser].pos.x &&
				g_aLaser[nCntLaser].pos.x <= pAircraft->pos.x + (AIRCRAFT_WIDTH / 2.0f) + (g_aLaser[nCntLaser].fWidth / 2.0f) &&
				pAircraft->pos.y - (AIRCRAFT_HEIGHT / 2.0f) - g_aLaser[nCntLaser].fLength <= g_aLaser[nCntLaser].pos.y &&
				g_aLaser[nCntLaser].pos.y <= pAircraft->pos.y + (AIRCRAFT_HEIGHT / 2.0f) + g_aLaser[nCntLaser].fLength)
			{
				//敵のレーザー、自機の状態ノーマル
				if (g_aLaser[nCntLaser].ntype == 1 && pAircraft->state == AIRCRAFTSTATE_NORMAL)
				{
					HitAircraft();
				}
			}
		}
		pVtx += 4;
	}
	g_pVtxBuffLaser->Unlock();
}

//描画処理
void DrawLaser(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	int nCntLaser;
	pDevice = GetDevice();                        //デバイスを取得する
												  //頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLaser, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	for (nCntLaser = 0; nCntLaser < MAX_LASER; nCntLaser++)
	{
		//使用中
		if (g_aLaser[nCntLaser].bUse == true)
		{
			//敵のレーザー、寿命一定以上
			if (g_aLaser[nCntLaser].ntype == 1 && g_aLaser[nCntLaser].nLife >= 240)
			{
				pDevice->SetTexture(0, g_pTextureLaser[2]);    //テクスチャの設定
			}
			//それ以外
			else
			{
				pDevice->SetTexture(0, g_pTextureLaser[g_aLaser[nCntLaser].ntype]);    //テクスチャの設定
			}
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntLaser * 4,
				2);
		}
	}
}

//レーザーの設定処理
void SetLaser(D3DXVECTOR3 pos, int ntype)
{
	int nCntLaser;
	VERTEX_2D *pVtx;
	g_pVtxBuffLaser->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntLaser = 0; nCntLaser < MAX_LASER; nCntLaser++)
	{
		//使ってない
		if (g_aLaser[nCntLaser].bUse == false)
		{
			//色々設定
			g_aLaser[nCntLaser].pos = pos;
			pVtx[0].pos = D3DXVECTOR3(g_aLaser[nCntLaser].pos.x - 240.0f, g_aLaser[nCntLaser].pos.y - 240.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aLaser[nCntLaser].pos.x + 240.0f, g_aLaser[nCntLaser].pos.y - 240.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aLaser[nCntLaser].pos.x - 240.0f, g_aLaser[nCntLaser].pos.y + 240.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aLaser[nCntLaser].pos.x + 240.0f, g_aLaser[nCntLaser].pos.y + 240.0f, 0.0f);
			g_aLaser[nCntLaser].bDamage = false;
			g_aLaser[nCntLaser].fLength = 0.0f;
			g_aLaser[nCntLaser].bUse = true;
			g_aLaser[nCntLaser].ntype = ntype;
			g_aLaser[nCntLaser].nGather = 360;
			g_aLaser[nCntLaser].nClear = 0;
			if (ntype == 0)
			{
				g_aLaser[nCntLaser].nLife = 480;
				g_aLaser[nCntLaser].fWidth = LASER_WIDTH;
				PlaySound(SOUND_LABEL_LASER000);
			}
			else
			{
				g_aLaser[nCntLaser].nLife = 360;
				g_aLaser[nCntLaser].fWidth = 120.0f;
			}
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffLaser->Unlock();
}

//リセット
void ResetLaser(void)
{
	int nCntLaser;
	for (nCntLaser = 0; nCntLaser < MAX_LASER; nCntLaser++)
	{
		g_aLaser[nCntLaser].bUse = false;
	}
}