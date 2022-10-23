//--------------------------
//Author:三上航世
//ロックオンシーカーの処理(lockon.cpp)
//--------------------------
#include "lockon.h"
#include "enemy.h"
#include "boss.h"
//マクロ定義
#define MAX_LOCKON (128)   //ロックオンシーカーの最大数
#define LOCKON_BLOCK (30)  //ロックオンシーカーの大きさ
//ロックオンシーカーの構造体定義
typedef struct
{
	D3DXVECTOR3 pos;  //位置
	bool bUse;        //使用しているかどうか
} Lockon;
//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureLockon = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLockon = NULL; //頂点バッファへのポインタ
Lockon g_aLockon[MAX_LOCKON];

//初期化処理
void InitLockon(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntLockon;
	pDevice = GetDevice();
	//ロックオンの初期化
	for (nCntLockon = 0; nCntLockon < MAX_LOCKON; nCntLockon++)
	{
		g_aLockon[nCntLockon].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aLockon[nCntLockon].bUse = false;
	}
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/lockon.png",
		&g_pTextureLockon);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LOCKON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLockon,
		NULL);
	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffLockon->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntLockon = 0; nCntLockon < MAX_LOCKON; nCntLockon++)
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
		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffLockon->Unlock();
}

//ロックオンシーカーの終了処理
void UninitLockon(void)
{
	//テクスチャの破棄
	if (g_pTextureLockon != NULL)
	{
		g_pTextureLockon->Release();
		g_pTextureLockon = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffLockon != NULL)
	{
		g_pVtxBuffLockon->Release();
		g_pVtxBuffLockon = NULL;
	}
}

//ロックオンシーカーの更新処理
void UpdateLockon(void)
{
	int nCntLockon;
	VERTEX_2D *pVtx;
	int nNowEnemy = -1;
	float fRed;
	int nCntEnemy;
	Enemy *pEnemy;
	int nNowBoss = -1;
	int nCntBoss;
	Boss *pBoss;
	for (nCntLockon = 0; nCntLockon < MAX_LOCKON; nCntLockon++)
	{
		g_aLockon[nCntLockon].bUse = false;
		pEnemy = GetEnemy();
		for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{
			if (g_aLockon[nCntLockon].bUse == true)
			{
				break;
			}
			if (pEnemy->bUse == true && nNowEnemy < nCntEnemy)
			{
				nNowEnemy = nCntEnemy;
				g_aLockon[nCntLockon].bUse = true;
			}
		}
		pBoss = GetBoss();
		for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++, pBoss++)
		{
			if (g_aLockon[nCntLockon].bUse == true)
			{
				break;
			}
			if (pBoss->bUse == true && nNowBoss < nCntBoss && pBoss->type != BOSS_SEAGULL && pBoss->state != BOSSSTATE_DEATH)
			{
				nNowBoss = nCntBoss;
				g_aLockon[nCntLockon].bUse = true;
			}
		}
	}
	nNowEnemy = -1;
	nNowBoss = -1;
	g_pVtxBuffLockon->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntLockon = 0; nCntLockon < MAX_LOCKON; nCntLockon++)
	{
		fRed = 0.0f;
		pEnemy = GetEnemy();
		for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{
			if (pEnemy->bUse == true)
			{
				if (g_aLockon[nCntLockon].bUse == true && nNowEnemy < nCntEnemy)
				{
					if (pEnemy->bMissile == true)
					{
						fRed = 0.5f;
					}
					pVtx[0].tex = D3DXVECTOR2(fRed, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(fRed + 0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(fRed, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(fRed + 0.5f, 1.0f);
					g_aLockon[nCntLockon].pos = pEnemy->pos;
					pVtx[0].pos = D3DXVECTOR3(g_aLockon[nCntLockon].pos.x - (LOCKON_BLOCK / 2), g_aLockon[nCntLockon].pos.y - (LOCKON_BLOCK / 2), 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aLockon[nCntLockon].pos.x + (LOCKON_BLOCK / 2), g_aLockon[nCntLockon].pos.y - (LOCKON_BLOCK / 2), 0.0f);
					pVtx[2].pos = D3DXVECTOR3(g_aLockon[nCntLockon].pos.x - (LOCKON_BLOCK / 2), g_aLockon[nCntLockon].pos.y + (LOCKON_BLOCK / 2), 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aLockon[nCntLockon].pos.x + (LOCKON_BLOCK / 2), g_aLockon[nCntLockon].pos.y + (LOCKON_BLOCK / 2), 0.0f);
					nNowEnemy = nCntEnemy;
					break;
				}
			}
		}
		pBoss = GetBoss();
		for (nCntBoss = 0; nCntBoss < MAX_ENEMY; nCntBoss++, pBoss++)
		{
			if (pBoss->bUse == true)
			{
				if (g_aLockon[nCntLockon].bUse == true && nNowBoss < nCntBoss && pBoss->type != BOSS_SEAGULL && pBoss->state != BOSSSTATE_DEATH)
				{
					if (pBoss->bMissile == true)
					{
						fRed = 0.5f;
					}
					pVtx[0].tex = D3DXVECTOR2(fRed, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(fRed + 0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(fRed, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(fRed + 0.5f, 1.0f);
					g_aLockon[nCntLockon].pos = pBoss->pos;
					pVtx[0].pos = D3DXVECTOR3(g_aLockon[nCntLockon].pos.x - (LOCKON_BLOCK / 2), g_aLockon[nCntLockon].pos.y - (LOCKON_BLOCK / 2), 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aLockon[nCntLockon].pos.x + (LOCKON_BLOCK / 2), g_aLockon[nCntLockon].pos.y - (LOCKON_BLOCK / 2), 0.0f);
					pVtx[2].pos = D3DXVECTOR3(g_aLockon[nCntLockon].pos.x - (LOCKON_BLOCK / 2), g_aLockon[nCntLockon].pos.y + (LOCKON_BLOCK / 2), 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aLockon[nCntLockon].pos.x + (LOCKON_BLOCK / 2), g_aLockon[nCntLockon].pos.y + (LOCKON_BLOCK / 2), 0.0f);
					nNowBoss = nCntBoss;
					break;
				}
			}
		}
		pVtx += 4;
	}
	g_pVtxBuffLockon->Unlock();
}

//ロックオンシーカーの描画処理
void DrawLockon(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	int nCntLockon;
	pDevice = GetDevice();                        //デバイスを取得する
												  //頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLockon, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureLockon);    //テクスチャの設定
	//ポリゴン描画
	for (nCntLockon = 0; nCntLockon < MAX_LOCKON; nCntLockon++)
	{
		if (g_aLockon[nCntLockon].bUse == true)
		{
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntLockon * 4,
				2);
		}
	}
}

void ResetLockon(void)
{
	int nCntLockon;
	for (nCntLockon = 0; nCntLockon < MAX_LOCKON; nCntLockon++)
	{
		g_aLockon[nCntLockon].bUse = false;
	}
}