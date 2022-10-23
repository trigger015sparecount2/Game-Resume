//-------------------------
//Author:三上航世
//選択画面UI処理(choiceui.cpp)
//-------------------------
#include "titleui.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturetitleUi[2] = {};    //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufftitleUI = NULL;//頂点バッファへのポインタ
int g_nClear;
bool g_bPressClear;

//初期化処理
void InittitleUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntUi = 0;
	g_bPressClear = true;
	pDevice = GetDevice();

	D3DXVECTOR3 pos;

	pos = D3DXVECTOR3(640.0f, 500.0f, 0.0f);
	g_nClear = 0;
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBufftitleUI,
		NULL);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/title.png",
		&g_pTexturetitleUi[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/press.png",
		&g_pTexturetitleUi[1]);

	VERTEX_2D *pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データ絵のポインタを取得
	g_pVtxBufftitleUI->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//座標の設定(x,y)float
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx += 4;

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//座標の設定(x,y)float
	pVtx[0].pos = D3DXVECTOR3(pos.x - 240.0f, pos.y - 40.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + 240.0f, pos.y - 40.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - 240.0f, pos.y + 40.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + 240.0f, pos.y + 40.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);

	//頂点バッファをアンロックする
	g_pVtxBufftitleUI->Unlock();
}

//UIの終了処理
void UninittitleUi(void)
{
	int nCntUi;
	for (nCntUi = 0; nCntUi < 2; nCntUi++)
	{
		//テクスチャの破棄
		if (g_pTexturetitleUi[nCntUi] != NULL)
		{
			g_pTexturetitleUi[nCntUi]->Release();
			g_pTexturetitleUi[nCntUi] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBufftitleUI != NULL)
	{
		g_pVtxBufftitleUI->Release();
		g_pVtxBufftitleUI = NULL;
	}
}

//UIの更新処理
void UpdatetitleUi(void)
{
	if (g_bPressClear == true)
	{
		g_nClear -= 15;
		if (g_nClear <= 0)
		{
			g_nClear = 0;
			g_bPressClear = false;
		}
	}
	else
	{
		g_nClear += 15;
		if (g_nClear >= 255)
		{
			g_nClear = 255;
			g_bPressClear = true;
		}
	}
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	//頂点バッファをロックし、頂点データ絵のポインタを取得
	g_pVtxBufftitleUI->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += 4;
	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_nClear);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_nClear);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_nClear);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_nClear);

	//頂点バッファをアンロックする
	g_pVtxBufftitleUI->Unlock();
}


//描画処理
void DrawtitleUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ
	pDevice = GetDevice();//デバイスの取得
	int nCntUI;
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBufftitleUI, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定
	for (nCntUI = 0; nCntUI < 2; nCntUI++)
	{
		pDevice->SetTexture(0, g_pTexturetitleUi[nCntUI]);//テクスチャの設定
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4, 2);//ポリゴンの描画
	}
}