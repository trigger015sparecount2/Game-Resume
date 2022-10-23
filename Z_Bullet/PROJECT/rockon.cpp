//---------------------------
//Author:三上航世
//ロックオン(rockon.cpp)
//---------------------------
#include "rockon.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureRockon = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRockon = NULL; //頂点バッファへのポインタ
Rockon g_Rockon;

//初期化処理
HRESULT InitRockon(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
							   //テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/target000.png",
		&g_pTextureRockon);
	g_Rockon.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Rockon.nNumber = 0;
	g_Rockon.bUse = false;
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRockon,
		NULL);
	VERTEX_3D *pVtx; //頂点情報へのポインタ
					 //頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffRockon->Lock(0, 0, (void **)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-15.0f, 15.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(15.0f, 15.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-15.0f, -15.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(15.0f, -15.0f, 0.0f);
	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[1].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[2].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	//頂点バッファをアンロックする
	g_pVtxBuffRockon->Unlock();
	return S_OK;
}

//終了処理
void UninitRockon(void)
{
	//テクスチャの破棄
	if (g_pTextureRockon != NULL)
	{
		g_pTextureRockon->Release();
		g_pTextureRockon = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffRockon != NULL)
	{
		g_pVtxBuffRockon->Release();
		g_pVtxBuffRockon = NULL;
	}
}

//更新処理
void UpdateRockon(void)
{

}

//描画処理
void DrawRockon(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxTrans; //計算用マトリックス
	if (g_Rockon.bUse == true)
	{
		pDevice = GetDevice();     //デバイスを取得する
		//ラインティングを無視する
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		//常に表示されるようにする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Rockon.mtxWorld);
		//ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);
		//ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&g_Rockon.mtxWorld, NULL, &mtxView); //逆行列を求める
		g_Rockon.mtxWorld._41 = 0.0f;
		g_Rockon.mtxWorld._42 = 0.0f;
		g_Rockon.mtxWorld._43 = 0.0f;
		/*										左辺のこと
		g_mtxWorldBillboard._11 = mtxView._11;
		g_mtxWorldBillboard._12 = mtxView._21;
		g_mtxWorldBillboard._13 = mtxView._31;
		g_mtxWorldBillboard._14 = mtxView._41;
		g_mtxWorldBillboard._21 = mtxView._12;
		g_mtxWorldBillboard._22 = mtxView._22;
		g_mtxWorldBillboard._23 = mtxView._32;
		g_mtxWorldBillboard._24 = mtxView._42;
		g_mtxWorldBillboard._31 = mtxView._13;
		g_mtxWorldBillboard._32 = mtxView._23;
		g_mtxWorldBillboard._33 = mtxView._33;
		g_mtxWorldBillboard._34 = mtxView._43;
		g_mtxWorldBillboard._41 = mtxView._14;	xのPos
		g_mtxWorldBillboard._42 = mtxView._24;	yのPos
		g_mtxWorldBillboard._43 = mtxView._34;	zのPos
		g_mtxWorldBillboard._44 = mtxView._44;

		*/
		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Rockon.pos.x, g_Rockon.pos.y, g_Rockon.pos.z);
		D3DXMatrixMultiply(&g_Rockon.mtxWorld, &g_Rockon.mtxWorld, &mtxTrans);
		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Rockon.mtxWorld);
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffRockon, 0, sizeof(VERTEX_3D));
		//頂点フォーマット
		pDevice->SetFVF(FVF_VERTEX_3D);
		pDevice->SetTexture(0, g_pTextureRockon);    //テクスチャの設定
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,  //開始する始点のインデックス
			2); //描画するプリミティブ数
		//ラインティングを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		//位置によっては映らないようにする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

void SetRockon(D3DXVECTOR3 pos, int nNumber)
{
	if (nNumber != g_Rockon.nNumber)
	{
		g_Rockon.pos = pos;
		g_Rockon.nNumber = nNumber;
		g_Rockon.bUse = true;
	}
}

void MoveRockon(D3DXVECTOR3 pos)
{
	g_Rockon.pos = pos;
}

int RemoveRockon(void)
{
	g_Rockon.nNumber = -1;
	g_Rockon.bUse = false;
	return -1;
}

int RockNumber(void)
{
	return g_Rockon.nNumber;
}