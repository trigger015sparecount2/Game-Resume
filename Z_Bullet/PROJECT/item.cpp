//---------------------------
//Author:三上航世
//アイテム(item.cpp)
//---------------------------
#include "item.h"
#include "sound.h"
#include "player.h"

//マクロ定義
#define MAX_ITEM (4)

typedef struct
{
	D3DXVECTOR3 pos; //位置
	D3DXVECTOR3 rot;
	float fRadius;
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	bool bUse;
} Item;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureItem = NULL; //テクスチャへのポインタ
LPD3DXMESH g_pMeshItem; //メッシュ（頂点情報）へのポインタ
LPD3DXBUFFER g_pBuffMatItem; //マテリアル（材質情報）へのポインタ
DWORD g_nNumMatItem; //マテリアルの数
Item g_Item[MAX_ITEM];
float g_fRadius;

//初期化処理
HRESULT InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	int nCntItem;
	D3DXMATERIAL *pMat;
	D3DXLoadMeshFromX("data/MODEL/Aidkit_00.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatItem,
		NULL,
		&g_nNumMatItem,
		&g_pMeshItem);
	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatItem->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)g_nNumMatItem; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureItem);
		}
	}
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_Item[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Item[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Item[nCntItem].bUse = false;
	}
	int nNumVtx; //頂点数
	DWORD sizeFVF; //頂点フォーマットのサイズ
	BYTE *pVtxBuff; //頂点バッファへのポインタ
	D3DXVECTOR3 vtxMaxItem;
	D3DXVECTOR3 vtxMinItem;
	for (nCntItem = 0; nCntItem < (int)g_nNumMatItem; nCntItem++)
	{
		//頂点数を取得
		nNumVtx = g_pMeshItem->GetNumVertices();
		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_pMeshItem->GetFVF());
		vtxMaxItem.x = -10000.0f;
		vtxMaxItem.y = -10000.0f;
		vtxMaxItem.z = -10000.0f;
		vtxMinItem.x = 10000.0f;
		vtxMinItem.y = 10000.0f;
		vtxMinItem.z = 10000.0f;
		//頂点バッファのロック
		g_pMeshItem->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標の代入
			if (vtxMinItem.x > vtx.x)
			{
				vtxMinItem.x = vtx.x;
			}
			if (vtxMinItem.y > vtx.y)
			{
				vtxMinItem.y = vtx.y;
			}
			if (vtxMinItem.z > vtx.z)
			{
				vtxMinItem.z = vtx.z;
			}
			if (vtxMaxItem.x < vtx.x)
			{
				vtxMaxItem.x = vtx.x;
			}
			if (vtxMaxItem.y < vtx.y)
			{
				vtxMaxItem.y = vtx.y;
			}
			if (vtxMaxItem.z < vtx.z)
			{
				vtxMaxItem.z = vtx.z;
			}
			pVtxBuff += sizeFVF; //頂点フォーマットのサイズ分ポインタを進める
		}
		//頂点バッファをアンロック
		g_pMeshItem->UnlockVertexBuffer();
	}

	g_fRadius = (vtxMaxItem.x - vtxMinItem.x) / 2;
	if (g_fRadius < (vtxMaxItem.z - vtxMinItem.z) / 2)
	{
		g_fRadius = (vtxMaxItem.z - vtxMinItem.z) / 2;
	}
	return S_OK;
}

//終了処理
void UninitItem(void)
{
	//テクスチャの破棄
	if (g_pMeshItem != NULL)
	{
		g_pMeshItem->Release();
		g_pMeshItem = NULL;
	}
	//頂点バッファの破棄
	if (g_pBuffMatItem != NULL)
	{
		g_pBuffMatItem->Release();
		g_pBuffMatItem = NULL;
	}
}

//更新処理
void UpdateItem(void)
{
	int nCntItem;
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[nCntItem].bUse == true)
		{
			g_Item[nCntItem].rot.y += 0.02f;
			if (g_Item[nCntItem].rot.y > D3DX_PI)
			{
				g_Item[nCntItem].rot.y -= D3DX_PI * 2;
			}
		}
	}
}

//描画処理
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;   //デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
	D3DMATERIAL9 matDef;		 //現在のマテリアル保存用
	D3DXMATERIAL *pMat;			 //マテリアルデータへのポインタ
	int nCntItem;
	pDevice = GetDevice();		 //デバイスを取得する
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[nCntItem].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Item[nCntItem].mtxWorld);
			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Item[nCntItem].rot.y, g_Item[nCntItem].rot.x, g_Item[nCntItem].rot.z);
			D3DXMatrixMultiply(&g_Item[nCntItem].mtxWorld, &g_Item[nCntItem].mtxWorld, &mtxRot);
			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y, g_Item[nCntItem].pos.z);
			D3DXMatrixMultiply(&g_Item[nCntItem].mtxWorld, &g_Item[nCntItem].mtxWorld, &mtxTrans);
			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Item[nCntItem].mtxWorld);
			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);
			//マテリアルデータへのポインタの反映
			pMat = (D3DXMATERIAL*)g_pBuffMatItem->GetBufferPointer();
			int nCntMat;
			for (nCntMat = 0; nCntMat < (int)g_nNumMatItem; nCntMat++)
			{
				//プレイヤーのマテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//テクスチャの設定
					pDevice->SetTexture(0, g_pTextureItem);
				}
				else
				{
					pDevice->SetTexture(0, NULL);
				}
				//プレイヤーのモデル（パーツ）の描画
				g_pMeshItem->DrawSubset(nCntMat);
			}
			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

void SetItem(D3DXVECTOR3 pos)
{
	int nCntItem;
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[nCntItem].bUse == false)
		{
			g_Item[nCntItem].fRadius = g_fRadius;
			g_Item[nCntItem].pos = pos;
			g_Item[nCntItem].bUse = true;
			break;
		}
	}
}

void CollisionItem(D3DXVECTOR3 pos, float fRadius)
{
	int nCntItem;
	float fLengthX; //アイテムとプレイヤーのX方向の長さ
	float fLengthZ; //アイテムとプレイヤーのZ方向の長さ
	float fDistance; //アイテムとプレイヤーの距離
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[nCntItem].bUse == true)
		{
			fLengthX = powf(pos.x - g_Item[nCntItem].pos.x, 2);
			fLengthZ = powf(pos.z - g_Item[nCntItem].pos.z, 2);
			fDistance = sqrtf(fLengthX + fLengthZ);
			if (fDistance <= fRadius + g_Item[nCntItem].fRadius)
			{
				AddItem();
				PlaySound(SOUND_LABEL_ITEM);
				g_Item[nCntItem].bUse = false;
			}
		}
	}
}

void ResetItem(void)
{
	int nCntItem;
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_Item[nCntItem].bUse = false;
	}
}