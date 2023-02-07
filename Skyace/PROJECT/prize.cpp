//--------------------------
//Author:三上航世
//プライズの処理(prize.cpp)
//--------------------------
#include "prize.h"
#include "aircraft.h"
#include "sound.h"

//マクロ定義
#define MAX_PRIZE (16) //プライズの最大数
#define PRIZE_SIZE (20.0f) //プライズのサイズ

//プライズの構造体定義
typedef struct
{
	D3DXVECTOR3 pos;  //位置
	int nTime;        //点滅用時間
	D3DXCOLOR col;    //色
	bool bUse;        //使用しているかどうか
} Prize;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePrize = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPrize = NULL;	//頂点バッファへのポインタ
Prize g_aPrize[MAX_PRIZE];						//プライズ

//初期化処理
void InitPrize(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntPrize;
	pDevice = GetDevice();

	//弾の初期化
	for (nCntPrize = 0; nCntPrize < MAX_PRIZE; nCntPrize++)
	{
		g_aPrize[nCntPrize].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPrize[nCntPrize].nTime = 0;
		g_aPrize[nCntPrize].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aPrize[nCntPrize].bUse = false;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/Smark.png",
		&g_pTexturePrize);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PRIZE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPrize,
		NULL);

	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPrize->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntPrize = 0; nCntPrize < MAX_PRIZE; nCntPrize++)
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
	g_pVtxBuffPrize->Unlock();
}

//弾の終了処理
void UninitPrize(void)
{
	//テクスチャの破棄
	if (g_pTexturePrize != NULL)
	{
		g_pTexturePrize->Release();
		g_pTexturePrize = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffPrize != NULL)
	{
		g_pVtxBuffPrize->Release();
		g_pVtxBuffPrize = NULL;
	}
}

//プライズの更新処理
void UpdatePrize(void)
{
	int nCntPrize;
	VERTEX_2D *pVtx;
	g_pVtxBuffPrize->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntPrize = 0; nCntPrize < MAX_PRIZE; nCntPrize++)
	{
		//使ってる
		if (g_aPrize[nCntPrize].bUse == true)
		{
			Aircraft *pAircraft;
			pAircraft = GetAircraft();
			g_aPrize[nCntPrize].nTime++;

			//10フレーム毎に点滅させる
			if (g_aPrize[nCntPrize].nTime % 10 == 0)
			{
				g_aPrize[nCntPrize].nTime = 0;
				if (g_aPrize[nCntPrize].col.b == 1.0f)
				{
					g_aPrize[nCntPrize].col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
				}
				else
				{
					g_aPrize[nCntPrize].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
			}

			//当たり判定
			if (pAircraft->pos.x - (AIRCRAFT_WIDTH / 2.0f) - PRIZE_SIZE <= g_aPrize[nCntPrize].pos.x &&
				g_aPrize[nCntPrize].pos.x <= pAircraft->pos.x + (AIRCRAFT_WIDTH / 2.0f) + PRIZE_SIZE &&
				pAircraft->pos.y - (AIRCRAFT_HEIGHT / 2.0f) - PRIZE_SIZE <= g_aPrize[nCntPrize].pos.y &&
				g_aPrize[nCntPrize].pos.y <= pAircraft->pos.y + (AIRCRAFT_HEIGHT / 2.0f) + PRIZE_SIZE)
			{
				//自機の特殊兵装の数増やす
				if (pAircraft->nSpecial < 6)
				{
					pAircraft->nSpecial++;
				}

				//プライズ消す
				g_aPrize[nCntPrize].bUse = false;
				PlaySound(SOUND_LABEL_ITEM000);
				break;
			}

			//下に落とす
			g_aPrize[nCntPrize].pos.y += 1.0f;
			pVtx[0].pos = D3DXVECTOR3(g_aPrize[nCntPrize].pos.x - PRIZE_SIZE, g_aPrize[nCntPrize].pos.y - PRIZE_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aPrize[nCntPrize].pos.x + PRIZE_SIZE, g_aPrize[nCntPrize].pos.y - PRIZE_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aPrize[nCntPrize].pos.x - PRIZE_SIZE, g_aPrize[nCntPrize].pos.y + PRIZE_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aPrize[nCntPrize].pos.x + PRIZE_SIZE, g_aPrize[nCntPrize].pos.y + PRIZE_SIZE, 0.0f);
			pVtx[0].col = g_aPrize[nCntPrize].col;
			pVtx[1].col = g_aPrize[nCntPrize].col;
			pVtx[2].col = g_aPrize[nCntPrize].col;
			pVtx[3].col = g_aPrize[nCntPrize].col;

			//画面外に行くと、消える
			if (g_aPrize[nCntPrize].pos.y > 720 - PRIZE_SIZE)
			{
				g_aPrize[nCntPrize].bUse = false;
			}
			
		}
		pVtx += 4;
	}
	g_pVtxBuffPrize->Unlock();
}

//描画処理
void DrawPrize(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	int nCntPrize;
	pDevice = GetDevice();                        //デバイスを取得する
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPrize, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	for (nCntPrize = 0; nCntPrize < MAX_PRIZE; nCntPrize++)
	{
		//使用中
		if (g_aPrize[nCntPrize].bUse == true)
		{
			pDevice->SetTexture(0, g_pTexturePrize);    //テクスチャの設定
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntPrize * 4,
				2);
		}
	}
}

//プライズの設定処理
void SetPrize(D3DXVECTOR3 pos)
{
	int nCntPrize;
	VERTEX_2D *pVtx;
	g_pVtxBuffPrize->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntPrize = 0; nCntPrize < MAX_PRIZE; nCntPrize++)
	{
		//使ってない
		if (g_aPrize[nCntPrize].bUse == false)
		{
			//色々設定
			g_aPrize[nCntPrize].pos = pos;
			g_aPrize[nCntPrize].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[0].pos = D3DXVECTOR3(g_aPrize[nCntPrize].pos.x - PRIZE_SIZE, g_aPrize[nCntPrize].pos.y - PRIZE_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aPrize[nCntPrize].pos.x + PRIZE_SIZE, g_aPrize[nCntPrize].pos.y - PRIZE_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aPrize[nCntPrize].pos.x - PRIZE_SIZE, g_aPrize[nCntPrize].pos.y + PRIZE_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aPrize[nCntPrize].pos.x + PRIZE_SIZE, g_aPrize[nCntPrize].pos.y + PRIZE_SIZE, 0.0f);
			g_aPrize[nCntPrize].bUse = true;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffPrize->Unlock();
}

//リセット
void ResetPrize(void)
{
	int nCntPrize;
	for (nCntPrize = 0; nCntPrize < MAX_PRIZE; nCntPrize++)
	{
		g_aPrize[nCntPrize].bUse = false;
	}
}