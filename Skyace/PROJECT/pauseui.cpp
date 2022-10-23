//-------------------------
//Author:三上航世
//ポーズ画面UI処理(pauseui.cpp)
//-------------------------
#include "pauseui.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "game.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturepauseUi[5] = {};    //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffpauseUi = NULL; //頂点バッファへのポインタ
D3DXVECTOR3 g_apauseUiPos[4];   //各ポリゴンの位置(四角、文*3)
int g_ncommand;

//初期化処理
void InitpauseUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntUi = 0;
	g_ncommand = 0;
	pDevice = GetDevice();
	g_apauseUiPos[nCntUi] = D3DXVECTOR3(640.0f, 360.0f, 0.0f);//四角
	for (nCntUi++; nCntUi < 4; nCntUi++)
	{
		g_apauseUiPos[nCntUi] = D3DXVECTOR3(640.0f, 230.0f + (nCntUi - 1) * 100.0f, 0.0f);//続ける、このステージからやり直す、タイトルに戻る
	}
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/.png",
		&g_pTexturepauseUi[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/縦長四角.png",
		&g_pTexturepauseUi[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/gamestart.png",
		&g_pTexturepauseUi[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/restartgame.png",
		&g_pTexturepauseUi[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/QUIT.png",
		&g_pTexturepauseUi[4]);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 5,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffpauseUi,
		NULL);
	VERTEX_2D *pVtx;
	nCntUi = 0;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffpauseUi->Lock(0, 0, (void **)&pVtx, 0);
	//薄暗く
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
	pVtx[0].col = D3DCOLOR_RGBA(0, 0, 0, 150);
	pVtx[1].col = D3DCOLOR_RGBA(0, 0, 0, 150);
	pVtx[2].col = D3DCOLOR_RGBA(0, 0, 0, 150);
	pVtx[3].col = D3DCOLOR_RGBA(0, 0, 0, 150);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	//四角
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_apauseUiPos[nCntUi].x - 150, g_apauseUiPos[nCntUi].y - 250, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_apauseUiPos[nCntUi].x + 150, g_apauseUiPos[nCntUi].y - 250, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_apauseUiPos[nCntUi].x - 150, g_apauseUiPos[nCntUi].y + 250, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_apauseUiPos[nCntUi].x + 150, g_apauseUiPos[nCntUi].y + 250, 0.0f);
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
	for (nCntUi++; nCntUi < 4; nCntUi++)
	{
		//文字
		//頂点座標の設定
		if (nCntUi < 3)
		{
			pVtx[0].pos = D3DXVECTOR3(g_apauseUiPos[nCntUi].x - 130, g_apauseUiPos[nCntUi].y - 25, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_apauseUiPos[nCntUi].x + 130, g_apauseUiPos[nCntUi].y - 25, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_apauseUiPos[nCntUi].x - 130, g_apauseUiPos[nCntUi].y + 25, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_apauseUiPos[nCntUi].x + 130, g_apauseUiPos[nCntUi].y + 25, 0.0f);
		}
		else
		{
			pVtx[0].pos = D3DXVECTOR3(g_apauseUiPos[nCntUi].x - 60, g_apauseUiPos[nCntUi].y - 25, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_apauseUiPos[nCntUi].x + 60, g_apauseUiPos[nCntUi].y - 25, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_apauseUiPos[nCntUi].x - 60, g_apauseUiPos[nCntUi].y + 25, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_apauseUiPos[nCntUi].x + 60, g_apauseUiPos[nCntUi].y + 25, 0.0f);
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffpauseUi->Unlock();
}

//UIの終了処理
void UninitpauseUi(void)
{
	int nCntUi;
	for (nCntUi = 0; nCntUi < 5; nCntUi++)
	{
		//テクスチャの破棄
		if (g_pTexturepauseUi[nCntUi] != NULL)
		{
			g_pTexturepauseUi[nCntUi]->Release();
			g_pTexturepauseUi[nCntUi] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffpauseUi != NULL)
	{
		g_pVtxBuffpauseUi->Release();
		g_pVtxBuffpauseUi = NULL;
	}
}

//UIの更新処理
void UpdatepauseUi(void)
{
	int nCntpauseUi;
	if (GetKeyboardTrigger(DIK_W) == true && GetKeyboardRelease(DIK_W) == false)
	{
		PlaySound(SOUND_LABEL_CHOICE000);
		g_ncommand--;
		if (g_ncommand < 0)
		{
			g_ncommand = 2;
		}
	}
	if (GetKeyboardTrigger(DIK_S) == true && GetKeyboardRelease(DIK_S) == false)
	{
		PlaySound(SOUND_LABEL_CHOICE000);
		g_ncommand = (g_ncommand + 1) % 3;
	}
	if (GetKeyboardTrigger(DIK_P) == true && GetKeyboardRelease(DIK_P) == false)
	{
		g_ncommand = 0;
	}
	if (GetKeyboardTrigger(DIK_J) == true && GetKeyboardRelease(DIK_J) == false)
	{
		PlaySound(SOUND_LABEL_YES000);
		switch (g_ncommand)
		{
		case 0:
			StartGame();
			break;
		case 1:
			RestartGame();
			StartGame();
			g_ncommand = 0;
			break;
		case 2:
			SetFade(MODE_TITLE, true);
			break;
		}
	}
	for (nCntpauseUi = 0; nCntpauseUi < 3; nCntpauseUi++)
	{
		VERTEX_2D *pVtx;
		g_pVtxBuffpauseUi->Lock(0, 0, (void **)&pVtx, 0);
		pVtx += nCntpauseUi * 4 + 8;
		pVtx[0].col = D3DCOLOR_RGBA(120, 120, 120, 255);
		pVtx[1].col = D3DCOLOR_RGBA(120, 120, 120, 255);
		pVtx[2].col = D3DCOLOR_RGBA(120, 120, 120, 255);
		pVtx[3].col = D3DCOLOR_RGBA(120, 120, 120, 255);
		g_pVtxBuffpauseUi->Unlock();
	}
	VERTEX_2D *pVtx;
	g_pVtxBuffpauseUi->Lock(0, 0, (void **)&pVtx, 0);
	pVtx += g_ncommand * 4 + 8;
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_pVtxBuffpauseUi->Unlock();
}


//描画処理
void DrawpauseUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	int nCntUi;
	pDevice = GetDevice();                        //デバイスを取得する
												  //頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffpauseUi, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	for (nCntUi = 0; nCntUi < 5; nCntUi++)
	{
		pDevice->SetTexture(0, g_pTexturepauseUi[nCntUi]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntUi * 4,
			2);
	}
}
