//---------------------------
//Author:三上航世
//影(shadow.cpp)
//---------------------------
#include "shadow.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL; //頂点バッファへのポインタ
Shadow g_Shadow[MAX_SHADOW];

//初期化処理
HRESULT InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	int nCntShadow;
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/shadow000.jpg",
		&g_pTextureShadow);
	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_Shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Shadow[nCntShadow].fRadius = 0.0f;
		g_Shadow[nCntShadow].bUse = false;
	}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);
	VERTEX_3D *pVtx; //頂点情報へのポインタ
					 //頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 120);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 120);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 120);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 120);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
	return S_OK;
}

//終了処理
void UninitShadow(void)
{
	//テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//更新処理
void UpdateShadow(void)
{

}

//描画処理
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
	pDevice = GetDevice();     //デバイスを取得する
	int nCntShadow;
	//減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_Shadow[nCntShadow].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Shadow[nCntShadow].mtxWorld);
			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Shadow[nCntShadow].rot.y, g_Shadow[nCntShadow].rot.x, g_Shadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_Shadow[nCntShadow].mtxWorld, &g_Shadow[nCntShadow].mtxWorld, &mtxRot);
			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Shadow[nCntShadow].pos.x, g_Shadow[nCntShadow].pos.y, g_Shadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_Shadow[nCntShadow].mtxWorld, &g_Shadow[nCntShadow].mtxWorld, &mtxTrans);
			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Shadow[nCntShadow].mtxWorld);
			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));
			//頂点フォーマット
			pDevice->SetFVF(FVF_VERTEX_3D);
			pDevice->SetTexture(0, g_pTextureShadow);    //テクスチャの設定
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntShadow * 4,  //開始する始点のインデックス
				2); //描画するプリミティブ数
			
		}
	}
	//通常合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//影の設定
int SetShadow(D3DXVECTOR3 pos, float fRadius)
{
	int nIdx = -1;
	int nCntShadow;
	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_Shadow[nCntShadow].bUse == false)
		{
			g_Shadow[nCntShadow].pos.x = pos.x;
			g_Shadow[nCntShadow].pos.y = 0.0f;
			g_Shadow[nCntShadow].pos.z = pos.z;
			g_Shadow[nCntShadow].fRadius = fRadius;
			g_Shadow[nCntShadow].bUse = true;
			g_Shadow[nCntShadow].nLength = 0;
			VERTEX_3D *pVtx;
			g_pVtxBuffShadow->Lock(0, 0, (void **)&pVtx, 0);
			pVtx += nCntShadow * 4;
			pVtx[0].pos = D3DXVECTOR3(-fRadius, 0.0f, fRadius);
			pVtx[1].pos = D3DXVECTOR3(fRadius, 0.0f, fRadius);
			pVtx[2].pos = D3DXVECTOR3(-fRadius, 0.0f, -fRadius);
			pVtx[3].pos = D3DXVECTOR3(fRadius, 0.0f,  -fRadius);
			g_pVtxBuffShadow->Unlock();
			nIdx = nCntShadow;
			break;
		}
	}
	return nIdx;
}

//影が動いたとき
void MoveShadow(int nShadow, D3DXVECTOR3 pos)
{
	int nLength;
	nLength = ceilf(pos.y);
	g_Shadow[nShadow].pos.x = pos.x;
	g_Shadow[nShadow].pos.z = pos.z;
	if (nLength >= 120)
	{
		nLength = 120;
	}
	VERTEX_3D *pVtx;
	g_pVtxBuffShadow->Lock(0, 0, (void **)&pVtx, 0);
	pVtx += nShadow * 4;
	pVtx[0].pos = D3DXVECTOR3(- g_Shadow[nShadow].fRadius - (float)nLength / 10, 0.0f, g_Shadow[nShadow].fRadius + (float)nLength / 10);
	pVtx[1].pos = D3DXVECTOR3(g_Shadow[nShadow].fRadius + (float)nLength / 10,   0.0f, g_Shadow[nShadow].fRadius + (float)nLength / 10);
	pVtx[2].pos = D3DXVECTOR3(- g_Shadow[nShadow].fRadius - (float)nLength / 10, 0.0f, -g_Shadow[nShadow].fRadius - (float)nLength / 10);
	pVtx[3].pos = D3DXVECTOR3(g_Shadow[nShadow].fRadius + (float)nLength / 10,   0.0f, -g_Shadow[nShadow].fRadius - (float)nLength / 10);
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 120 - nLength);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 120 - nLength);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 120 - nLength);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 120 - nLength);
	g_pVtxBuffShadow->Unlock();
}

void ResetShadow(void)
{
	int nCntShadow;
	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_Shadow[nCntShadow].bUse == true)
		{
			g_Shadow[nCntShadow].bUse = false;
		}
	}
}

void RemoveShadow(int nShadow)
{
	g_Shadow[nShadow].bUse = false;
}