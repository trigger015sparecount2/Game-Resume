//---------------------------
//Author:三上航世
//ポリゴン(polygon.cpp)
//---------------------------
#include "polygon.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePolygon = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL; //頂点バッファへのポインタ
D3DXVECTOR3 g_posPolygon; //位置
D3DXVECTOR3 g_rotPolygon; //向き
D3DXMATRIX g_mtxWorldPolygon; //ワールドマトリックス

//初期化処理
HRESULT InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/field000.jpg",
		&g_pTexturePolygon);

	g_posPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);

	VERTEX_3D *pVtx; //頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void **)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_posPolygon.x - 100.0f, 0.0f, g_posPolygon.z + 100.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posPolygon.x + 100.0f, 0.0f, g_posPolygon.z + 100.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posPolygon.x - 100.0f, 0.0f, g_posPolygon.z - 100.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posPolygon.x + 100.0f, 0.0f, g_posPolygon.z - 100.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
	//頂点バッファをアンロックする
	g_pVtxBuffPolygon->Unlock();
	return S_OK;
}

//終了処理
void UninitPolygon(void)
{
	//テクスチャの破棄
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//更新処理
void UpdatePolygon(void)
{

}

//描画処理
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
	pDevice = GetDevice();     //デバイスを取得する
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldPolygon);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotPolygon.y, g_rotPolygon.x, g_rotPolygon.z);
	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posPolygon.x, g_posPolygon.y, g_posPolygon.z);
	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPolygon);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetTexture(0, g_pTexturePolygon);    //テクスチャの設定

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,  //開始する始点のインデックス
		2); //描画するプリミティブ数
}