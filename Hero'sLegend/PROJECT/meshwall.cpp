//-----------------------------------------
//Author:三上航世
//メッシュウォール(meshwall.cpp)
//-----------------------------------------
#include "meshwall.h"

//マクロ定義
#define MAX_WIDTH (8) //x方向の正方形の数
#define MAX_HEIGHT (1) //y方向の正方形の数
#define MAX_DEPTH (8) //z方向の正方形の数

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureMeshwall = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshwall = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshwall = NULL;	//インデックスバッファのポインタ
D3DXVECTOR3 g_posMeshwall;		//位置
D3DXVECTOR3 g_rotMeshwall;		//向き
D3DXMATRIX g_mtxWorldMeshwall;	//ワールドマトリックス

//初期化処理
HRESULT InitMeshwall(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	int nWidth; //x方向
	int nHeight; //y方向
	int nDepth; //z方向
	int nBlock = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/field000.jpg",
		&g_pTextureMeshwall);

	g_posMeshwall = D3DXVECTOR3(-100.0f, 0.0f, 100.0f);
	g_rotMeshwall = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (MAX_WIDTH + MAX_DEPTH) * 2 * (MAX_HEIGHT + 1) + MAX_HEIGHT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshwall,
		NULL);

	VERTEX_3D *pVtx; //頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMeshwall->Lock(0, 0, (void **)&pVtx, 0);
	for (nHeight = 0; nHeight < MAX_HEIGHT + 1; nHeight++)
	{
		for (nWidth = 0; nWidth < MAX_WIDTH; nWidth++)
		{
			//頂点座標の設定
			pVtx[nBlock].pos = D3DXVECTOR3(nWidth * 50.0f, (MAX_HEIGHT - nHeight) * 50.0f, 0.0f); //正面
			//法線ベクトルの設定
			pVtx[nBlock].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//頂点カラー
			pVtx[nBlock].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			//テクスチャ座標
			pVtx[nBlock].tex = D3DXVECTOR2(nWidth * 1.0f, (MAX_HEIGHT - nHeight) * 1.0f);
			nBlock++;
		}
		for (nDepth = 0; nDepth < MAX_DEPTH; nDepth++)
		{
			//頂点座標の設定
			pVtx[nBlock].pos = D3DXVECTOR3(MAX_WIDTH * 50.0f, (MAX_HEIGHT - nHeight) * 50.0f, -nDepth * 50.0f); //右側
			//法線ベクトルの設定
			pVtx[nBlock].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//頂点カラー
			pVtx[nBlock].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			//テクスチャ座標
			pVtx[nBlock].tex = D3DXVECTOR2(MAX_WIDTH * 1.0f + nDepth * 1.0f, (MAX_HEIGHT - nHeight) * 1.0f);
			nBlock++;
		}
		for (nWidth = 0; nWidth < MAX_WIDTH; nWidth++)
		{
			//頂点座標の設定
			pVtx[nBlock].pos = D3DXVECTOR3((MAX_WIDTH - nWidth) * 50.0f, (MAX_HEIGHT - nHeight) * 50.0f, -MAX_DEPTH * 50.0f); //後ろ
			//法線ベクトルの設定
			pVtx[nBlock].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//頂点カラー
			pVtx[nBlock].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			//テクスチャ座標
			pVtx[nBlock].tex = D3DXVECTOR2(MAX_WIDTH * 1.0f + MAX_DEPTH * 1.0f + nWidth * 1.0f, (MAX_HEIGHT - nHeight) * 1.0f);
			nBlock++;
		}
		for (nDepth = 0; nDepth < MAX_DEPTH; nDepth++)
		{
			//頂点座標の設定
			pVtx[nBlock].pos = D3DXVECTOR3(0.0f, (MAX_HEIGHT - nDepth) * 50.0f, -(MAX_DEPTH - nDepth) * 50.0f); //左側
			//法線ベクトルの設定
			pVtx[nBlock].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//頂点カラー
			pVtx[nBlock].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			//テクスチャ座標
			pVtx[nBlock].tex = D3DXVECTOR2(MAX_WIDTH * 2.0f + MAX_DEPTH * 1.0f + nDepth * 1.0f, (MAX_HEIGHT - nHeight) * 1.0f);
			nBlock++;
		}
		//頂点座標の設定
		pVtx[nBlock].pos = D3DXVECTOR3(0.0f, (MAX_HEIGHT - nHeight) * 50.0f, 0.0f); //左斜め前の角
		//法線ベクトルの設定
		pVtx[nBlock].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//頂点カラー
		pVtx[nBlock].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//テクスチャ座標
		pVtx[nBlock].tex = D3DXVECTOR2(MAX_WIDTH * 2.0f + MAX_DEPTH * 2.0f + 1.0f, (MAX_HEIGHT - nHeight) * 1.0f);
		nBlock++;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffMeshwall->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (2 * (MAX_WIDTH * 2 + MAX_DEPTH * 2 + 1) * MAX_HEIGHT + 2 * (MAX_HEIGHT - 1)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshwall,
		NULL);
	nBlock = 0;
	WORD *pIdx; //インデックス情報へのポインタ
	//インデックスバッファをロックし、番号データへのポインタを取得
	g_pIdxBuffMeshwall->Lock(0, 0, (void **)&pIdx, 0);
	for (nHeight = 0; nHeight < MAX_HEIGHT; nHeight++)
	{
		for (nWidth = 0; nWidth < MAX_WIDTH * 2 + MAX_DEPTH * 2 + 1; nWidth++)
		{
			//番号データの設定
			pIdx[nBlock] = nWidth + (nHeight + 1) * (MAX_WIDTH * 2 + MAX_DEPTH * 2);
			pIdx[nBlock + 1] = nWidth + nHeight * (MAX_WIDTH * 2 + MAX_DEPTH * 2);
			nBlock += 2;
		}
		if (nHeight < MAX_HEIGHT - 1)
		{
			pIdx[nBlock] = (nHeight + 1) * (MAX_WIDTH * 2 + MAX_DEPTH * 2) + nHeight;
			pIdx[nBlock + 1] = (nHeight + 2) * (MAX_WIDTH * 2 + MAX_DEPTH * 2) + nHeight + 2;
			nBlock += 2;
		}
	}
	//インデックスバッファをアンロックする
	g_pIdxBuffMeshwall->Unlock();
	return S_OK;
}

//終了処理
void UninitMeshwall(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshwall != NULL)
	{
		g_pTextureMeshwall->Release();
		g_pTextureMeshwall = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffMeshwall != NULL)
	{
		g_pVtxBuffMeshwall->Release();
		g_pVtxBuffMeshwall = NULL;
	}
	//インデックスバッファの破棄
	if (g_pIdxBuffMeshwall != NULL)
	{
		g_pIdxBuffMeshwall->Release();
		g_pIdxBuffMeshwall = NULL;
	}
}

//更新処理
void UpdateMeshwall(void)
{

}

//描画処理
void DrawMeshwall(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
	pDevice = GetDevice();     //デバイスを取得する
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshwall);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshwall.y, g_rotMeshwall.x, g_rotMeshwall.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshwall, &g_mtxWorldMeshwall, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshwall.x, g_posMeshwall.y, g_posMeshwall.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshwall, &g_mtxWorldMeshwall, &mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshwall);
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshwall, 0, sizeof(VERTEX_3D));
	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshwall);
	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->SetTexture(0, g_pTextureMeshwall);    //テクスチャの設定
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		(MAX_WIDTH + MAX_DEPTH) * 2 * (MAX_HEIGHT + 1) + 2 * MAX_HEIGHT, //頂点の数
		0,
		2 * (MAX_WIDTH * 2 + MAX_DEPTH * 2 + 1) * MAX_HEIGHT + 2 * (MAX_HEIGHT - 2)); //描画するプリミティブ数
}