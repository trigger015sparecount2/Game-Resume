//---------------------------
//Author:三上航世
//壁(wall.cpp)
//---------------------------
#include "wall.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureWall[2] = {}; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL; //頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffWall = NULL;
D3DXVECTOR3 g_posWall[4]; //位置
D3DXVECTOR3 g_rotWall[4]; //向き
D3DXMATRIX g_mtxWorldWall[4]; //ワールドマトリックス
int g_nwallTexType;

							  //初期化処理
HRESULT InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/wall000.jpg",
		&g_pTextureWall[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/wall.jpg",
		&g_pTextureWall[1]);
	g_nwallTexType = 0;
	g_posWall[0] = D3DXVECTOR3(0.0f, 0.0f, 750.0f);
	g_rotWall[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_posWall[1] = D3DXVECTOR3(750.0f, 0.0f, 0.0f);
	g_rotWall[1] = D3DXVECTOR3(0.0f, 1.57f, 0.0f);
	g_posWall[2] = D3DXVECTOR3(-750.0f, 0.0f, 0.0f);
	g_rotWall[2] = D3DXVECTOR3(0.0f, -1.57f, 0.0f);
	g_posWall[3] = D3DXVECTOR3(0.0f, 0.0f, -750.0f);
	g_rotWall[3] = D3DXVECTOR3(0.0f, 3.14f, 0.0f);
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 9,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);
	VERTEX_3D *pVtx; //頂点情報へのポインタ
					 //頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void **)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-750.0f, 400.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 400.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(750.0f, 400.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(-750.0f, 200.0f, 0.0f);
	pVtx[4].pos = D3DXVECTOR3(0.0f, 200.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(750.0f, 200.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(-750.0f, 0.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[8].pos = D3DXVECTOR3(750.0f, 0.0f, 0.0f);
	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 2.0f);
	pVtx[1].tex = D3DXVECTOR2(3.0f, 2.0f);
	pVtx[2].tex = D3DXVECTOR2(6.0f, 2.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[4].tex = D3DXVECTOR2(3.0f, 1.0f);
	pVtx[5].tex = D3DXVECTOR2(6.0f, 1.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[7].tex = D3DXVECTOR2(3.0f, 0.0f);
	pVtx[8].tex = D3DXVECTOR2(6.0f, 0.0f);
	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 14,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffWall,
		NULL);
	WORD *pIdx; //インデックス情報へのポインタ
				//インデックスバッファをロックし、番号データへのポインタを取得
	g_pIdxBuffWall->Lock(0, 0, (void **)&pIdx, 0);
	//番号データの設定
	pIdx[0] = 3;
	pIdx[1] = 0;
	pIdx[2] = 4;
	pIdx[3] = 1;
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;
	//インデックスバッファをアンロックする
	g_pIdxBuffWall->Unlock();
	return S_OK;
}

//終了処理
void UninitWall(void)
{
	int nCntWall;
	for (nCntWall = 0; nCntWall < 2; nCntWall++)
	{
		//テクスチャの破棄
		if (g_pTextureWall[nCntWall] != NULL)
		{
			g_pTextureWall[nCntWall]->Release();
			g_pTextureWall[nCntWall] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
	if (g_pIdxBuffWall != NULL)
	{
		g_pIdxBuffWall->Release();
		g_pIdxBuffWall = NULL;
	}
}

//更新処理
void UpdateWall(void)
{

}

//描画処理
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
	pDevice = GetDevice();     //デバイスを取得する
	int nCntWall;
	for (nCntWall = 0; nCntWall < 4; nCntWall++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldWall[nCntWall]);
		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotWall[nCntWall].y, g_rotWall[nCntWall].x, g_rotWall[nCntWall].z);
		D3DXMatrixMultiply(&g_mtxWorldWall[nCntWall], &g_mtxWorldWall[nCntWall], &mtxRot);
		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_posWall[nCntWall].x, g_posWall[nCntWall].y, g_posWall[nCntWall].z);
		D3DXMatrixMultiply(&g_mtxWorldWall[nCntWall], &g_mtxWorldWall[nCntWall], &mtxTrans);
		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldWall[nCntWall]);
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));
		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_pIdxBuffWall);
		//頂点フォーマット
		pDevice->SetFVF(FVF_VERTEX_3D);
		pDevice->SetTexture(0, g_pTextureWall[g_nwallTexType]);    //テクスチャの設定

		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			0,
			9, //頂点の数
			0,
			12); //描画するプリミティブ数
	}
}

void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumBlock[2], int nSize[2], int nWall)
{

}

void CollisionWall(D3DXVECTOR3 *pPos, float fRadius)
{
	if (pPos->x - fRadius < -750.0f)
	{
		pPos->x = -750.0f + fRadius;
	}
	else if (pPos->x + fRadius > 750.0f)
	{
		pPos->x = 750.0f - fRadius;
	}
	if (pPos->z - fRadius < -750.0f)
	{
		pPos->z = -750.0f + fRadius;
	}
	else if (pPos->z + fRadius > 750.0f)
	{
		pPos->z = 750.0f - fRadius;
	}
}

void ResetWall(void)
{
	g_nwallTexType = 1;
}