//---------------------------
//Author:三上航世
//残像処理(orbit.cpp)
//---------------------------
#include "orbit.h"

typedef struct
{
	int nTime; //消えるまでの時間
	int nCountTime; //経過時間
	D3DXVECTOR3 pos;
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	bool bUse;
} Orbit;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureOrbit = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffOrbit = NULL; //頂点バッファへのポインタ
Orbit g_Orbit[MAX_ORBIT];

//初期化処理
HRESULT InitOrbit(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	int nCntOrbit;
	for (nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		g_Orbit[nCntOrbit].nTime = 0;
		g_Orbit[nCntOrbit].nCountTime = 0;
		g_Orbit[nCntOrbit].bUse = false;
		g_Orbit[nCntOrbit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_ORBIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffOrbit,
		NULL);
	VERTEX_3D *pVtx; //頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffOrbit->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		//頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(100, 0, 100, 160);
		pVtx[1].col = D3DCOLOR_RGBA(100, 0, 100, 160);
		pVtx[2].col = D3DCOLOR_RGBA(100, 100, 100, 160);
		pVtx[3].col = D3DCOLOR_RGBA(100, 100, 100, 160);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffOrbit->Unlock();
	return S_OK;
}

//終了処理
void UninitOrbit(void)
{
	//テクスチャの破棄
	if (g_pTextureOrbit != NULL)
	{
		g_pTextureOrbit->Release();
		g_pTextureOrbit = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffOrbit != NULL)
	{
		g_pVtxBuffOrbit->Release();
		g_pVtxBuffOrbit = NULL;
	}
}

//更新処理
void UpdateOrbit(void)
{
	int nCntOrbit;
	for (nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		if (g_Orbit[nCntOrbit].bUse == true)
		{
			g_Orbit[nCntOrbit].nCountTime++;
			VERTEX_3D *pVtx;
			g_pVtxBuffOrbit->Lock(0, 0, (void **)&pVtx, 0);
			pVtx += nCntOrbit * 4;
			pVtx[0].col = D3DCOLOR_RGBA(100, 0, 100, 160 - g_Orbit[nCntOrbit].nCountTime * 10);
			pVtx[1].col = D3DCOLOR_RGBA(100, 0, 100, 160 - g_Orbit[nCntOrbit].nCountTime * 10);
			pVtx[2].col = D3DCOLOR_RGBA(100, 100, 100, 160 - g_Orbit[nCntOrbit].nCountTime * 10);
			pVtx[3].col = D3DCOLOR_RGBA(100, 100, 100, 160 - g_Orbit[nCntOrbit].nCountTime * 10);
			g_pVtxBuffOrbit->Unlock();
			if (g_Orbit[nCntOrbit].nCountTime >= g_Orbit[nCntOrbit].nTime)
			{
				g_Orbit[nCntOrbit].bUse = false;
			}
		}
	}
}

//描画処理
void DrawOrbit(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	int nCntOrbit;
	D3DXMATRIX mtxTrans; //計算用マトリックス
	pDevice = GetDevice();     //デバイスを取得する
	//ラインティングを無視する
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//カリングオフ
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	for (nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		if (g_Orbit[nCntOrbit].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Orbit[nCntOrbit].mtxWorld);
			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Orbit[nCntOrbit].pos.x, g_Orbit[nCntOrbit].pos.y, g_Orbit[nCntOrbit].pos.z);
			D3DXMatrixMultiply(&g_Orbit[nCntOrbit].mtxWorld, &g_Orbit[nCntOrbit].mtxWorld, &mtxTrans);
			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Orbit[nCntOrbit].mtxWorld);
			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffOrbit, 0, sizeof(VERTEX_3D));
			//頂点フォーマット
			pDevice->SetFVF(FVF_VERTEX_3D);
			pDevice->SetTexture(0, NULL);    //テクスチャの設定
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntOrbit * 4,  //開始する始点のインデックス
				2); //描画するプリミティブ数
		}
	}
	//ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//カリングオン
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void SetOrbit(D3DXVECTOR3 pos, D3DXVECTOR3 Tippos, D3DXVECTOR3 posOld, D3DXVECTOR3 TipposOld, int nTime)
{
	int nCntOrbit;
	for (nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		if (g_Orbit[nCntOrbit].bUse == false)
		{
			g_Orbit[nCntOrbit].bUse = true;
			g_Orbit[nCntOrbit].nTime = nTime;
			g_Orbit[nCntOrbit].nCountTime = 0;
			g_Orbit[nCntOrbit].pos = TipposOld;
			VERTEX_3D *pVtx;
			g_pVtxBuffOrbit->Lock(0, 0, (void **)&pVtx, 0);
			pVtx += nCntOrbit * 4;
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //切っ先古い
			pVtx[1].pos = Tippos - TipposOld; //切っ先新しい
			pVtx[2].pos = posOld - TipposOld; //持ち手側古い
			pVtx[3].pos = pos - TipposOld; //持ち手側新しい
			g_pVtxBuffOrbit->Unlock();
			break;
		}
	}
}