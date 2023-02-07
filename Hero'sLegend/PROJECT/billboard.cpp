//---------------------------
//Author:三上航世
//ビルボード(billboard.cpp)
//---------------------------
#include "billboard.h"
#include "enemy.h"


//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL; //頂点バッファへのポインタ
Billboard g_Billboard[MAX_BILLBOURD];

//初期化処理
HRESULT InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	int nCntBoard;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/field000.jpg",
		&g_pTextureBillboard);

	for (nCntBoard = 0; nCntBoard < MAX_BILLBOURD; nCntBoard++)
	{
		g_Billboard[nCntBoard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Billboard[nCntBoard].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	VERTEX_3D *pVtx; //頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-20.0f, 20.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(20.0f, 20.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-20.0f, -20.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(20.0f, -20.0f, 0.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

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
	g_pVtxBuffBillboard->Unlock();
	return S_OK;
}

//終了処理
void UninitBillboard(void)
{
	//テクスチャの破棄
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//更新処理
void UpdateBillboard(void)
{
	

}

//描画処理
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxTrans; //計算用マトリックス
	int nCntBoard;
	pDevice = GetDevice();     //デバイスを取得する

	//ラインティングを無視する
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//常に表示されるようにする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (nCntBoard = 0; nCntBoard < MAX_BILLBOURD; nCntBoard++)
	{
		if (g_Billboard[nCntBoard].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Billboard[nCntBoard].mtxWorld);
			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_Billboard[nCntBoard].mtxWorld, NULL, &mtxView); //逆行列を求める
			g_Billboard[nCntBoard].mtxWorld._41 = 0.0f;
			g_Billboard[nCntBoard].mtxWorld._42 = 0.0f;
			g_Billboard[nCntBoard].mtxWorld._43 = 0.0f;
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
			D3DXMatrixTranslation(&mtxTrans, g_Billboard[nCntBoard].pos.x, g_Billboard[nCntBoard].pos.y, g_Billboard[nCntBoard].pos.z);
			D3DXMatrixMultiply(&g_Billboard[nCntBoard].mtxWorld, &g_Billboard[nCntBoard].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Billboard[nCntBoard].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			//頂点フォーマット
			pDevice->SetFVF(FVF_VERTEX_3D);
			pDevice->SetTexture(0, g_pTextureBillboard);    //テクスチャの設定
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				0,  //開始する始点のインデックス
				2); //描画するプリミティブ数
		}
	}
	//ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//位置によっては映らないようにする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//ビルボードセット
void SetBillboard(D3DXVECTOR3 pos)
{
	int nCntBoard;
	for (nCntBoard = 0; nCntBoard < MAX_BILLBOURD; nCntBoard++)
	{
		//使ってない枠を使う
		if (g_Billboard[nCntBoard].bUse == false)
		{
			g_Billboard[nCntBoard].pos = pos;
			g_Billboard[nCntBoard].bUse = true;
			break;
		}
	}
}

//ビルボード何枚使ってるか
int CountBoard(void)
{
	int nCntBoard;
	int nCount = 0;
	for (nCntBoard = 0; nCntBoard < MAX_BILLBOURD; nCntBoard++)
	{
		if (g_Billboard[nCntBoard].bUse == true)
		{
			nCount++;
		}
	}
	return nCount;
}