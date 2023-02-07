//-------------------------
//Author:三上航世
//チュートリアル画面UI処理(tutorialui.cpp)
//-------------------------
#include "tutorialui.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturetutorialUi = NULL;    //テクスチャへのポインタ
RECT g_recttutoUI; //画像の描画領域
D3DXVECTOR3 g_centertuto, g_postuto; //画像の中心、貼られる場所
D3DXVECTOR3 g_Scaletuto;
D3DXMATRIX g_mtxtutoUi;

//初期化処理
void InittutorialUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	g_postuto = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//PRESS
	g_centertuto = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_recttutoUI.top = 0;
	g_recttutoUI.left = 0;
	g_recttutoUI.bottom = 1024;
	g_recttutoUI.right = 2048;
	g_Scaletuto = D3DXVECTOR3(0.65f, 0.73f, 1.0f);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/tutorial.jpg",
		&g_pTexturetutorialUi);
}

//UIの終了処理
void UninittutorialUi(void)
{
	//テクスチャの破棄
	if (g_pTexturetutorialUi != NULL)
	{
		g_pTexturetutorialUi->Release();
		g_pTexturetutorialUi = NULL;
	}
}

//UIの更新処理
void UpdatetutorialUi(void)
{
	
}


//描画処理
void DrawtutorialUi(void)
{
	LPD3DXSPRITE pSprite; //デバイスのポインタ
	pSprite = GetSprite();     //デバイスを取得する
	D3DXMATRIX mtxTransUI, mtxScaleUI;

	//UIのワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxtutoUi);

	//拡大率を反映
	D3DXMatrixScaling(&mtxScaleUI, g_Scaletuto.x, g_Scaletuto.y, g_Scaletuto.z);
	D3DXMatrixMultiply(&g_mtxtutoUi, &g_mtxtutoUi, &mtxScaleUI);

	//位置を反映
	D3DXMatrixTranslation(&mtxTransUI, g_postuto.x, g_postuto.y, g_postuto.z);
	D3DXMatrixMultiply(&g_mtxtutoUi, &g_mtxtutoUi, &mtxTransUI);

	//ワールドマトリックスの設定
	pSprite->SetTransform(&g_mtxtutoUi);

	//画像、画像の描画領域、画像の中心座標、画像が貼られる場所、色
	pSprite->Draw(g_pTexturetutorialUi, &g_recttutoUI, &g_centertuto, &g_postuto, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}