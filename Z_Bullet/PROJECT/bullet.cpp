//--------------------------
//Author:三上航世
//弾丸の処理(bullet.cpp)
//--------------------------
#include "bullet.h"
#include "enemy.h"
#include "player.h"

//マクロ定義
#define MAX_BULLET (32)   //弾の最大数

//弾の構造体定義
typedef struct
{
	D3DXVECTOR3 pos;  //位置
	D3DXVECTOR3 posold; //過去の位置
	D3DXVECTOR3 move; //移動量
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	int nLife;
	int nDamage;
	int nDamageTime;
	bool bUse;        //使用しているかどうか
} Bullet;
//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL; //頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];

//初期化処理
HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;
	pDevice = GetDevice();
	//弾の初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].nDamage = 40;
		g_aBullet[nCntBullet].bUse = false;
		g_aBullet[nCntBullet].nDamageTime = 0;
	}
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/bullet000.png",
		&g_pTextureBullet);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);
	VERTEX_3D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void **)&pVtx, 0);
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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
	return S_OK;
}

//弾の終了処理
void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//弾の更新処理
void UpdateBullet(void)
{
	int nCntBullet;
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			g_aBullet[nCntBullet].posold = g_aBullet[nCntBullet].pos;
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;
			if (CollisionDamage(g_aBullet[nCntBullet].pos, 5.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_aBullet[nCntBullet].nDamage, g_aBullet[nCntBullet].nDamageTime) == true)
			{
				g_aBullet[nCntBullet].bUse = false;
			}
			g_aBullet[nCntBullet].nLife--;
			if (g_aBullet[nCntBullet].nLife <= 0)
			{
				g_aBullet[nCntBullet].bUse = false;
			}
		}
	}
}

//描画処理
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxTrans; //計算用マトリックス
	int nCntBullet;
	pDevice = GetDevice();     //デバイスを取得する
							   //ラインティングを無視する
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//透明な部分を描画しないようにする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);
			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aBullet[nCntBullet].mtxWorld, NULL, &mtxView); //逆行列を求める
			g_aBullet[nCntBullet].mtxWorld._41 = 0.0f;
			g_aBullet[nCntBullet].mtxWorld._42 = 0.0f;
			g_aBullet[nCntBullet].mtxWorld._43 = 0.0f;
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
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);
			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);
			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));
			//頂点フォーマット
			pDevice->SetFVF(FVF_VERTEX_3D);
			pDevice->SetTexture(0, g_pTextureBullet);    //テクスチャの設定
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				0,  //開始する始点のインデックス
				2); //描画するプリミティブ数
		}
	}
	//ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//デフォルトに戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
}

//弾の設定処理
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nDamage, int nDamageTime)
{
	int nCntBullet;
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].nLife = 100;
			g_aBullet[nCntBullet].nDamage = nDamage;
			g_aBullet[nCntBullet].nDamageTime = nDamageTime;
			g_aBullet[nCntBullet].bUse = true;
			break;
		}
	}
}

int CountBullet(void)
{
	int nCntBullet;
	int nBullet = 0;
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			nBullet++;
		}
	}
	return nBullet;
}

bool CollisionBullet(D3DXVECTOR3 pos, float fRadius)
{
	float fLengthX; //弾丸と何かとのX方向の長さ
	float fLengthY; //弾丸と何かとのY方向の長さ
	float fLengthZ; //弾丸と何かとのZ方向の長さ
	float fHDistance; //弾丸と何かとの水平方向の長さ
	float fDistance; //弾丸と何かとの距離
	int nCntBullet;
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			fLengthX = powf(pos.x - g_aBullet[nCntBullet].pos.x, 2);
			fLengthY = powf(pos.y - g_aBullet[nCntBullet].pos.y + 5.0f, 2);
			fLengthZ = powf(pos.z - g_aBullet[nCntBullet].pos.z, 2);
			fHDistance = fLengthX + fLengthZ;
			fDistance = sqrtf(fLengthY + fHDistance);
			if (fDistance <= fRadius + 5.0f) //オブジェクトか敵に当たったとき
			{
				g_aBullet[nCntBullet].bUse = false;
				return true;
			}
		}
	}
	return false;
}

void ResetBullet(void)
{
	int nCntBullet;
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].bUse = false;
	}
}