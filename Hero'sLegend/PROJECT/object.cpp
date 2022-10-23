//---------------------------
//Author:三上航世
//オブジェクト(object.cpp)
//---------------------------
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "gamepad.h"
#include "bullet.h"

//マクロ定義
#define MAX_OBJECT (64)

//グローバル変数
typedef struct
{
	LPD3DXMESH pMesh; //メッシュ（頂点情報）へのポインタ
	LPD3DXBUFFER pBuffMat; //マテリアル（材質情報）へのポインタ
	DWORD nNumMat; //マテリアルの数
	LPDIRECT3DTEXTURE9 TextureModel[4] = {};
	D3DXVECTOR3 vtxMinModel, vtxMaxModel; //モデルの最大値,最小値
	int nNumTex;
} Object;

typedef struct
{
	Object Object;
	int nShadow;
	int nType;
	float fRadius;
	D3DXVECTOR3 pos; //位置
	D3DXVECTOR3 rot; //向き
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	bool bUse;
} SetObject;

Object g_Object[16];
SetObject g_SetObject[MAX_OBJECT];
int g_nNumObject; //オブジェクトが何種類あるか

//初期化処理
HRESULT InitObject(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	int nCntModel = 0;
	int nCntTex;
	//Xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/armoredge/01_head.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Object[nCntModel].pBuffMat,
		NULL,
		&g_Object[nCntModel].nNumMat,
		&g_Object[nCntModel].pMesh);
	g_nNumObject = 1;
	D3DXMATERIAL *pMat;
	for (nCntModel = 0; nCntModel < g_nNumObject; nCntModel++)
	{
		nCntTex = 0;
		//マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_Object[nCntModel].pBuffMat->GetBufferPointer();
		for (int nCntMat = 0; nCntMat < (int)g_Object[nCntModel].nNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_Object[nCntModel].TextureModel[nCntTex]);
				nCntMat++;
			}
		}
		g_Object[nCntModel].nNumTex = nCntTex;
	}
	int nNumVtx; //頂点数
	DWORD sizeFVF; //頂点フォーマットのサイズ
	BYTE *pVtxBuff; //頂点バッファへのポインタ
	for (nCntModel = 0; nCntModel < g_nNumObject; nCntModel++)
	{
		//頂点数を取得
		nNumVtx = g_Object[nCntModel].pMesh->GetNumVertices();
		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_Object[nCntModel].pMesh->GetFVF());
		g_Object[nCntModel].vtxMaxModel.x = -10000.0f;
		g_Object[nCntModel].vtxMaxModel.y = -10000.0f;
		g_Object[nCntModel].vtxMaxModel.z = -10000.0f;
		g_Object[nCntModel].vtxMinModel.x = 10000.0f;
		g_Object[nCntModel].vtxMinModel.y = 10000.0f;
		g_Object[nCntModel].vtxMinModel.z = 10000.0f;
		//頂点バッファのロック
		g_Object[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標の代入
			if (g_Object[nCntModel].vtxMinModel.x > vtx.x)
			{
				g_Object[nCntModel].vtxMinModel.x = vtx.x;
			}
			if (g_Object[nCntModel].vtxMinModel.y > vtx.y)
			{
				g_Object[nCntModel].vtxMinModel.y = vtx.y;
			}
			if (g_Object[nCntModel].vtxMinModel.z > vtx.z)
			{
				g_Object[nCntModel].vtxMinModel.z = vtx.z;
			}
			if (g_Object[nCntModel].vtxMaxModel.x < vtx.x)
			{
				g_Object[nCntModel].vtxMaxModel.x = vtx.x;
			}
			if (g_Object[nCntModel].vtxMaxModel.y < vtx.y)
			{
				g_Object[nCntModel].vtxMaxModel.y = vtx.y;
			}
			if (g_Object[nCntModel].vtxMaxModel.z < vtx.z)
			{
				g_Object[nCntModel].vtxMaxModel.z = vtx.z;
			}
			pVtxBuff += sizeFVF; //頂点フォーマットのサイズ分ポインタを進める
		}
		//頂点バッファをアンロック
		g_Object[nCntModel].pMesh->UnlockVertexBuffer();
	}
	/*
	for (nCntModel = 0; nCntModel < 1; nCntModel++) //txtファイルに書いていないため、とりあえず手打ち
	{
		g_SetObject[nCntModel].nType = 0;
		g_SetObject[nCntModel].Object.pBuffMat = g_Object[g_SetObject[nCntModel].nType].pBuffMat;
		g_SetObject[nCntModel].Object.pMesh = g_Object[g_SetObject[nCntModel].nType].pMesh;
		g_SetObject[nCntModel].Object.nNumMat = g_Object[g_SetObject[nCntModel].nType].nNumMat;
		g_SetObject[nCntModel].Object.vtxMaxModel = g_Object[g_SetObject[nCntModel].nType].vtxMaxModel;
		g_SetObject[nCntModel].Object.vtxMinModel = g_Object[g_SetObject[nCntModel].nType].vtxMinModel;
		for (nCntTex = 0; nCntTex < g_Object[g_SetObject[nCntModel].nType].nNumTex; nCntTex++)
		{
			g_SetObject[nCntModel].Object.TextureModel[nCntTex] = g_Object[g_SetObject[nCntModel].nType].TextureModel[nCntTex];
		}
		g_SetObject[nCntModel].pos = D3DXVECTOR3(100.0f, 0.0f, -50.0f);
		g_SetObject[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_SetObject[nCntModel].fRadius = (g_Object[g_SetObject[nCntModel].nType].vtxMaxModel.x - g_Object[g_SetObject[nCntModel].nType].vtxMinModel.x) / 2;
		if (g_SetObject[nCntModel].fRadius < (g_Object[g_SetObject[nCntModel].nType].vtxMaxModel.z - g_Object[g_SetObject[nCntModel].nType].vtxMinModel.z) / 2)
		{
			g_SetObject[nCntModel].fRadius = (g_Object[g_SetObject[nCntModel].nType].vtxMaxModel.z - g_Object[g_SetObject[nCntModel].nType].vtxMinModel.z) / 2;
		}
		g_SetObject[nCntModel].nShadow = SetShadow(g_SetObject[nCntModel].pos, g_SetObject[nCntModel].fRadius);
		g_SetObject[nCntModel].bUse = true;
	}
	*/
	return S_OK;
}

//終了処理
void UninitObject(void)
{
	int nCntModel;
	for (nCntModel = 0; nCntModel < g_nNumObject; nCntModel++)
	{
		//テクスチャの破棄
		if (g_Object[nCntModel].pMesh != NULL)
		{
			g_Object[nCntModel].pMesh->Release();
			g_Object[nCntModel].pMesh = NULL;
		}
		//頂点バッファの破棄
		if (g_Object[nCntModel].pBuffMat != NULL)
		{
			g_Object[nCntModel].pBuffMat->Release();
			g_Object[nCntModel].pBuffMat = NULL;
		}
	}
	for (nCntModel = 0; nCntModel < MAX_OBJECT; nCntModel++)
	{
		if (g_SetObject[nCntModel].bUse == true)
		{
			g_SetObject[nCntModel].bUse = false;
		}
	}
}

//更新処理
void UpdateObject(void)
{
	//CollisionBullet(&g_SetObject[0].pos, g_SetObject[0].fRadius, g_SetObject[0].fRadius);
}

//描画処理
void DrawObject(void)
{
	LPDIRECT3DDEVICE9 pDevice;   //デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
	D3DMATERIAL9 matDef;		 //現在のマテリアル保存用
	D3DXMATERIAL *pMat;			 //マテリアルデータへのポインタ
	int nCntModel;
	pDevice = GetDevice();		 //デバイスを取得する
	for (nCntModel = 0; nCntModel < MAX_OBJECT; nCntModel++)
	{
		if (g_SetObject[nCntModel].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_SetObject[nCntModel].mtxWorld);
			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_SetObject[nCntModel].rot.y, g_SetObject[nCntModel].rot.x, g_SetObject[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_SetObject[nCntModel].mtxWorld, &g_SetObject[nCntModel].mtxWorld, &mtxRot);
			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_SetObject[nCntModel].pos.x, g_SetObject[nCntModel].pos.y, g_SetObject[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_SetObject[nCntModel].mtxWorld, &g_SetObject[nCntModel].mtxWorld, &mtxTrans);
			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_SetObject[nCntModel].mtxWorld);
			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);
			//マテリアルデータへのポインタの反映
			pMat = (D3DXMATERIAL*)g_SetObject[nCntModel].Object.pBuffMat->GetBufferPointer();
			int nCntTex = 0;
			int nCntMat;
			for (nCntMat = 0; nCntMat < (int)g_SetObject[nCntModel].Object.nNumMat; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//テクスチャの設定
					pDevice->SetTexture(0, g_SetObject[nCntModel].Object.TextureModel[nCntTex]);
					nCntMat++;
				}
				else
				{
					pDevice->SetTexture(0, NULL);
				}
				//オブジェクトの描画
				g_SetObject[nCntModel].Object.pMesh->DrawSubset(nCntMat);
			}
			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//円柱状の当たり判定
void CollisionObject(D3DXVECTOR3 *pPos, float fRadius, float fHeight)
{
	float fLengthX; //誰かとオブジェクトのX方向の長さ
	float fLengthY;
	float fLengthZ; //誰かとオブジェクトのZ方向の長さ
	bool bPlusX; //fLengthXがプラスか
	bool bPlusZ; //fLengthZがプラスか
	float fDistance; //誰かとオブジェクトの距離
	float fshort; //誰かとオブジェクトが当たっている場合の、足りない長さの倍率
	int nCntModel;
	for (nCntModel = 0; nCntModel < MAX_OBJECT; nCntModel++)
	{
		if (g_SetObject[nCntModel].bUse == true)
		{
			fLengthX = powf(pPos->x - g_SetObject[nCntModel].pos.x, 2);
			fLengthY = pPos->y + (fHeight / 2) + g_SetObject[nCntModel].pos.y;
			fLengthZ = powf(pPos->z - g_SetObject[nCntModel].pos.z, 2);
			if (pPos->x - g_SetObject[nCntModel].pos.x < 0.0f)
			{
				bPlusX = false;
			}
			else
			{
				bPlusX = true;
			}
			if (pPos->z - g_SetObject[nCntModel].pos.z < 0.0f)
			{
				bPlusZ = false;
			}
			else
			{
				bPlusZ = true;
			}
			fDistance = sqrtf(fLengthX + fLengthZ);
			if (fDistance <= fRadius + g_SetObject[nCntModel].fRadius && fLengthY <= (fHeight / 2) + g_SetObject[nCntModel].fRadius) //オブジェクトに当たったとき
			{
				fLengthX = sqrtf(fLengthX);
				fLengthZ = sqrtf(fLengthZ);
				fshort = (fRadius + g_SetObject[nCntModel].fRadius) / fDistance - 1.0f;
				fLengthX *= fshort;
				fLengthZ *= fshort;
				if (bPlusX == true)
				{
					pPos->x += fLengthX;
				}
				else
				{
					pPos->x -= fLengthX;
				}
				if (bPlusZ == true)
				{
					pPos->z += fLengthZ;
				}
				else
				{
					pPos->z -= fLengthZ;
				}
			}
		}
	}
}

void Setobject(D3DXVECTOR3 pos, int nType, D3DXVECTOR3 rot)
{
	int nCntObject;
	for (nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_SetObject[nCntObject].bUse != true)
		{
			g_SetObject[nCntObject].nType = nType;
			g_SetObject[nCntObject].Object = g_Object[nType];
			g_SetObject[nCntObject].fRadius = (g_Object[nType].vtxMaxModel.x - g_Object[nType].vtxMinModel.x) / 2;
			if (g_SetObject[nCntObject].fRadius < (g_Object[nType].vtxMaxModel.z - g_Object[nType].vtxMinModel.z) / 2)
			{
				g_SetObject[nCntObject].fRadius = (g_Object[nType].vtxMaxModel.z - g_Object[nType].vtxMinModel.z) / 2;
			}
			g_SetObject[nCntObject].pos = pos;
			g_SetObject[nCntObject].rot = rot;
			g_SetObject[nCntObject].nShadow = SetShadow(pos, g_SetObject[nCntObject].fRadius);
			g_SetObject[nCntObject].bUse = true;
			break;
		}
	}
}