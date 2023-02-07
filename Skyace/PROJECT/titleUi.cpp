//-------------------------
//Author:三上航世
//選択画面UI処理(choiceui.cpp)
//-------------------------
#include "titleui.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturetitleUi[2] = {};    //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufftitleUi = NULL; //頂点バッファへのポインタ
D3DXVECTOR3 g_atitlePos[2];   //各ポリゴンの位置(タイトルロゴ、Jキー)
bool g_bPressClear;
int g_nPressClear;

//初期化処理
void InittitleUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntUi = 0;
	g_nPressClear = 255;
	g_bPressClear = true;
	pDevice = GetDevice();
	g_atitlePos[nCntUi] = D3DXVECTOR3(640.0f, 100.0f, 0.0f);//タイトル
	nCntUi++;
	g_atitlePos[nCntUi] = D3DXVECTOR3(640.0f, 600.0f, 0.0f);//PushPressJ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/タイトルロゴ.png",
		&g_pTexturetitleUi[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/PressJ.png",
		&g_pTexturetitleUi[1]);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBufftitleUi,
		NULL);
	
	VERTEX_2D *pVtx;
	nCntUi = 0;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBufftitleUi->Lock(0, 0, (void **)&pVtx, 0);
	//タイトルロゴ
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_atitlePos[nCntUi].x - 300.0f, g_atitlePos[nCntUi].y - 80.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_atitlePos[nCntUi].x + 300.0f, g_atitlePos[nCntUi].y - 80.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_atitlePos[nCntUi].x - 300.0f, g_atitlePos[nCntUi].y + 80.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_atitlePos[nCntUi].x + 300.0f, g_atitlePos[nCntUi].y + 80.0f, 0.0f);
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
	nCntUi++;
	//pressJkey
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_atitlePos[nCntUi].x - 230.0f, g_atitlePos[nCntUi].y - 30.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_atitlePos[nCntUi].x + 230.0f, g_atitlePos[nCntUi].y - 30.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_atitlePos[nCntUi].x - 230.0f, g_atitlePos[nCntUi].y + 30.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_atitlePos[nCntUi].x + 230.0f, g_atitlePos[nCntUi].y + 30.0f, 0.0f);
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
	//頂点バッファをアンロックする
	g_pVtxBufftitleUi->Unlock();
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
	if (g_pVtxBufftitleUi != NULL)
	{
		g_pVtxBufftitleUi->Release();
		g_pVtxBufftitleUi = NULL;
	}
}

//UIの更新処理
void UpdatetitleUi(void)
{
	//PRESSを明るくしたり暗くしたりする
	if (g_bPressClear == true)
	{
		g_nPressClear -= 15;
		if (g_nPressClear <= 0)
		{
			g_nPressClear = 0;
			g_bPressClear = false;
		}
	}
	else
	{
		g_nPressClear += 15;
		if (g_nPressClear >= 255)
		{
			g_nPressClear = 255;
			g_bPressClear = true;
		}
	}

	VERTEX_2D *pVtx;
	g_pVtxBufftitleUi->Lock(0, 0, (void **)&pVtx, 0);
	pVtx += 4;
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_nPressClear);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_nPressClear);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_nPressClear);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_nPressClear);
	g_pVtxBufftitleUi->Unlock();
}


//描画処理
void DrawtitleUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	int nCntUi = 0;
	pDevice = GetDevice();                        //デバイスを取得する
												  //頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBufftitleUi, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	for (nCntUi = 0; nCntUi < 2; nCntUi++)
	{
		pDevice->SetTexture(0, g_pTexturetitleUi[nCntUi]);//タイトル、press
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntUi * 4,
			2);
	}
}