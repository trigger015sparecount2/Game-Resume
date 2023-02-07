//-------------------------
//Author:三上航世
//選択画面UI処理(choiceui.cpp)
//-------------------------
#include "titleui.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturetitleUi[3] = {};    //テクスチャへのポインタ
D3DXVECTOR3 g_atitlePos[3],g_atitleCenter[3];   //各ポリゴンの位置(タイトルロゴ、Jキー)
RECT g_recttitle[3];
D3DXVECTOR3 g_Scaletitle[3];
D3DXMATRIX g_mtxtitleUi[3];
D3DXCOLOR g_colortitle[3];
bool g_bPressClear;

//初期化処理
void InittitleUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntUi = 0;
	g_bPressClear = true;
	pDevice = GetDevice();

	g_atitlePos[nCntUi] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//タイトル
	g_recttitle[nCntUi].top = 0;
	g_recttitle[nCntUi].left = 0;
	g_recttitle[nCntUi].right = 2048;
	g_recttitle[nCntUi].bottom = 1024;
	g_Scaletitle[nCntUi] = D3DXVECTOR3(0.65f, 0.75f, 0.0f);
	g_atitleCenter[nCntUi] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_colortitle[nCntUi] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	nCntUi++;

	g_atitlePos[nCntUi] = D3DXVECTOR3(0.0f, 70.0f, 0.0f);//タイトル
	g_recttitle[nCntUi].top = 0;
	g_recttitle[nCntUi].left = 0;
	g_recttitle[nCntUi].right = 512;
	g_recttitle[nCntUi].bottom = 256;
	g_Scaletitle[nCntUi] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	g_atitleCenter[nCntUi] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_colortitle[nCntUi] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	nCntUi++;

	g_atitlePos[nCntUi] = D3DXVECTOR3(300.0f, 430.0f, 0.0f);//PushPressJ
	g_recttitle[nCntUi].top = 0;
	g_recttitle[nCntUi].left = 0;
	g_recttitle[nCntUi].right = 1024;
	g_recttitle[nCntUi].bottom = 128;
	g_Scaletitle[nCntUi] = D3DXVECTOR3(0.4f, 0.4f, 0.0f);
	g_atitleCenter[nCntUi] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_colortitle[nCntUi] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/title.JPG",
		&g_pTexturetitleUi[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/title.png",
		&g_pTexturetitleUi[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/press.png",
		&g_pTexturetitleUi[2]);
	
}

//UIの終了処理
void UninittitleUi(void)
{
	int nCntUi;
	for (nCntUi = 0; nCntUi < 3; nCntUi++)
	{
		//テクスチャの破棄
		if (g_pTexturetitleUi[nCntUi] != NULL)
		{
			g_pTexturetitleUi[nCntUi]->Release();
			g_pTexturetitleUi[nCntUi] = NULL;
		}
	}
}

//UIの更新処理
void UpdatetitleUi(void)
{
	//点いた
	if (g_bPressClear == true)
	{
		//薄くする
		g_colortitle[2].a -= 0.04f;
		if (g_colortitle[2].a <= 0.0f)
		{
			g_colortitle[2].a = 0.0f;
			g_bPressClear = false;
		}
	}
	//消えた
	else
	{
		//濃くする
		g_colortitle[2].a += 0.04f;
		if (g_colortitle[2].a >= 1.0f)
		{
			g_colortitle[2].a = 1.0f;
			g_bPressClear = true;
		}
	}
}


//描画処理
void DrawtitleUi(void)
{
	LPD3DXSPRITE pSprite; //デバイスのポインタ
	int nCntUi;
	pSprite = GetSprite();     //デバイスを取得する
	for (nCntUi = 0; nCntUi < 3; nCntUi++)
	{
		D3DXMATRIX mtxTransUI, mtxScaleUI;
		//UIのワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxtitleUi[nCntUi]);

		//拡大率を反映
		D3DXMatrixScaling(&mtxScaleUI, g_Scaletitle[nCntUi].x, g_Scaletitle[nCntUi].y, g_Scaletitle[nCntUi].z);
		D3DXMatrixMultiply(&g_mtxtitleUi[nCntUi], &g_mtxtitleUi[nCntUi], &mtxScaleUI);

		//位置を反映
		D3DXMatrixTranslation(&mtxTransUI, g_atitlePos[nCntUi].x, g_atitlePos[nCntUi].y, g_atitlePos[nCntUi].z);
		D3DXMatrixMultiply(&g_mtxtitleUi[nCntUi], &g_mtxtitleUi[nCntUi], &mtxTransUI);

		//ワールドマトリックスの設定
		pSprite->SetTransform(&g_mtxtitleUi[nCntUi]);

		//画像、画像の描画領域、画像の中心座標、画像が貼られる場所、色
		pSprite->Draw(g_pTexturetitleUi[nCntUi], &g_recttitle[nCntUi], &g_atitleCenter[nCntUi], &g_atitlePos[nCntUi], g_colortitle[nCntUi]);
	}
}