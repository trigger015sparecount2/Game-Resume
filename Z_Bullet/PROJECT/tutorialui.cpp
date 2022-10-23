//-------------------------
//Author:三上航世
//チュートリアル画面UI処理(tutorialui.cpp)
//-------------------------
#include "tutorialui.h"
#include "mouse.h"
#include "gamepad.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturetutorialUi[2] = {};    //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufftutoUI = NULL;//頂点バッファへのポインタ

//初期化処理
void InittutorialUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	D3DXVECTOR3 pos;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBufftutoUI,
		NULL);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/square.png",
		&g_pTexturetutorialUi[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/tutotext.png",
		&g_pTexturetutorialUi[1]);

	pos = D3DXVECTOR3(1140.0f, 400.0f, 0.0f);

	VERTEX_2D *pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データ絵のポインタを取得
	g_pVtxBufftutoUI->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//座標の設定(x,y)float
	pVtx[0].pos = D3DXVECTOR3(pos.x - 120.0f, pos.y - 180.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + 120.0f, pos.y - 180.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - 120.0f, pos.y + 180.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + 120.0f, pos.y + 180.0f, 0.0f);

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
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.4f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.4f);

	//座標の設定(x,y)float
	pVtx[0].pos = D3DXVECTOR3(pos.x - 115.0f, pos.y - 170.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + 115.0f, pos.y - 170.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - 115.0f, pos.y + 170.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + 115.0f, pos.y + 170.0f, 0.0f);

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

	//頂点バッファをアンロックする
	g_pVtxBufftutoUI->Unlock();
}

//UIの終了処理
void UninittutorialUi(void)
{
	int nCntUi;
	for (nCntUi = 0; nCntUi < 2; nCntUi++)
	{
		//テクスチャの破棄
		if (g_pTexturetutorialUi[nCntUi] != NULL)
		{
			g_pTexturetutorialUi[nCntUi]->Release();
			g_pTexturetutorialUi[nCntUi] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBufftutoUI != NULL)
	{
		g_pVtxBufftutoUI->Release();
		g_pVtxBufftutoUI = NULL;
	}
}

//UIの更新処理
void UpdatetutorialUi(void)
{
	if (MouseZ() > 0.0f || PressCrossKey(0) == true && GamePad() == true)
	{

		VERTEX_2D *pVtx;//頂点情報へのポインタ
		//頂点バッファをロックし、頂点データ絵のポインタを取得
		g_pVtxBufftutoUI->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += 4;
		//テクスチャの設定(u,v)float
		if (pVtx[0].tex.y > 0.0f)
		{
			pVtx[0].tex.y -= 0.01f;
			pVtx[1].tex.y -= 0.01f;
			pVtx[2].tex.y -= 0.01f;
			pVtx[3].tex.y -= 0.01f;
		}
		//頂点バッファをアンロックする
		g_pVtxBufftutoUI->Unlock();
	}
	if (MouseZ() < 0.0f || PressCrossKey(18000) == true)
	{
		VERTEX_2D *pVtx;//頂点情報へのポインタ

		//頂点バッファをロックし、頂点データ絵のポインタを取得
		g_pVtxBufftutoUI->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += 4;
		//テクスチャの設定(u,v)float
		if (pVtx[2].tex.y < 1.0f)
		{
			pVtx[0].tex.y += 0.01f;
			pVtx[1].tex.y += 0.01f;
			pVtx[2].tex.y += 0.01f;
			pVtx[3].tex.y += 0.01f;
		}
		//頂点バッファをアンロックする
		g_pVtxBufftutoUI->Unlock();
	}
}


//描画処理
void DrawtutorialUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ
	pDevice = GetDevice();//デバイスの取得
	int nCntUI;
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBufftutoUI, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定
	for (nCntUI = 0; nCntUI < 2; nCntUI++)
	{
		pDevice->SetTexture(0, g_pTexturetutorialUi[nCntUI]);//テクスチャの設定
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4, 2);//ポリゴンの描画
	}
}