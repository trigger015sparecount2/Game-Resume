//-------------------------
//Author:三上航世
//チュートリアル画面UI処理(tutorialui.cpp)
//-------------------------
#include "tutorialui.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "padinput.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturetutorialUi[5] = {};    //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufftutorialUi = NULL; //頂点バッファへのポインタ
D3DXVECTOR3 g_atutorialPos[2];   //ポリゴンの位置(pressenter)
bool g_bPressCleartuto;
int g_nPressCleartuto;
int g_nPage;

//初期化処理
void InittutorialUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	g_nPressCleartuto = 255;
	g_bPressCleartuto = true;
	g_nPage = 0;
	pDevice = GetDevice();
	g_atutorialPos[0] = D3DXVECTOR3(1140.0f, 50.0f, 0.0f);//PRESS
	g_atutorialPos[1] = D3DXVECTOR3(140.0f, 50.0f, 0.0f);//PREV
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/tutorial1.JPG",
		&g_pTexturetutorialUi[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/tutorial2.JPG",
		&g_pTexturetutorialUi[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/nextpage.png",
		&g_pTexturetutorialUi[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/gamestart.png",
		&g_pTexturetutorialUi[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/prevpage.png",
		&g_pTexturetutorialUi[4]);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 3,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBufftutorialUi,
		NULL);
	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBufftutorialUi->Lock(0, 0, (void **)&pVtx, 0);
	//チュートリアル画面
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);
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
	//nextpage,gamestart
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_atutorialPos[0].x - 120, g_atutorialPos[0].y - 40, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_atutorialPos[0].x + 120, g_atutorialPos[0].y - 40, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_atutorialPos[0].x - 120, g_atutorialPos[0].y + 40, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_atutorialPos[0].x + 120, g_atutorialPos[0].y + 40, 0.0f);
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
	//prevpage
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_atutorialPos[1].x - 120, g_atutorialPos[1].y - 40, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_atutorialPos[1].x + 120, g_atutorialPos[1].y - 40, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_atutorialPos[1].x - 120, g_atutorialPos[1].y + 40, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_atutorialPos[1].x + 120, g_atutorialPos[1].y + 40, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	//頂点バッファをアンロックする
	g_pVtxBufftutorialUi->Unlock();
}

//UIの終了処理
void UninittutorialUi(void)
{
	int nCntUi;
	for (nCntUi = 0; nCntUi < 5; nCntUi++)
	{
		//テクスチャの破棄
		if (g_pTexturetutorialUi[nCntUi] != NULL)
		{
			g_pTexturetutorialUi[nCntUi]->Release();
			g_pTexturetutorialUi[nCntUi] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBufftutorialUi != NULL)
	{
		g_pVtxBufftutorialUi->Release();
		g_pVtxBufftutorialUi = NULL;
	}
}

//UIの更新処理
void UpdatetutorialUi(void)
{
	if (g_bPressCleartuto == true)
	{
		g_nPressCleartuto -= 15;
		if (g_nPressCleartuto <= 0)
		{
			g_nPressCleartuto = 0;
			g_bPressCleartuto = false;
		}
	}
	else
	{
		g_nPressCleartuto += 15;
		if (g_nPressCleartuto >= 255)
		{
			g_nPressCleartuto = 255;
			g_bPressCleartuto = true;
		}
	}
	if (GetKeyboardKey(DIK_RETURN) == true && g_nPage == 1 && GetFade() == FADE_NONE ||
		GetGamePadButton(11) == true && g_nPage == 1 && GetFade() == FADE_NONE ||
		GetGamePadButton(3) == true && g_nPage == 1 && GetFade() == FADE_NONE)
	{
		SetFade(MODE_GAME);
		PlaySound(SOUND_LABEL_YES);
	}
	if (GetKeyboardKey(DIK_A) == true || CrossKey() == 27000.0f)
	{
		if (g_nPage >= 1)
		{
			g_nPage = 0;
			PlaySound(SOUND_LABEL_CHOICE);
		}
	}
	if (GetKeyboardKey(DIK_D) == true || GetGamePadButton(3) == true || CrossKey() == 9000.0f)
	{
		if (g_nPage <= 0)
		{
			g_nPage = 1;
			PlaySound(SOUND_LABEL_CHOICE);
		}
	}
	VERTEX_2D *pVtx;
	g_pVtxBufftutorialUi->Lock(0, 0, (void **)&pVtx, 0);
	pVtx += 4;
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_nPressCleartuto);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_nPressCleartuto);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_nPressCleartuto);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_nPressCleartuto);
	pVtx += 4;
	if (g_nPage == 0)
	{
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	}
	else
	{
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_nPressCleartuto);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_nPressCleartuto);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_nPressCleartuto);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_nPressCleartuto);
	}
	g_pVtxBufftutorialUi->Unlock();
}


//描画処理
void DrawtutorialUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	pDevice = GetDevice();                        //デバイスを取得する
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBufftutorialUi, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTexturetutorialUi[g_nPage]);//画面
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		2);
	pDevice->SetTexture(0, g_pTexturetutorialUi[g_nPage + 2]);//press,next
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		4,
		2);
	pDevice->SetTexture(0, g_pTexturetutorialUi[4]);//press,next
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		8,
		2);
}