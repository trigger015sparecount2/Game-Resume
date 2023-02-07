//-------------------------
//Author:三上航世
//リザルト画面UI処理(resultui.cpp)
//-------------------------
#include "resultui.h"
#include "player.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureresultUi[4] = {};    //テクスチャへのポインタ
D3DXVECTOR3 g_aresultPos[2], g_aresultCenter[2];   //各ポリゴンの位置
RECT g_rectresult[2];
D3DXVECTOR3 g_Scaleresult[2];
D3DXMATRIX g_mtxresult[2];

//初期化処理
void InitresultUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntUi = 0;
	pDevice = GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/gameclear.JPG",
		&g_pTextureresultUi[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/gameover.JPG",
		&g_pTextureresultUi[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/clear.png",
		&g_pTextureresultUi[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/failed.png",
		&g_pTextureresultUi[3]);

	g_aresultPos[nCntUi] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//背景
	g_rectresult[nCntUi].top = 0;
	g_rectresult[nCntUi].left = 0;
	g_rectresult[nCntUi].right = 2048;
	g_rectresult[nCntUi].bottom = 1024;
	g_aresultCenter[nCntUi] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Scaleresult[nCntUi] = D3DXVECTOR3(0.7f, 0.7f, 1.0f);

	nCntUi++;
	g_aresultPos[nCntUi] = D3DXVECTOR3(200.0f, 30.0f, 0.0f);//文字
	g_rectresult[nCntUi].top = 0;
	g_rectresult[nCntUi].left = 0;
	g_rectresult[nCntUi].right = 512;
	g_rectresult[nCntUi].bottom = 128;
	g_aresultCenter[nCntUi] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Scaleresult[nCntUi] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

//UIの終了処理
void UninitresultUi(void)
{
	int nCntUi;
	for (nCntUi = 0; nCntUi < 4; nCntUi++)
	{
		//テクスチャの破棄
		if (g_pTextureresultUi[nCntUi] != NULL)
		{
			g_pTextureresultUi[nCntUi]->Release();
			g_pTextureresultUi[nCntUi] = NULL;
		}
	}
}

//UIの更新処理
void UpdateresultUi(void)
{

}


//描画処理
void DrawresultUi(void)
{
	LPD3DXSPRITE pSprite; //デバイスのポインタ
	int ntex = 0;
	int nCntui = 0;
	pSprite = GetSprite();     //デバイスを取得する
	for (nCntui = 0; nCntui < 2; nCntui++)
	{
		D3DXMATRIX mtxTransUI, mtxScaleUI;
		//UIのワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxresult[nCntui]);

		//拡大率を反映
		D3DXMatrixScaling(&mtxScaleUI, g_Scaleresult[nCntui].x, g_Scaleresult[nCntui].y, g_Scaleresult[nCntui].z);
		D3DXMatrixMultiply(&g_mtxresult[nCntui], &g_mtxresult[nCntui], &mtxScaleUI);

		//位置を反映
		D3DXMatrixTranslation(&mtxTransUI, g_aresultPos[nCntui].x, g_aresultPos[nCntui].y, g_aresultPos[nCntui].z);
		D3DXMatrixMultiply(&g_mtxresult[nCntui], &g_mtxresult[nCntui], &mtxTransUI);

		//ワールドマトリックスの設定
		pSprite->SetTransform(&g_mtxresult[nCntui]);

		//画像、画像の描画領域、画像の中心座標、画像が貼られる場所、色
		if(nCntui == 0)
		{
			if (PlayerHP() > 0)
			{
				ntex = 0;
			}
			else
			{
				ntex = 1;
			}
		}
		else
		{
			ntex += 2;
		}
		pSprite->Draw(g_pTextureresultUi[ntex], &g_rectresult[nCntui], &g_aresultCenter[nCntui], &g_aresultPos[nCntui], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}
