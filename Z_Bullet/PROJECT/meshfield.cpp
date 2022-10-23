//-----------------------------------------
//Author:三上航世
//メッシュフィールド(meshfield.cpp)
//-----------------------------------------
#include "meshfield.h"

//マクロ定義
#define MAX_WIDTH (30) //x方向の正方形の数
#define MAX_DEPTH (30) //z方向の正方形の数

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureMeshfield[2] = {}; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield = NULL; //頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;
int g_nWidth, g_nDepth, g_nSize[2];
int g_nfieldTexType;
D3DXVECTOR3 g_posMeshfield; //位置
D3DXVECTOR3 g_rotMeshfield; //向き
D3DXMATRIX g_mtxWorldMeshfield; //ワールドマトリックス

//初期化処理
HRESULT InitMeshfield(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	g_nWidth = 1;
	g_nDepth = 1;
	g_nSize[0] = 50;
	g_nSize[1] = 50;
	g_nfieldTexType = 0;
	int nWidth; //x方向
	int nDepth; //z方向
	int nBlock = 0;
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/field000.jpg",
		&g_pTextureMeshfield[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/field.jpg",
		&g_pTextureMeshfield[1]);
	g_posMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (g_nWidth + 1) * (g_nDepth + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield,
		NULL);
	VERTEX_3D *pVtx; //頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMeshfield->Lock(0, 0, (void **)&pVtx, 0);
	for (nDepth = 0; nDepth < g_nDepth + 1; nDepth++)
	{
		for (nWidth = 0; nWidth < g_nWidth + 1; nWidth++)
		{
			//頂点座標の設定
			pVtx[nBlock].pos = D3DXVECTOR3(nWidth * g_nSize[0] - (g_nWidth / 2) * g_nSize[0], 0.0f, -nDepth * g_nSize[1] + (g_nDepth / 2) * g_nSize[1]); //左奥から順番に配置される
			//法線ベクトルの設定
			pVtx[nBlock].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//頂点カラー
			pVtx[nBlock].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			//テクスチャ座標
			pVtx[nBlock].tex = D3DXVECTOR2(nWidth * 1.0f, (g_nDepth - nDepth) * 1.0f);
			nBlock++;
		}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffMeshfield->Unlock();
	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (2 * (g_nWidth + 1) * g_nDepth + 2 * (g_nDepth - 1)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);
	nBlock = 0;
	WORD *pIdx; //インデックス情報へのポインタ
	//インデックスバッファをロックし、番号データへのポインタを取得
	g_pIdxBuffMeshField->Lock(0, 0, (void **)&pIdx, 0);
	for (nDepth = 0; nDepth < g_nDepth; nDepth++)
	{
		for (nWidth = 0; nWidth < g_nWidth + 1; nWidth++)
		{
			//番号データの設定
			pIdx[nBlock] = nWidth + (nDepth + 1) * (g_nWidth + 1);
			pIdx[nBlock + 1] = nWidth + nDepth * (g_nWidth + 1);
			nBlock += 2;
		}
		if (nDepth < g_nDepth - 1)
		{
			pIdx[nBlock] = g_nWidth + nDepth * (g_nWidth + 1);
			pIdx[nBlock + 1] = (nDepth + 2) * (g_nWidth + 1);
			nBlock += 2;
		}
	}
	//インデックスバッファをアンロックする
	g_pIdxBuffMeshField->Unlock();
	return S_OK;
}

//終了処理
void UninitMeshfield(void)
{
	int nCntField;
	for (nCntField = 0; nCntField < 2; nCntField++)
	{
		//テクスチャの破棄
		if (g_pTextureMeshfield[nCntField] != NULL)
		{
			g_pTextureMeshfield[nCntField]->Release();
			g_pTextureMeshfield[nCntField] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffMeshfield != NULL)
	{
		g_pVtxBuffMeshfield->Release();
		g_pVtxBuffMeshfield = NULL;
	}
	//インデックスバッファの破棄
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//更新処理
void UpdateMeshfield(void)
{

}

//描画処理
void DrawMeshfield(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
	pDevice = GetDevice();     //デバイスを取得する
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshfield);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshfield.y, g_rotMeshfield.x, g_rotMeshfield.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshfield);
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));
	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshField);
	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->SetTexture(0, g_pTextureMeshfield[g_nfieldTexType]);    //テクスチャの設定
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		(g_nWidth + 1) * (g_nDepth + 1), //頂点の数
		0,
		2 * (g_nWidth + 1) * g_nDepth + 2 * (g_nDepth - 2)); //描画するプリミティブ数
}

void SetField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumBlock[2], int nSize[2])
{
	//頂点バッファの破棄
	if (g_pVtxBuffMeshfield != NULL)
	{
		g_pVtxBuffMeshfield->Release();
		g_pVtxBuffMeshfield = NULL;
	}
	//インデックスバッファの破棄
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
	g_posMeshfield = pos;
	g_rotMeshfield = rot;
	g_nWidth = nNumBlock[0];
	g_nDepth = nNumBlock[1];
	g_nSize[0] = nSize[0];
	g_nSize[1] = nSize[1];
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	int nWidth; //x方向
	int nDepth; //z方向
	int nBlock = 0;
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/field000.jpg",
		&g_pTextureMeshfield[0]);
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (g_nWidth + 1) * (g_nDepth + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield,
		NULL);
	VERTEX_3D *pVtx; //頂点情報へのポインタ
					 //頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMeshfield->Lock(0, 0, (void **)&pVtx, 0);
	for (nDepth = 0; nDepth < g_nDepth + 1; nDepth++)
	{
		for (nWidth = 0; nWidth < g_nWidth + 1; nWidth++)
		{
			//頂点座標の設定
			pVtx[nBlock].pos = D3DXVECTOR3(nWidth * g_nSize[0] - (g_nWidth / 2) * g_nSize[0], 0.0f, -nDepth * g_nSize[1] + (g_nDepth / 2) * g_nSize[1]); //左奥から順番に配置される
																																						 //法線ベクトルの設定
			pVtx[nBlock].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//頂点カラー
			pVtx[nBlock].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			//テクスチャ座標
			pVtx[nBlock].tex = D3DXVECTOR2(nWidth * 1.0f, (g_nDepth - nDepth) * 1.0f);
			nBlock++;
		}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffMeshfield->Unlock();
	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (2 * (g_nWidth + 1) * g_nDepth + 2 * (g_nDepth - 1)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);
	nBlock = 0;
	WORD *pIdx; //インデックス情報へのポインタ
				//インデックスバッファをロックし、番号データへのポインタを取得
	g_pIdxBuffMeshField->Lock(0, 0, (void **)&pIdx, 0);
	for (nDepth = 0; nDepth < g_nDepth; nDepth++)
	{
		for (nWidth = 0; nWidth < g_nWidth + 1; nWidth++)
		{
			//番号データの設定
			pIdx[nBlock] = nWidth + (nDepth + 1) * (g_nWidth + 1);
			pIdx[nBlock + 1] = nWidth + nDepth * (g_nWidth + 1);
			nBlock += 2;
		}
		if (nDepth < g_nDepth - 1)
		{
			pIdx[nBlock] = g_nWidth + nDepth * (g_nWidth + 1);
			pIdx[nBlock + 1] = (nDepth + 2) * (g_nWidth + 1);
			nBlock += 2;
		}
	}
	//インデックスバッファをアンロックする
	g_pIdxBuffMeshField->Unlock();
}

//フィールドとの当たり判定
bool CollisionField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadius)
{
	bool bLand = false;
	if (pPos->x - fRadius < g_posMeshfield.x + (g_nWidth * g_nSize[0] / 2) && g_posMeshfield.x - (g_nWidth * g_nSize[0] / 2) < pPos->x + fRadius &&
		pPos->z + fRadius > g_posMeshfield.z - (g_nDepth * g_nSize[1] / 2) && g_posMeshfield.z + (g_nDepth * g_nSize[1] / 2) > pPos->z - fRadius)
	{
		//フィールドの上側にぶつかった(乗った)とき
		if (pPosOld->y >= g_posMeshfield.y && g_posMeshfield.y >= pPos->y)
		{
			bLand = true;
			pMove->y = 0.0f;
			pPos->y = g_posMeshfield.y;
		}
	}
	return bLand;
}

void ResetField(void)
{
	g_nfieldTexType = 1;
}