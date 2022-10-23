//-------------------------
//Author:三上航世
//リザルト画面UI処理(resultui.cpp)
//-------------------------
#include "pauseui.h"
#include "input.h"
#include "gamepad.h"
#include "sound.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturepauseUi[5] = {};    //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffpauseUI = NULL;//頂点バッファへのポインタ
int g_nSelect;

//初期化処理
HRESULT InitpauseUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntUi = 0;
	D3DXVECTOR3 pos[4];
	pDevice = GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/BgBlack.png",
		&g_pTexturepauseUi[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/square.png",
		&g_pTexturepauseUi[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/continue.png",
		&g_pTexturepauseUi[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/retry.png",
		&g_pTexturepauseUi[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/quit.png",
		&g_pTexturepauseUi[4]);
	g_nSelect = 0;

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 5,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffpauseUI,
		NULL);

	VERTEX_2D *pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データ絵のポインタを取得
	g_pVtxBuffpauseUI->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの設定(u,v)float 背景
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
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 160);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 160);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 160);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 160);
	pVtx += 4;

	pos[nCntUi] = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	//テクスチャの設定(u,v)float 四角
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//座標の設定(x,y)float
	pVtx[0].pos = D3DXVECTOR3(pos[nCntUi].x - 200.0f, pos[nCntUi].y - 240.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos[nCntUi].x + 200.0f, pos[nCntUi].y - 240.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos[nCntUi].x - 200.0f, pos[nCntUi].y + 240.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos[nCntUi].x + 200.0f, pos[nCntUi].y + 240.0f, 0.0f);

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
	nCntUi++;

	pos[nCntUi] = D3DXVECTOR3(640.0f, 210.0f, 0.0f);
	//テクスチャの設定(u,v)float 続ける
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//座標の設定(x,y)float
	pVtx[0].pos = D3DXVECTOR3(pos[nCntUi].x - 160.0f, pos[nCntUi].y - 40.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos[nCntUi].x + 160.0f, pos[nCntUi].y - 40.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos[nCntUi].x - 160.0f, pos[nCntUi].y + 40.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos[nCntUi].x + 160.0f, pos[nCntUi].y + 40.0f, 0.0f);

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
	nCntUi++;

	pos[nCntUi] = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	//テクスチャの設定(u,v)float やり直し
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//座標の設定(x,y)float
	pVtx[0].pos = D3DXVECTOR3(pos[nCntUi].x - 160.0f, pos[nCntUi].y - 40.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos[nCntUi].x + 160.0f, pos[nCntUi].y - 40.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos[nCntUi].x - 160.0f, pos[nCntUi].y + 40.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos[nCntUi].x + 160.0f, pos[nCntUi].y + 40.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);
	pVtx += 4;
	nCntUi++;

	pos[nCntUi] = D3DXVECTOR3(640.0f, 510.0f, 0.0f);
	//テクスチャの設定(u,v)float タイトル
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//座標の設定(x,y)float
	pVtx[0].pos = D3DXVECTOR3(pos[nCntUi].x - 160.0f, pos[nCntUi].y - 40.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos[nCntUi].x + 160.0f, pos[nCntUi].y - 40.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos[nCntUi].x - 160.0f, pos[nCntUi].y + 40.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos[nCntUi].x + 160.0f, pos[nCntUi].y + 40.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);
	//頂点バッファをアンロックする
	g_pVtxBuffpauseUI->Unlock();

	return S_OK;
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
}

//UIの更新処理
void UpdatepauseUi(void)
{
	if (GetKeyboardKey(DIK_UP) == true || CrossKey(0) == true)
	{
		PlaySound(SOUND_LABEL_SELECT);
		int nSelect;
		nSelect = g_nSelect;
		VERTEX_2D *pVtx;//頂点情報へのポインタ
		//頂点バッファをロックし、頂点データ絵のポインタを取得
		g_pVtxBuffpauseUI->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (g_nSelect + 2) * 4;
		//カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_nSelect--;
		if (g_nSelect < 0)
		{
			g_nSelect = 2;
		}

		pVtx += (g_nSelect - nSelect) * 4;
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//頂点バッファをアンロックする
		g_pVtxBuffpauseUI->Unlock();
	}
	if (GetKeyboardKey(DIK_DOWN) == true || CrossKey(18000) == true)
	{
		PlaySound(SOUND_LABEL_SELECT);
		int nSelect;
		nSelect = g_nSelect;
		VERTEX_2D *pVtx;//頂点情報へのポインタ
						//頂点バッファをロックし、頂点データ絵のポインタを取得
		g_pVtxBuffpauseUI->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (g_nSelect + 2) * 4;
		//カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		g_nSelect++;
		if (g_nSelect > 2)
		{
			g_nSelect = 0;
		}

		pVtx += (g_nSelect - nSelect) * 4;
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//頂点バッファをアンロックする
		g_pVtxBuffpauseUI->Unlock();
	}
}


//描画処理
void DrawpauseUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ
	pDevice = GetDevice();//デバイスの取得
	int nCntUI;
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffpauseUI, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定
	for (nCntUI = 0; nCntUI < 5; nCntUI++)
	{
		pDevice->SetTexture(0, g_pTexturepauseUi[nCntUI]);//テクスチャの設定
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4, 2);//ポリゴンの描画
	}
}

int GetSelect(void)
{
	return g_nSelect;
}

void ResetpauseUi(void)
{
	g_nSelect = 0;
	int nCntUI;
	VERTEX_2D *pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データ絵のポインタを取得
	g_pVtxBuffpauseUI->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += 4 * 2;
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	for (nCntUI = 0; nCntUI < 2; nCntUI++)
	{
		pVtx += 4;
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);
	}
	//頂点バッファをアンロックする
	g_pVtxBuffpauseUI->Unlock();
}