//---------------------------
//Author:三上航世
//爆発処理(explosion.cpp)
//---------------------------
#include "explosion.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL; //頂点バッファへのポインタ
Explosion g_Explosion[MAX_EXPLOSION];

//初期化処理
HRESULT InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	int nCntExplosion;
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/explosion000.png",
		&g_pTextureExplosion);
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_Explosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Explosion[nCntExplosion].nTime = 0;
		g_Explosion[nCntExplosion].fTex = 0.0f;
		g_Explosion[nCntExplosion].bUse = false;
	}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);
	VERTEX_3D *pVtx; //頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-5.0f, 10.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(5.0f, 10.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-5.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(5.0f, 0.0f, 0.0f);
		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		//頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
	return S_OK;
}

//終了処理
void UninitExplosion(void)
{
	//テクスチャの破棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//更新処理
void UpdateExplosion(void)
{
	int nCntExplosion;
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_Explosion[nCntExplosion].bUse == true)
		{
			g_Explosion[nCntExplosion].nTime++;
			if (g_Explosion[nCntExplosion].nTime >= 4)
			{
				g_Explosion[nCntExplosion].nTime = 0;
				g_Explosion[nCntExplosion].fTex += 0.125f;
				if (g_Explosion[nCntExplosion].fTex == 1.0f)
				{
					g_Explosion[nCntExplosion].bUse = false;
				}
				VERTEX_3D *pVtx;
				g_pVtxBuffExplosion->Lock(0, 0, (void **)&pVtx, 0);
				pVtx += nCntExplosion * 4;
				pVtx[0].tex = D3DXVECTOR2(g_Explosion[nCntExplosion].fTex         , 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_Explosion[nCntExplosion].fTex + 0.125f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_Explosion[nCntExplosion].fTex         , 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_Explosion[nCntExplosion].fTex + 0.125f, 1.0f);
				g_pVtxBuffExplosion->Unlock();
			}
		}
	}
}

//描画処理
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxTrans; //計算用マトリックス
	int nCntExplosion;
	pDevice = GetDevice();     //デバイスを取得する
	//ラインティングを無視する
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//常に表示されるようにする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_Explosion[nCntExplosion].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Explosion[nCntExplosion].mtxWorld);
			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_Explosion[nCntExplosion].mtxWorld, NULL, &mtxView); //逆行列を求める
			g_Explosion[nCntExplosion].mtxWorld._41 = 0.0f;
			g_Explosion[nCntExplosion].mtxWorld._42 = 0.0f;
			g_Explosion[nCntExplosion].mtxWorld._43 = 0.0f;
			/*
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
			g_mtxWorldBillboard._41 = mtxView._14;
			g_mtxWorldBillboard._42 = mtxView._24;
			g_mtxWorldBillboard._43 = mtxView._34;
			g_mtxWorldBillboard._44 = mtxView._44;
			*/
			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Explosion[nCntExplosion].pos.x, g_Explosion[nCntExplosion].pos.y, g_Explosion[nCntExplosion].pos.z);
			D3DXMatrixMultiply(&g_Explosion[nCntExplosion].mtxWorld, &g_Explosion[nCntExplosion].mtxWorld, &mtxTrans);
			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Explosion[nCntExplosion].mtxWorld);
			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));
			//頂点フォーマット
			pDevice->SetFVF(FVF_VERTEX_3D);
			pDevice->SetTexture(0, g_pTextureExplosion);    //テクスチャの設定
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntExplosion * 4,  //開始する始点のインデックス
				2); //描画するプリミティブ数
		}
	}
	//ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//位置によっては映らないようにする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void SetExplosion(D3DXVECTOR3 pos)
{
	int nCntExplosion;
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_Explosion[nCntExplosion].bUse == false)
		{
			g_Explosion[nCntExplosion].pos = pos;
			g_Explosion[nCntExplosion].bUse = true;
			g_Explosion[nCntExplosion].nTime = 0;
			g_Explosion[nCntExplosion].fTex = 0.0f;
			VERTEX_3D *pVtx;
			g_pVtxBuffExplosion->Lock(0, 0, (void **)&pVtx, 0);
			pVtx += nCntExplosion * 4;
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);
			g_pVtxBuffExplosion->Unlock();
			break;
		}
	}
}