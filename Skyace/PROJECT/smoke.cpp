//--------------------------
//Author:三上航世
//煙の処理(smoke.cpp)
//--------------------------
#include "smoke.h"
//マクロ定義
#define MAX_SMOKE (256)   //煙の最大数
#define SMOKE_WIDTH (36)  //煙の幅
#define SMOKE_HEIGHT (32) //煙の高さ
//煙の構造体定義
typedef struct
{
	D3DXVECTOR3 pos;  //位置
	int nTime;        //寿命
	int nRed;         //赤み具合
	bool bUse;        //使用しているかどうか
} Smoke;
//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureSmoke = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSmoke = NULL; //頂点バッファへのポインタ
Smoke g_aSmoke[MAX_SMOKE];

//初期化処理
void InitSmoke(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntSmoke;
	pDevice = GetDevice();
	//煙の初期化
	for (nCntSmoke = 0; nCntSmoke < MAX_SMOKE; nCntSmoke++)
	{
		g_aSmoke[nCntSmoke].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSmoke[nCntSmoke].nRed = 0;
		g_aSmoke[nCntSmoke].nTime = 0;
		g_aSmoke[nCntSmoke].bUse = false;
	}
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/smoke.png",
		&g_pTextureSmoke);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SMOKE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSmoke,
		NULL);
	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffSmoke->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntSmoke = 0; nCntSmoke < MAX_SMOKE; nCntSmoke++)
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
	g_pVtxBuffSmoke->Unlock();
}

//煙の終了処理
void UninitSmoke(void)
{
	//テクスチャの破棄
	if (g_pTextureSmoke != NULL)
	{
		g_pTextureSmoke->Release();
		g_pTextureSmoke = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffSmoke != NULL)
	{
		g_pVtxBuffSmoke->Release();
		g_pVtxBuffSmoke = NULL;
	}
}

//煙の更新処理
void UpdateSmoke(void)
{
	int nCntSmoke;
	VERTEX_2D *pVtx;
	g_pVtxBuffSmoke->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntSmoke = 0; nCntSmoke < MAX_SMOKE; nCntSmoke++)
	{
		if (g_aSmoke[nCntSmoke].bUse == true)
		{
			g_aSmoke[nCntSmoke].nTime++;
			g_aSmoke[nCntSmoke].nRed -= 34;
			if (g_aSmoke[nCntSmoke].nRed <= 0)
			{
				g_aSmoke[nCntSmoke].nRed = 0;
			}
			if (g_aSmoke[nCntSmoke].nTime == 180)
			{
				g_aSmoke[nCntSmoke].bUse = false;
			}
			g_aSmoke[nCntSmoke].pos.y += 0.75f;
			if (g_aSmoke[nCntSmoke].pos.y >= 720.0f + (SMOKE_HEIGHT / 2))
			{
				g_aSmoke[nCntSmoke].bUse = false;
			}
			pVtx[0].col = D3DCOLOR_RGBA(255, 255 - g_aSmoke[nCntSmoke].nRed, 255 - g_aSmoke[nCntSmoke].nRed, 180 - g_aSmoke[nCntSmoke].nTime);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255 - g_aSmoke[nCntSmoke].nRed, 255 - g_aSmoke[nCntSmoke].nRed, 180 - g_aSmoke[nCntSmoke].nTime);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255 - g_aSmoke[nCntSmoke].nRed, 255 - g_aSmoke[nCntSmoke].nRed, 180 - g_aSmoke[nCntSmoke].nTime);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255 - g_aSmoke[nCntSmoke].nRed, 255 - g_aSmoke[nCntSmoke].nRed, 180 - g_aSmoke[nCntSmoke].nTime);
			pVtx[0].pos = D3DXVECTOR3(g_aSmoke[nCntSmoke].pos.x - (SMOKE_WIDTH / 2), g_aSmoke[nCntSmoke].pos.y - (SMOKE_HEIGHT / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aSmoke[nCntSmoke].pos.x + (SMOKE_WIDTH / 2), g_aSmoke[nCntSmoke].pos.y - (SMOKE_HEIGHT / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aSmoke[nCntSmoke].pos.x - (SMOKE_WIDTH / 2), g_aSmoke[nCntSmoke].pos.y + (SMOKE_HEIGHT / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aSmoke[nCntSmoke].pos.x + (SMOKE_WIDTH / 2), g_aSmoke[nCntSmoke].pos.y + (SMOKE_HEIGHT / 2), 0.0f);
		}
		pVtx += 4;
	}
	g_pVtxBuffSmoke->Unlock();
}

//描画処理
void DrawSmoke(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	int nCntSmoke;
	pDevice = GetDevice();                        //デバイスを取得する
												  //頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSmoke, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureSmoke);    //テクスチャの設定
	for (nCntSmoke = 0; nCntSmoke < MAX_SMOKE; nCntSmoke++)
	{
		if (g_aSmoke[nCntSmoke].bUse == true)
		{
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntSmoke * 4,
				2);
		}
	}
}

//煙の設定処理
void SetSmoke(D3DXVECTOR3 pos)
{
	int nCntSmoke;
	VERTEX_2D *pVtx;
	g_pVtxBuffSmoke->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntSmoke = 0; nCntSmoke < MAX_SMOKE; nCntSmoke++)
	{
		if (g_aSmoke[nCntSmoke].bUse == false)
		{
			g_aSmoke[nCntSmoke].pos = pos;
			pVtx[0].pos = D3DXVECTOR3(g_aSmoke[nCntSmoke].pos.x - (SMOKE_WIDTH / 2), g_aSmoke[nCntSmoke].pos.y - (SMOKE_HEIGHT / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aSmoke[nCntSmoke].pos.x + (SMOKE_WIDTH / 2), g_aSmoke[nCntSmoke].pos.y - (SMOKE_HEIGHT / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aSmoke[nCntSmoke].pos.x - (SMOKE_WIDTH / 2), g_aSmoke[nCntSmoke].pos.y + (SMOKE_HEIGHT / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aSmoke[nCntSmoke].pos.x + (SMOKE_WIDTH / 2), g_aSmoke[nCntSmoke].pos.y + (SMOKE_HEIGHT / 2), 0.0f);
			g_aSmoke[nCntSmoke].nRed = 255;
			g_aSmoke[nCntSmoke].nTime = 0;
			g_aSmoke[nCntSmoke].bUse = true;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffSmoke->Unlock();
}

void ResetSmoke(void)
{
	int nCntSmoke;
	for (nCntSmoke = 0; nCntSmoke < MAX_SMOKE; nCntSmoke++)
	{
		g_aSmoke[nCntSmoke].bUse = false;
	}
}