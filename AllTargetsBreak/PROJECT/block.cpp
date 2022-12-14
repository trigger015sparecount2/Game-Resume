//--------------------------
//Author:三上航世
//自機の処理(aircraft.cpp)
//--------------------------
#include "player.h"
#include "block.h"
#include "input.h"
#include "sound.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBlock[BLOCK_MAX] = {}; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL; //頂点バッファへのポインタ
Block g_aBlock[MAX_BLOCK];

//初期化処理
HRESULT InitBlock(void)
{
	int nCntBlock;
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].Startpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].Maxmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].BlockType = BLOCK_NORMAL;
		g_aBlock[nCntBlock].nTime = 0;
		g_aBlock[nCntBlock].nMaxTime = 0;
		g_aBlock[nCntBlock].fWidth = 50.0f;
		g_aBlock[nCntBlock].fHeight = 20.0f;
		g_aBlock[nCntBlock].bUse = false;
	}
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/block000.jpg",
		&g_pTextureBlock[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/upfloor.png",
		&g_pTextureBlock[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/downfloor.png",
		&g_pTextureBlock[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/iceblock.png",
		&g_pTextureBlock[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/carryblock.png",
		&g_pTextureBlock[4]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/warphole.png",
		&g_pTextureBlock[5]);
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);
	VERTEX_2D *pVtx; //頂点情報へのポインタ
					 //頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
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
	g_pVtxBuffBlock->Unlock();
	return S_OK;
}

//終了処理
void UninitBlock(void)
{
	int nCntBlock;
	for (nCntBlock = 0; nCntBlock < BLOCK_MAX; nCntBlock++)
	{
		//テクスチャの破棄
		if (g_pTextureBlock[nCntBlock] != NULL)
		{
			g_pTextureBlock[nCntBlock]->Release();
			g_pTextureBlock[nCntBlock] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//更新処理
void UpdateBlock(void)
{
	int nCntBlock;
	bool bJump = false;
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			g_aBlock[nCntBlock].nTime++;
			if (g_aBlock[nCntBlock].nTime <= g_aBlock[nCntBlock].nMaxTime / 2)
			{
				g_aBlock[nCntBlock].move.x += g_aBlock[nCntBlock].Maxmove.x / 30;
				if (g_aBlock[nCntBlock].move.x > g_aBlock[nCntBlock].Maxmove.x)
				{
					g_aBlock[nCntBlock].move.x = g_aBlock[nCntBlock].Maxmove.x;
				}
				g_aBlock[nCntBlock].move.y += g_aBlock[nCntBlock].Maxmove.y / 30;
				if (g_aBlock[nCntBlock].move.y > g_aBlock[nCntBlock].Maxmove.y)
				{
					g_aBlock[nCntBlock].move.y = g_aBlock[nCntBlock].Maxmove.y;
				}
			}
			else if (g_aBlock[nCntBlock].nTime < g_aBlock[nCntBlock].nMaxTime)
			{
				g_aBlock[nCntBlock].move.x -= g_aBlock[nCntBlock].Maxmove.x / 30;
				if (g_aBlock[nCntBlock].move.x < -g_aBlock[nCntBlock].Maxmove.x)
				{
					g_aBlock[nCntBlock].move.x = -g_aBlock[nCntBlock].Maxmove.x;
				}
				g_aBlock[nCntBlock].move.y -= g_aBlock[nCntBlock].Maxmove.y / 30;
				if (g_aBlock[nCntBlock].move.y < -g_aBlock[nCntBlock].Maxmove.y)
				{
					g_aBlock[nCntBlock].move.y = -g_aBlock[nCntBlock].Maxmove.y;
				}
			}
			else if (g_aBlock[nCntBlock].nTime >= g_aBlock[nCntBlock].nMaxTime)
			{
				g_aBlock[nCntBlock].nTime = 0;
			}
			if (g_aBlock[nCntBlock].BlockType == BLOCK_DOWN && g_aBlock[nCntBlock].pos.y > g_aBlock[nCntBlock].Startpos.y)
			{
				g_aBlock[nCntBlock].move.y -= 1.0f;
				if (g_aBlock[nCntBlock].move.y < -3.0f)
				{
					g_aBlock[nCntBlock].move.y = -3.0f;
				}
			}
			else if (g_aBlock[nCntBlock].BlockType == BLOCK_UP && g_aBlock[nCntBlock].pos.y < g_aBlock[nCntBlock].Startpos.y)
			{
				g_aBlock[nCntBlock].move.y += 1.0f;
				if (g_aBlock[nCntBlock].move.y > 3.0f)
				{
					g_aBlock[nCntBlock].move.y = 3.0f;
				}
			}
			if (g_aBlock[nCntBlock].BlockType == BLOCK_DOWN && g_aBlock[nCntBlock].pos.y < g_aBlock[nCntBlock].Startpos.y ||
				g_aBlock[nCntBlock].BlockType == BLOCK_UP && g_aBlock[nCntBlock].pos.y > g_aBlock[nCntBlock].Startpos.y)
			{
				g_aBlock[nCntBlock].move.y = 0.0f;
				g_aBlock[nCntBlock].pos.y = g_aBlock[nCntBlock].Startpos.y;
			}
			g_aBlock[nCntBlock].pos += g_aBlock[nCntBlock].move;
			if (g_aBlock[nCntBlock].BlockType == BLOCK_CARRY)
			{
				g_aBlock[nCntBlock].move.x = 0.0f;
			}
			VERTEX_2D *pVtx;
			g_pVtxBuffBlock->Lock(0, 0, (void **)&pVtx, 0);
			pVtx += nCntBlock * 4;
			pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);
			g_pVtxBuffBlock->Unlock();
		}
	}
}

//描画処理
void DrawBlock(void)
{
	int nCntBlock;
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	pDevice = GetDevice();                        //デバイスを取得する
												  //頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			switch (g_aBlock[nCntBlock].BlockType)
			{
			case BLOCK_NORMAL:
				pDevice->SetTexture(0, g_pTextureBlock[0]); //テクスチャの設定
				break;
			case BLOCK_UP:
				pDevice->SetTexture(0, g_pTextureBlock[1]); //テクスチャの設定
				break;
			case BLOCK_DOWN:
				pDevice->SetTexture(0, g_pTextureBlock[2]); //テクスチャの設定
				break;
			case BLOCK_ICE:
				pDevice->SetTexture(0, g_pTextureBlock[3]); //テクスチャの設定
				break;
			case BLOCK_CARRY:
				pDevice->SetTexture(0, g_pTextureBlock[4]); //テクスチャの設定
				break;
			case BLOCK_WARP:
				pDevice->SetTexture(0, g_pTextureBlock[5]); //テクスチャの設定
				break;
			}
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntBlock * 4,  //開始する始点のインデックス
				2); //描画するプリミティブ数
		}
	}
}

void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXVECTOR3 Maxmove, int nMaxTime, BLOCKTYPE BlockType)
{
	int nCntBlock;
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == false)
		{
			g_aBlock[nCntBlock].pos = pos;
			g_aBlock[nCntBlock].Startpos = pos;
			g_aBlock[nCntBlock].fWidth = fWidth;
			g_aBlock[nCntBlock].fHeight = fHeight;
			g_aBlock[nCntBlock].bUse = true;
			g_aBlock[nCntBlock].Maxmove = Maxmove;
			g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aBlock[nCntBlock].nMaxTime = nMaxTime;
			g_aBlock[nCntBlock].nTime = 0;
			g_aBlock[nCntBlock].BlockType = BlockType;
			VERTEX_2D *pVtx;
			g_pVtxBuffBlock->Lock(0, 0, (void **)&pVtx, 0);
			pVtx += nCntBlock * 4;
			pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);
			if (BlockType == BLOCK_NORMAL || BlockType == BLOCK_ICE)
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(fWidth / 20, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, fHeight / 20);
				pVtx[3].tex = D3DXVECTOR2(fWidth / 20, fHeight / 20);
			}
			else
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			g_pVtxBuffBlock->Unlock();
			break;
		}
	}
}

void ResetBlock(void)
{
	int nCntBlock;
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].bUse = false;
	}
}

bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight, D3DXVECTOR3 *pBlockMove, int nType)
{
	Block *pBlock;
	D3DXVECTOR3 WarpPos;
	bool bLand = false;
	pBlock = &g_aBlock[0];
	int nCntBlock;
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			if (nType == 0)
			{
				if (pPos->x - fWidth < pBlock->pos.x + pBlock->fWidth && pBlock->pos.x < pPos->x + fWidth)
				{
					//ブロックの上側にぶつかった(乗った)とき
					if (pPosOld->y - 5.0f <= pBlock->pos.y && pBlock->pos.y <= pPos->y)
					{
						bLand = true;
						pMove->y = 0.0f;
						if (pBlock->BlockType == BLOCK_ICE)
						{
							IcePlayer(true);
						}
						else
						{
							IcePlayer(false);
						}
						if (pBlock->BlockType == BLOCK_DOWN)
						{
							pBlock->move.y = 4.0f;
							if (pBlock->BlockType == BLOCK_DOWN && pBlock->pos.y >= pBlock->Maxmove.y)
							{
								pBlock->move.y = 1.0f;
								pBlock->pos.y = pBlock->Maxmove.y;
							}
						}
						else if (pBlock->BlockType == BLOCK_UP)
						{
							pBlock->move.y = -4.0f;
							if (pBlock->BlockType == BLOCK_UP && pBlock->pos.y <= pBlock->Maxmove.y)
							{
								pBlock->move.y = -1.0f;
								pBlock->pos.y = pBlock->Maxmove.y;
							}
						}
						pBlockMove->x = pBlock->move.x;
						pBlockMove->y = pBlock->move.y;
						pPos->y = pBlock->pos.y;
					}
					//ブロックの下側にぶつかったとき
					else if (pBlock->pos.y + pBlock->fHeight >= pPos->y - fHeight &&
						pBlock->pos.y + pBlock->fHeight <= pPosOld->y - fHeight)
					{
						//ブロックの左下にぶつかったとき
						if (pBlock->pos.x <= pPos->x + fWidth && pPos->x + (fWidth / 2) <= pBlock->pos.x)
						{
							pMove->x = 0.0f;
							pPos->x = pBlock->pos.x - fWidth;
						}
						//ブロックの右下にぶつかったとき
						else if (pPos->x - fWidth <= pBlock->pos.x + pBlock->fWidth && pBlock->pos.x + pBlock->fWidth <= pPos->x - (fWidth / 2))
						{
							pMove->x = 0.0f;
							pPos->x = pBlock->pos.x + pBlock->fWidth + fWidth;
						}
						else
						{
							pMove->y = 1.0f;
							pPos->y = pBlock->pos.y + pBlock->fHeight + fHeight + 1.0f;
							pBlockMove->x = 0.0f;
							pBlockMove->y = 0.0f;
							PlaySound(SOUND_LABEL_BLOCKCLASH);
						}
					}
				}
				if (pPos->y - fHeight < pBlock->pos.y + pBlock->fHeight && pBlock->pos.y < pPos->y)
				{
					//ブロックの左にぶつかったとき
					if (pBlock->pos.x <= pPos->x + fWidth && pPosOld->x + fWidth <= pBlock->pos.x)
					{
						if (pBlock->BlockType == BLOCK_WARP) //ワープホールに入ったとき
						{
							WarpPos = WarpBlock(pBlock->nMaxTime);
							pPos->x = WarpPos.x + pBlock->fWidth + fWidth + 5.0f;
							pPos->y = WarpPos.y + pBlock->fHeight - 1.0f;
							pMove->y = 0.0f;
							pPosOld = pPos;
						}
						else //それ以外のブロックに当たったとき
						{
							pMove->x = 0.0f;
							pPos->x = pBlock->pos.x - fWidth;
							if (pBlock->BlockType == BLOCK_CARRY) //キャリーブロックだった時
							{
								pBlock->move.x = 0.8f;
							}
						}
					}
					//ブロックの右にぶつかったとき
					if (pPos->x - fWidth <= pBlock->pos.x + pBlock->fWidth && pBlock->pos.x + pBlock->fWidth <= pPosOld->x - fWidth)
					{
						if (pBlock->BlockType == BLOCK_WARP) //ワープホールに入ったとき
						{
							WarpPos = WarpBlock(pBlock->nMaxTime);
							pPos->x = WarpPos.x - fWidth - 5.0f;
							pPos->y = WarpPos.y + pBlock->fHeight - 1.0f;
							pPosOld = pPos;
							pMove->y = 0.0f;
						}
						else //それ以外のブロックに当たったとき
						{
							pMove->x = 0.0f;
							pPos->x = pBlock->pos.x + pBlock->fWidth + fWidth;
							if (pBlock->BlockType == BLOCK_CARRY) //キャリーブロックだった時
							{
								pBlock->move.x = -0.8f;
							}
						}
					}
				}
			}
			else if (nType == 1)
			{
				if (pPos->x - (fWidth / 2) <= pBlock->pos.x + pBlock->fWidth && pBlock->pos.x <= pPos->x + (fWidth / 2) &&
					pPos->y - (fHeight / 2) <= pBlock->pos.y + pBlock->fHeight && pBlock->pos.y <= pPos->y + (fHeight / 2))
				{
					if (pBlock->BlockType == BLOCK_WARP)
					{
						if (pBlock->pos.x <= pPos->x + fWidth / 2 && pPosOld->x + fWidth / 2 <= pBlock->pos.x) //ワープホールの左側に刺さったとき
						{
							WarpPos = WarpBlock(pBlock->nMaxTime);
							pPos->x = WarpPos.x + pBlock->fWidth + fWidth + 5.0f;
							pPos->y = WarpPos.y - (pBlock->pos.y - pPos->y);
							pPosOld = pPos;
						}
						else if (pPos->x - fWidth / 2 <= pBlock->pos.x + pBlock->fWidth && pBlock->pos.x + pBlock->fWidth <= pPosOld->x - fWidth / 2) //ワープゾーンの右側に刺さったとき
						{
							WarpPos = WarpBlock(pBlock->nMaxTime);
							pPos->x = WarpPos.x - fWidth - 5.0f;
							pPos->y = WarpPos.y - (pBlock->pos.y - pPos->y);
							pPosOld = pPos;
						}
					}
					else
					{
						pMove->x = 0.0f;
						pMove->y = 0.0f;
						pPos->x += pBlock->move.x;
						if (pBlock->BlockType != BLOCK_DOWN)
						{
							pPos->y += pBlock->move.y;
						}
						else if (pBlock->Startpos.y < pBlock->pos.y && pBlock->pos.y < pBlock->Maxmove.y)
						{
							pPos->y += pBlock->move.y - 1.0f;
						}
						bLand = true;
					}
				}
			}
		}
	}
	return bLand;
}

D3DXVECTOR3 WarpBlock(int nMaxTime)
{
	int nCntBlock;
	D3DXVECTOR3 pos;
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			if (g_aBlock[nCntBlock].BlockType == BLOCK_WARP)
			{
				if (nMaxTime % 2 == 0 && nMaxTime + 1 == g_aBlock[nCntBlock].nMaxTime ||
					nMaxTime % 2 == 1 && nMaxTime - 1 == g_aBlock[nCntBlock].nMaxTime)
				{
					pos = g_aBlock[nCntBlock].pos;
				}
			}
		}
	}
	PlaySound(SOUND_LABEL_WARP);
	return pos;
}