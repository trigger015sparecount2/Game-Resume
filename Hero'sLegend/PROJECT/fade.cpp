//--------------------------
//Author:三上航世
//フェードアウトの処理(fade.cpp)
//--------------------------
#include "fade.h"
#include "camera.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;    //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL; //頂点バッファへのポインタ
D3DXMATRIX g_mtxWorldFade;
D3DXVECTOR3 g_posFade;
FADE g_fade;                                   //フェードの状態
MODE g_modeNext;                               //次の画面(モード)
D3DXCOLOR g_colorFade;                         //フェードの色

//初期化処理
void InitFade(MODE modeNext)
{
	g_fade = FADE_IN;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/シューティング操作説明.JPG",
		&g_pTextureFade);
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);
	VERTEX_3D *pVtx; //頂点情報へのポインタ
					 //頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void **)&pVtx, 0);
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
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();
}

//終了処理
void UninitFade(void)
{
	//テクスチャの破棄
	if (g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//更新処理
void UpdateFade(void)
{
	if (g_fade != FADE_NONE)
	{
		if (g_fade == FADE_IN)
		{
			g_colorFade.a -= 0.02f;
			if (g_colorFade.a <= 0.0f)
			{
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;
			}
		}
		else if (g_fade == FADE_OUT)
		{
			g_colorFade.a += 0.02f;
			if (g_colorFade.a >= 1.0f)
			{
				g_colorFade.a = 1.0f;
				g_fade = FADE_IN;
				//モードの設定
				SetMode(g_modeNext);
			}
		}
		VERTEX_3D *pVtx; //頂点情報へのポインタ
		g_pVtxBuffFade->Lock(0, 0, (void **)&pVtx, 0);
		pVtx[0].col = g_colorFade;
		pVtx[1].col = g_colorFade;
		pVtx[2].col = g_colorFade;
		pVtx[3].col = g_colorFade;
		g_pVtxBuffFade->Unlock();
	}
}

//描画処理
void DrawFade(void)
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
	D3DXMatrixIdentity(&g_mtxWorldFade);
	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&g_mtxWorldFade, NULL, &mtxView); //逆行列を求める
	g_mtxWorldFade._41 = 0.0f;
	g_mtxWorldFade._42 = 0.0f;
	g_mtxWorldFade._43 = 0.0f;
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posFade.x, g_posFade.y, g_posFade.z);
	D3DXMatrixMultiply(&g_mtxWorldFade, &g_mtxWorldFade, &mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldFade);
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_3D));
	pDevice->SetFVF(FVF_VERTEX_3D);               //頂点フォーマットの設定
	pDevice->SetTexture(0, NULL);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,  //開始する始点のインデックス
		2); //描画するプリミティブ数
	//ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//位置によっては映らないようにする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//フェードの設定処理
void SetFade(MODE modeNext)
{
	g_fade = FADE_OUT;
	g_modeNext = modeNext;
	g_posFade = CameraPosR();
}

//フェードの取得処理
FADE GetFade(void)
{
	return g_fade;
}