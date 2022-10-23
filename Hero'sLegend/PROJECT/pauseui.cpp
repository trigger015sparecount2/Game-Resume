//-------------------------
//Author:三上航世
//ポーズ画面UI処理(pauseui.cpp)
//-------------------------
#include "pauseui.h"
#include "camera.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturepauseUi = NULL;    //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffpauseUi = NULL; //頂点バッファへのポインタ
D3DXVECTOR3 g_apauseUiPos;
D3DXMATRIX g_mtxWorldpauseUi;

//初期化処理
void InitpauseUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	int nCntUi = 0;
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/.png",
		&g_pTexturepauseUi);
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffpauseUi,
		NULL);
	VERTEX_3D *pVtx; //頂点情報へのポインタ
					 //頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffpauseUi->Lock(0, 0, (void **)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-10000.0f, 10000.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(10000.0f, 10000.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-10000.0f, -10000.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(10000.0f, -10000.0f, 0.0f);
	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(0, 0, 0, 200);
	pVtx[1].col = D3DCOLOR_RGBA(0, 0, 0, 200);
	pVtx[2].col = D3DCOLOR_RGBA(0, 0, 0, 200);
	pVtx[3].col = D3DCOLOR_RGBA(0, 0, 0, 200);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	//頂点バッファをアンロックする
	g_pVtxBuffpauseUi->Unlock();
}

//UIの終了処理
void UninitpauseUi(void)
{
	//テクスチャの破棄
	if (g_pTexturepauseUi != NULL)
	{
		g_pTexturepauseUi->Release();
		g_pTexturepauseUi = NULL;
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
	
}


//描画処理
void DrawpauseUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	D3DXMATRIX mtxTrans; //計算用マトリックス
	D3DXMATRIX mtxView;
	pDevice = GetDevice();                        //デバイスを取得する
	//ラインティングを無視する
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//常に表示されるようにする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldpauseUi);
	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&g_mtxWorldpauseUi, NULL, &mtxView); //逆行列を求める
	g_mtxWorldpauseUi._41 = 0.0f;
	g_mtxWorldpauseUi._42 = 0.0f;
	g_mtxWorldpauseUi._43 = 0.0f;
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_apauseUiPos.x, g_apauseUiPos.y, g_apauseUiPos.z);
	D3DXMatrixMultiply(&g_mtxWorldpauseUi, &g_mtxWorldpauseUi, &mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldpauseUi);
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffpauseUi, 0, sizeof(VERTEX_3D));
	pDevice->SetFVF(FVF_VERTEX_3D);               //頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTexturepauseUi);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,  //開始する始点のインデックス
		2); //描画するプリミティブ数
			//ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//位置によっては映らないようにする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void Setpause(void)
{
	g_apauseUiPos = CameraPosR();
}