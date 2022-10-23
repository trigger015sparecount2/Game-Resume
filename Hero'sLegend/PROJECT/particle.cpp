//---------------------------
//Author:三上航世
//パーティクル処理(particle.cpp)
//---------------------------
#include "particle.h"
#include "camera.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureParticle = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL; //頂点バッファへのポインタ
Particle g_Particle[MAX_PARTICLE];

//初期化処理
HRESULT InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	int nCntParticle;
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/bullet000.png",
		&g_pTextureParticle);
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_Particle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Particle[nCntParticle].nTime = 0;
		g_Particle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Particle[nCntParticle].bUse = false;
	}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);
	VERTEX_3D *pVtx; //頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-5.0f, 5.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(5.0f, 5.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-5.0f, -5.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(5.0f, -5.0f, 0.0f);
		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		//頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffParticle->Unlock();
	return S_OK;
}

//終了処理
void UninitParticle(void)
{
	//テクスチャの破棄
	if (g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//更新処理
void UpdateParticle(void)
{
	int nCntParticle;
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_Particle[nCntParticle].bUse == true)
		{
			g_Particle[nCntParticle].nTime++;
			g_Particle[nCntParticle].pos += g_Particle[nCntParticle].move;
			
			if (g_Particle[nCntParticle].nTime >= 30)
			{
				g_Particle[nCntParticle].bUse = false;
			}
		}
	}
}

//描画処理
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxTrans; //計算用マトリックス
	int nCntParticle;
	pDevice = GetDevice();     //デバイスを取得する
	//ラインティングを無視する
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_Particle[nCntParticle].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Particle[nCntParticle].mtxWorld);
			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_Particle[nCntParticle].mtxWorld, NULL, &mtxView); //逆行列を求める
			g_Particle[nCntParticle].mtxWorld._41 = 0.0f;
			g_Particle[nCntParticle].mtxWorld._42 = 0.0f;
			g_Particle[nCntParticle].mtxWorld._43 = 0.0f;
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
			D3DXMatrixTranslation(&mtxTrans, g_Particle[nCntParticle].pos.x, g_Particle[nCntParticle].pos.y, g_Particle[nCntParticle].pos.z);
			D3DXMatrixMultiply(&g_Particle[nCntParticle].mtxWorld, &g_Particle[nCntParticle].mtxWorld, &mtxTrans);
			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Particle[nCntParticle].mtxWorld);
			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_3D));
			//頂点フォーマット
			pDevice->SetFVF(FVF_VERTEX_3D);
			pDevice->SetTexture(0, g_pTextureParticle);    //テクスチャの設定
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntParticle * 4,  //開始する始点のインデックス
				2); //描画するプリミティブ数
		}
	}
	//ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//パーティクルの設定(もっといいやり方ありそう)
void SetParticle(D3DXVECTOR3 pos)
{
	int nCntParticle;
	int nCntPoint = 0;
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_Particle[nCntParticle].bUse == false)
		{
			g_Particle[nCntParticle].pos = pos;
			g_Particle[nCntParticle].nTime = 0;
			g_Particle[nCntParticle].bUse = true;
			switch (nCntPoint)
			{
			case 0:
				g_Particle[nCntParticle].move = D3DXVECTOR3(0.0f, 2.5f, 0.0f);
				break;
			case 1:
				g_Particle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * 0.5f + CameraRot()) * 2.0f, cosf(D3DX_PI * 0.25f) * 2.0f, sinf(-CameraRot()) * 2.0f);
				break;
			case 2:
				g_Particle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * 0.5f + CameraRot()) * 2.5f, cosf(D3DX_PI * 0.5f) * 2.0f, sinf(-CameraRot()) * 2.5f);
				break;
			case 3:
				g_Particle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * 0.5f + CameraRot()) * 2.0f, cosf(D3DX_PI * 0.75f) * 2.0f, sinf(-CameraRot()) * 2.0f);
				break;
			case 4:
				g_Particle[nCntParticle].move = D3DXVECTOR3(0.0f, -2.5f, 0.0f);
				break;
			case 5:
				g_Particle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * -0.5f + CameraRot()) * 2.0f, cosf(D3DX_PI * -0.75f) * 2.0f, sinf(CameraRot()) * 2.0f);
				break;
			case 6:
				g_Particle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * -0.5f + CameraRot()) * 2.5f, cosf(D3DX_PI * -0.5f) * 2.0f, sinf(CameraRot()) * 2.5f);
				break;
			case 7:
				g_Particle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * -0.5f + CameraRot()) * 2.0f, cosf(D3DX_PI * -0.25f) * 2.0f, sinf(CameraRot()) * 2.0f);
				break;
			}
			nCntPoint++;
		}
		if (nCntPoint == 8)
		{
			break;
		}
	}
}