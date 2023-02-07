//--------------------------
//Author:三上航世
//背景の処理(bg.cpp)
//--------------------------
#include "bg.h"

//背景の構造体定義
typedef struct
{
	int ntype;   //種類
	float fPosTexU;
	bool bscroll; //スクロールするか
} Bg;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBg[2] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;	//頂点バッファへのポインタ
Bg g_aBg;	//背景

//初期化処理
void InitBg(void)
{
	int nCntBg;
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	g_aBg.bscroll = true;
	g_aBg.fPosTexU = 0.0f;
	g_aBg.ntype = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/sky000.jpg",
		&g_pTextureBg[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/mountain000.png",
		&g_pTextureBg[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg,
		NULL);

	VERTEX_2D *pVtx; //頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntBg = 0; nCntBg < 2; nCntBg++)
	{
		//頂点座標の設定
		if (nCntBg == 0)
		{
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);
		}
		else
		{
			pVtx[0].pos = D3DXVECTOR3(0.0f, 550.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1280.0f, 550.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);
		}
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
		if (nCntBg == 0)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
		}
		else
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBg->Unlock();
}

//終了処理
void UninitBg(void)
{
	int nCntBg;
	for (nCntBg = 0; nCntBg < 2; nCntBg++)
	{
		//テクスチャの破棄
		if (g_pTextureBg[nCntBg] != NULL)
		{
			g_pTextureBg[nCntBg]->Release();
			g_pTextureBg[nCntBg] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}
}

//更新処理
void UpdateBg(void)
{
	//背景スクロールさせる
	if (g_aBg.bscroll == true)
	{
		VERTEX_2D *pVtx;
		g_pVtxBuffBg->Lock(0, 0, (void **)&pVtx, 0);
		g_aBg.fPosTexU += 0.0001f;
		if (g_aBg.fPosTexU >= 1.0f)
		{
			g_aBg.fPosTexU = 0.0f;
		}

		pVtx[0].tex = D3DXVECTOR2(g_aBg.fPosTexU, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aBg.fPosTexU + 0.25f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aBg.fPosTexU, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aBg.fPosTexU + 0.25f, 1.0f);
		g_pVtxBuffBg->Unlock();
	}
}

//描画処理
void DrawBg(void)
{
	int nCntBg;
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	pDevice = GetDevice();                        //デバイスを取得する
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	for (nCntBg = 0; nCntBg < 2; nCntBg++)
	{
		pDevice->SetTexture(0, g_pTextureBg[nCntBg]);    //テクスチャの設定
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntBg * 4,  //開始する始点のインデックス
			2); //描画するプリミティブ数
	}
}

//背景の設定処理
void SetBg(int nType, bool bscroll)
{
	g_aBg.ntype = nType;
	g_aBg.bscroll = bscroll;
	g_aBg.fPosTexU = 1.0f;
}

void ScrollBg(bool bscroll)
{
	g_aBg.bscroll = bscroll;
}