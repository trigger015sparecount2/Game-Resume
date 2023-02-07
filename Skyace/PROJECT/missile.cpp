//--------------------------
//Author:三上航世
//ミサイルの処理(missile.cpp)
//--------------------------
#include "missile.h"
#include "enemy.h"
#include "smoke.h"
#include "explosion.h"
#include "sound.h"
#include "boss.h"
#include "barrier.h"

//マクロ定義
#define MAX_MISSILE (32)   //ミサイルの最大数
#define MISSILE_WIDTH (20.0f)  //ミサイルの幅
#define MISSILE_HEIGHT (60.0f) //ミサイルの高さ
#define MISSILE_SLIDE_TIME (15) //ミサイルの横移動の時間

//ミサイルの構造体定義
typedef struct
{
	D3DXVECTOR3 pos;  //位置
	D3DXVECTOR3 move; //移動量
	int nTime;        //撃ってからの時間経過
	int nType;        //2種類のうちどちらか
	float fPosx;      //発射時のx座標の位置
	bool bUse;        //使用しているかどうか
} Missile;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureMissile[2] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMissile = NULL;	//頂点バッファへのポインタ
Missile g_aMissile[MAX_MISSILE];					//ミサイル

//初期化処理
void InitMissile(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntMissile;
	pDevice = GetDevice();

	//弾の初期化
	for (nCntMissile = 0; nCntMissile < MAX_MISSILE; nCntMissile++)
	{
		g_aMissile[nCntMissile].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMissile[nCntMissile].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMissile[nCntMissile].nTime = 0;
		g_aMissile[nCntMissile].nType = 0;
		g_aMissile[nCntMissile].fPosx = 0.0f;
		g_aMissile[nCntMissile].bUse = false;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/missile.png",
		&g_pTextureMissile[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/bombmissile.png",
		&g_pTextureMissile[1]);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_MISSILE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMissile,
		NULL);

	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMissile->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntMissile = 0; nCntMissile < MAX_MISSILE; nCntMissile++)
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
	g_pVtxBuffMissile->Unlock();
}

//ミサイルの終了処理
void UninitMissile(void)
{
	int nCntMissile;
	//テクスチャの破棄
	for (nCntMissile = 0; nCntMissile < 2; nCntMissile++)
	{
		if (g_pTextureMissile[nCntMissile] != NULL)
		{
			g_pTextureMissile[nCntMissile]->Release();
			g_pTextureMissile[nCntMissile] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffMissile != NULL)
	{
		g_pVtxBuffMissile->Release();
		g_pVtxBuffMissile = NULL;
	}
}

//ミサイルの更新処理
void UpdateMissile(void)
{
	int nCntMissile;
	VERTEX_2D *pVtx;
	int nNowEnemy = -1;
	int nFirstEnemy = 0;
	Enemy *pFirstEnemy;
	pFirstEnemy = GetEnemy();
	int nNowBoss = -1;
	int nFirstBoss = 0;
	Boss *pFirstBoss;
	pFirstBoss = GetBoss();
	int nCntBarrier;
	Barrier *pBarrier;
	pBarrier = GetBarrier();
	g_pVtxBuffMissile->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntMissile = 0; nCntMissile < MAX_MISSILE; nCntMissile++)
	{
		//使われてる
		if (g_aMissile[nCntMissile].bUse == true)
		{
			int nCntEnemy;
			Enemy *pEnemy;
			pEnemy = GetEnemy();
			int nCntBoss;
			Boss *pBoss;
			pBoss = GetBoss();

			//時間進める
			g_aMissile[nCntMissile].nTime++;
			pVtx[0].pos = D3DXVECTOR3(g_aMissile[nCntMissile].pos.x - (MISSILE_WIDTH / 2.0f), g_aMissile[nCntMissile].pos.y - (MISSILE_HEIGHT / 2.0f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aMissile[nCntMissile].pos.x + (MISSILE_WIDTH / 2.0f), g_aMissile[nCntMissile].pos.y - (MISSILE_HEIGHT / 2.0f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aMissile[nCntMissile].pos.x - (MISSILE_WIDTH / 2.0f), g_aMissile[nCntMissile].pos.y + (MISSILE_HEIGHT / 2.0f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aMissile[nCntMissile].pos.x + (MISSILE_WIDTH / 2.0f), g_aMissile[nCntMissile].pos.y + (MISSILE_HEIGHT / 2.0f), 0.0f);
			
			//時間進んだ
			if (g_aMissile[nCntMissile].nTime > MISSILE_SLIDE_TIME + 15)
			{
				//ミサイル移動、煙出す
				g_aMissile[nCntMissile].pos += g_aMissile[nCntMissile].move;
				SetSmoke(D3DXVECTOR3(g_aMissile[nCntMissile].pos.x, g_aMissile[nCntMissile].pos.y + (MISSILE_HEIGHT / 2), 0.0f));
			}

			//画面外いったら消す
			if (g_aMissile[nCntMissile].pos.y <= -(MISSILE_HEIGHT / 2))
			{
				g_aMissile[nCntMissile].bUse = false;
			}

			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				//敵が使われてるまたは敵のカウントが最後
				if (pEnemy->bUse == true || nCntEnemy == MAX_ENEMY - 1)
				{
					//敵が使われてる
					if (pEnemy->bUse == true)
					{
						//当たり判定
						if (pEnemy->pos.x - (ENEMY_BLOCK / 2.0f) - (MISSILE_WIDTH / 2.0f) <= g_aMissile[nCntMissile].pos.x &&
							g_aMissile[nCntMissile].pos.x <= pEnemy->pos.x + (ENEMY_BLOCK / 2.0f) + (MISSILE_WIDTH / 2.0f) &&
							pEnemy->pos.y - (ENEMY_BLOCK / 2.0f) - (MISSILE_HEIGHT / 2.0f) <= g_aMissile[nCntMissile].pos.y &&
							g_aMissile[nCntMissile].pos.y <= pEnemy->pos.y + (ENEMY_BLOCK / 2.0f) + (MISSILE_HEIGHT / 2.0f))
						{
							PlaySound(SOUND_LABEL_MISSILE000);

							//通常ミサイル
							if (g_aMissile[nCntMissile].nType == 0)
							{
								HitEnemy(nCntEnemy, 8, true);
							}
							//フレアミサイル
							else if(g_aMissile[nCntMissile].nType == 1)
							{
								SetExplosion(g_aMissile[nCntMissile].pos);
								PlaySound(SOUND_LABEL_EXPLOSION000);
							}

							//ミサイル消す、敵のミサイルフラグオフ
							g_aMissile[nCntMissile].bUse = false;
							pEnemy->bMissile = false;
						}
					}

					//直前の敵のカウントよりもカウントが大きいかつ敵が使われてるまたは敵のカウントが最後かつ敵が使われてない
					if (nNowEnemy < nCntEnemy && pEnemy->bUse == true || nCntEnemy == MAX_ENEMY - 1 && pEnemy->bUse == false)
					{
						//直前の敵のカウントが-1かつ敵が使われてる
						if (nNowEnemy == -1 && pEnemy->bUse == true)
						{
							//最初の敵の番号とポインタを記録
							pFirstEnemy = pEnemy;
							nFirstEnemy = nCntEnemy;
						}

						//直前の敵の番号と今の敵の番号が異なる、同じにする
						if (nNowEnemy != nCntEnemy)
						{
							nNowEnemy = nCntEnemy;
						}

						//今の敵の番号が最後かつ敵が使われてない
						if (nCntEnemy == MAX_ENEMY - 1 && pEnemy->bUse == false)
						{
							//最初の敵を対象にする
							pEnemy = pFirstEnemy;
							nNowEnemy = nFirstEnemy;
						}

						//横にスライドする時間中かつ敵が使われてる
						if (g_aMissile[nCntMissile].nTime <= MISSILE_SLIDE_TIME && pEnemy->bUse == true)
						{
							if (g_aMissile[nCntMissile].pos.x < pEnemy->pos.x) //敵がミサイルより右側に居るとき
							{
								g_aMissile[nCntMissile].pos.x += fabsf(g_aMissile[nCntMissile].fPosx - pEnemy->pos.x) / MISSILE_SLIDE_TIME;
							}
							else //敵がミサイルより左側に居るとき
							{
								g_aMissile[nCntMissile].pos.x -= fabsf(g_aMissile[nCntMissile].fPosx - pEnemy->pos.x) / MISSILE_SLIDE_TIME;
							}
							g_aMissile[nCntMissile].pos.y += 1.5f;
							pEnemy->bMissile = true;
						}
						break;
					}
				}
			}
			for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++, pBoss++)
			{
				//ボスが使われてるまたはボスカウントが最後
				if (pBoss->bUse == true || nCntBoss == MAX_ENEMY - 1)
				{
					//ボスが使われてるかつバリアじゃないかつやられてない
					if (pBoss->bUse == true && pBoss->state != BOSSSTATE_BARRIER && pBoss->state != BOSSSTATE_DEATH)
					{
						//特定の種類
						if (pBoss->type == BOSS_XU_44 || pBoss->type == BOSS_XU_80)
						{
							//当たり判定
							if (pBoss->pos.x - 50.0f - (MISSILE_WIDTH / 2.0f) <= g_aMissile[nCntMissile].pos.x &&
								g_aMissile[nCntMissile].pos.x <= pBoss->pos.x + 50.0f + (MISSILE_WIDTH / 2.0f) &&
								pBoss->pos.y - 65.0f - (MISSILE_HEIGHT / 2.0f) <= g_aMissile[nCntMissile].pos.y &&
								g_aMissile[nCntMissile].pos.y <= pBoss->pos.y + 65.0f + (MISSILE_HEIGHT / 2.0f))
							{
								PlaySound(SOUND_LABEL_MISSILE000);

								//通常ミサイル
								if (g_aMissile[nCntMissile].nType == 0)
								{
									HitBoss(nCntBoss, 10, true);
								}
								//フレアミサイル
								else if (g_aMissile[nCntMissile].nType == 1)
								{
									SetExplosion(g_aMissile[nCntMissile].pos);
									PlaySound(SOUND_LABEL_EXPLOSION000);
								}
								//ミサイル消す
								g_aMissile[nCntMissile].bUse = false;
								pBoss->bMissile = false;
							}
						}
						//特定の種類
						else if (pBoss->type == BOSS_ENGINE)
						{
							//当たり判定
							if (pBoss->pos.x - 35.0f - (MISSILE_WIDTH / 2.0f) <= g_aMissile[nCntMissile].pos.x &&
								g_aMissile[nCntMissile].pos.x <= pBoss->pos.x + 35.0f + (MISSILE_WIDTH / 2.0f) &&
								pBoss->pos.y - 35.0f - (MISSILE_HEIGHT / 2.0f) <= g_aMissile[nCntMissile].pos.y &&
								g_aMissile[nCntMissile].pos.y <= pBoss->pos.y + 35.0f + (MISSILE_HEIGHT / 2.0f))
							{
								PlaySound(SOUND_LABEL_MISSILE000);

								//通常ミサイル
								if (g_aMissile[nCntMissile].nType == 0)
								{
									HitBoss(nCntBoss, 10, true);
								}
								//フレアミサイル
								else if (g_aMissile[nCntMissile].nType == 1)
								{
									SetExplosion(g_aMissile[nCntMissile].pos);
									PlaySound(SOUND_LABEL_EXPLOSION000);
								}

								//ミサイル消す
								g_aMissile[nCntMissile].bUse = false;
								pBoss->bMissile = false;
							}
						}
					}

					//直前のボスのカウントよりもカウントが大きいかつ敵がやられてないかつ特定の種類じゃない、
					//または敵のカウントが最後かつ敵がやられてない、またはボスカウントがマックスかつ使われてない
					if (nNowBoss < nCntBoss && pBoss->state != BOSSSTATE_DEATH && pBoss->type != BOSS_SEAGULL || 
						nCntBoss == MAX_BOSS - 1 && pBoss->state != BOSSSTATE_DEATH || nCntBoss == MAX_BOSS && pBoss->bUse == false)
					{
						//直前の敵のカウントが-1かつ敵が使われてるかつボスが特定の種類じゃない
						if (nNowBoss == -1 && pBoss->state != BOSSSTATE_DEATH && pBoss->type != BOSS_SEAGULL)
						{
							pFirstBoss = pBoss;
							nFirstBoss = nCntBoss;
						}

						//直前の敵のカウントと今のカウントが違うかつボスが特定の種類じゃない
						if (nNowBoss != nCntBoss && pBoss->type != BOSS_SEAGULL)
						{
							nNowBoss = nCntBoss;
						}

						//ボスのカウントが最後かつボスがやられてない
						if (nCntBoss == MAX_BOSS - 1 && pBoss->state != BOSSSTATE_DEATH)
						{
							//最初の敵を対象にする
							pBoss = pFirstBoss;
							nNowBoss = nFirstBoss;
						}

						//横にスライドする時間中かつボスがやられてない
						if (g_aMissile[nCntMissile].nTime <= MISSILE_SLIDE_TIME && pBoss->state != BOSSSTATE_DEATH)
						{
							if (g_aMissile[nCntMissile].pos.x < pBoss->pos.x) //敵がミサイルより右側に居るとき
							{
								g_aMissile[nCntMissile].pos.x += fabsf(g_aMissile[nCntMissile].fPosx - pBoss->pos.x) / MISSILE_SLIDE_TIME;
							}
							else //敵がミサイルより左側に居るとき
							{
								g_aMissile[nCntMissile].pos.x -= fabsf(g_aMissile[nCntMissile].fPosx - pBoss->pos.x) / MISSILE_SLIDE_TIME;
							}
							g_aMissile[nCntMissile].pos.y += 1.5f;
							pBoss->bMissile = true;
						}
						break;
					}
				}
			}

			for (nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++, pBarrier++)
			{
				//バリアが使われてるかつ敵のバリア
				if (pBarrier->bUse == true && pBarrier->ntype == 1)
				{
					//当たり判定
					if (pBarrier->pos.x - BARRIER_WIDTH - (MISSILE_WIDTH / 2.0f) <= g_aMissile[nCntMissile].pos.x &&
						g_aMissile[nCntMissile].pos.x <= pBarrier->pos.x + BARRIER_WIDTH + (MISSILE_WIDTH / 2.0f) &&
						pBarrier->pos.y - BARRIER_HEIGHT - (MISSILE_HEIGHT / 2.0f) <= g_aMissile[nCntMissile].pos.y &&
						g_aMissile[nCntMissile].pos.y <= pBarrier->pos.y + BARRIER_HEIGHT + (MISSILE_HEIGHT / 2.0f))
					{
						//ダメージ
						HitBarrier(8, nCntBarrier);
						g_aMissile[nCntMissile].bUse = false;
					}
				}
			}
		}
		pVtx += 4;
	}
	g_pVtxBuffMissile->Unlock();
}

//ミサイルの描画処理
void DrawMissile(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	int nCntMissile;
	pDevice = GetDevice();                        //デバイスを取得する
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMissile, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定

	for (nCntMissile = 0; nCntMissile < MAX_MISSILE; nCntMissile++)
	{
		//使用中
		if (g_aMissile[nCntMissile].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureMissile[g_aMissile[nCntMissile].nType]);    //テクスチャの設定
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntMissile * 4,
				2);
		}
	}
}

//ミサイルの設定処理
void SetMissile(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType)
{
	int nCntMissile;
	int nCntMissile2;
	VERTEX_2D *pVtx;
	int nNumber;
	g_pVtxBuffMissile->Lock(0, 0, (void **)&pVtx, 0);

	//ミサイルの種類に応じてループ回数変える
	if (nType == 0)
	{
		nNumber = 6;
	}
	else
	{
		nNumber = 1;
	}

	for (nCntMissile = 0; nCntMissile < nNumber; nCntMissile++)
	{
		for (nCntMissile2 = 0; nCntMissile2 < MAX_MISSILE; nCntMissile2++)
		{
			//使ってない
			if (g_aMissile[nCntMissile2].bUse == false)
			{
				//色々設定
				g_aMissile[nCntMissile2].pos = pos;
				g_aMissile[nCntMissile2].fPosx = pos.x;
				pVtx[0].pos = D3DXVECTOR3(g_aMissile[nCntMissile2].pos.x - (MISSILE_WIDTH / 2.0f), g_aMissile[nCntMissile2].pos.y - (MISSILE_HEIGHT / 2.0f), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aMissile[nCntMissile2].pos.x + (MISSILE_WIDTH / 2.0f), g_aMissile[nCntMissile2].pos.y - (MISSILE_HEIGHT / 2.0f), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aMissile[nCntMissile2].pos.x - (MISSILE_WIDTH / 2.0f), g_aMissile[nCntMissile2].pos.y + (MISSILE_HEIGHT / 2.0f), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aMissile[nCntMissile2].pos.x + (MISSILE_WIDTH / 2.0f), g_aMissile[nCntMissile2].pos.y + (MISSILE_HEIGHT / 2.0f), 0.0f);
				g_aMissile[nCntMissile2].move = move;
				g_aMissile[nCntMissile2].nType = nType;
				g_aMissile[nCntMissile2].nTime = 0;
				g_aMissile[nCntMissile2].bUse = true;
				break;
			}
			pVtx += 4;
		}
	}
	g_pVtxBuffMissile->Unlock();
}

//ミサイル取得
Missile *GetMissile(void)
{
	return &g_aMissile[0];
}

//リセット
void ResetMissile(void)
{
	int nCntMissile;
	for (nCntMissile = 0; nCntMissile < MAX_MISSILE; nCntMissile++)
	{
		g_aMissile[nCntMissile].bUse = false;
	}
}