//-------------------------
//Author:三上航世
//UI処理(ui.cpp)
//-------------------------
#include "ui.h"
#include "player.h"

//マクロ定義
#define MAX_UI (2)

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureUi[MAX_UI] = {};	 //テクスチャへのポインタ
RECT g_rectUI[MAX_UI]; //画像の描画領域
D3DXVECTOR3 g_center[MAX_UI], g_pos[MAX_UI]; //画像の中心、貼られる場所
D3DXVECTOR3 g_Scale[MAX_UI];
D3DXMATRIX g_mtxUi[MAX_UI];

//初期化処理
HRESULT InitUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HPgaugeGw.png",
		&g_pTextureUi[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HPgauge.png",
		&g_pTextureUi[1]);

	g_rectUI[0].top = 0;
	g_rectUI[0].left = 0;
	g_rectUI[0].bottom = 64;
	g_rectUI[0].right = 512;
	g_center[0].x = 0.0f;
	g_center[0].y = 0.0f;
	g_pos[0].x = 530.0f;
	g_pos[0].y = 390.0f;
	g_Scale[0] = D3DXVECTOR3(0.7f, 0.7f, 1.0f);

	g_rectUI[1].top = 0;
	g_rectUI[1].left = 0;
	g_rectUI[1].bottom = 64;
	g_rectUI[1].right = 512;
	g_center[1].x = 1230.0f;
	g_center[1].y = 0.0f;
	g_pos[1].x = 1230.0f;
	g_pos[1].y = 402.9f;
	g_Scale[1] = D3DXVECTOR3(-0.64f, 0.65f, 1.0f);
	return S_OK;
}

//UIの終了処理
void UninitUi(void)
{
	int nCntTex;
	for (nCntTex = 0; nCntTex < MAX_UI; nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureUi[nCntTex] != NULL)
		{
			g_pTextureUi[nCntTex]->Release();
			g_pTextureUi[nCntTex] = NULL;
		}
	}
}

//UIの更新処理
void UpdateUi(void)
{
	
}


//描画処理
void DrawUi(void)
{
	LPD3DXSPRITE pSprite; //デバイスのポインタ
	int nCntUi;
	pSprite = GetSprite();     //デバイスを取得する
	for (nCntUi = 0; nCntUi < MAX_UI; nCntUi++)
	{
		D3DXMATRIX mtxTransUI, mtxScaleUI;
		//UIのワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxUi[nCntUi]);

		if (nCntUi == 1)
		{
			//拡大率を反映
			D3DXMatrixScaling(&mtxScaleUI, g_Scale[nCntUi].x * PlayerHPRate(), g_Scale[nCntUi].y, g_Scale[nCntUi].z);
		}
		else
		{
			//拡大率を反映
			D3DXMatrixScaling(&mtxScaleUI, g_Scale[nCntUi].x, g_Scale[nCntUi].y, g_Scale[nCntUi].z);
		}
		D3DXMatrixMultiply(&g_mtxUi[nCntUi], &g_mtxUi[nCntUi], &mtxScaleUI);

		//位置を反映
		D3DXMatrixTranslation(&mtxTransUI, g_pos[nCntUi].x, g_pos[nCntUi].y, g_pos[nCntUi].z);
		D3DXMatrixMultiply(&g_mtxUi[nCntUi], &g_mtxUi[nCntUi], &mtxTransUI);

		//ワールドマトリックスの設定
		pSprite->SetTransform(&g_mtxUi[nCntUi]);

		//画像、画像の描画領域、画像の中心座標、画像が貼られる場所、色
		pSprite->Draw(g_pTextureUi[nCntUi], &g_rectUI[nCntUi], &g_center[nCntUi], &g_pos[nCntUi], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}