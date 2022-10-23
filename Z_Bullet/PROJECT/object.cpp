//---------------------------
//Author:三上航世
//オブジェクト(object.cpp)
//---------------------------
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "gamepad.h"
#include "bullet.h"
#include "light.h"
#include "meshfield.h"

//マクロ定義
#define MAX_OBJECT (32)

typedef struct
{
	LPD3DXMESH pMesh; //メッシュ（頂点情報）へのポインタ
	LPD3DXBUFFER pBuffMat; //マテリアル（材質情報）へのポインタ
	DWORD nNumMat; //マテリアルの数
	LPDIRECT3DTEXTURE9 TextureModel[8] = {};
	D3DXVECTOR3 vtxMinModel, vtxMaxModel; //モデルの最大値,最小値
	int nNumTex;
	int nShadow;
	int nType;
	float fRadius;
	D3DXVECTOR3 pos; //位置
	D3DXVECTOR3 rot; //向き
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	bool bUse;
} Object;

//プロトタイプ宣言
void LoadObject(const char *FileName);

//グローバル変数
Object g_Object[8];
Object g_SetObject[MAX_OBJECT];
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
	g_Object[nCntModel].fRadius = (g_Object[nCntModel].vtxMaxModel.x - g_Object[nCntModel].vtxMinModel.x) / 2;
	if (g_Object[nCntModel].fRadius < (g_Object[nCntModel].vtxMaxModel.z - g_Object[nCntModel].vtxMinModel.z) / 2)
	{
		g_Object[nCntModel].fRadius = (g_Object[nCntModel].vtxMaxModel.z - g_Object[nCntModel].vtxMinModel.z) / 2;
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
}

//更新処理
void UpdateObject(void)
{
	//CollisionBullet(g_SetObject[0].pos, g_SetObject[0].fRadius);
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
			pMat = (D3DXMATERIAL*)g_SetObject[nCntModel].pBuffMat->GetBufferPointer();
			int nCntTex = 0;
			int nCntMat;
			for (nCntMat = 0; nCntMat < (int)g_SetObject[nCntModel].nNumMat; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//テクスチャの設定
					pDevice->SetTexture(0, g_SetObject[nCntModel].TextureModel[nCntTex]);
					nCntMat++;
				}
				else
				{
					pDevice->SetTexture(0, NULL);
				}
				//オブジェクトの描画
				g_SetObject[nCntModel].pMesh->DrawSubset(nCntMat);
			}
			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

void Setobject(D3DXVECTOR3 pos, int nType, D3DXVECTOR3 rot)
{
	int nCntObject;
	for (nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_SetObject[nCntObject].bUse == false)
		{
			g_SetObject[nCntObject].nType = nType;
			g_SetObject[nCntObject] = g_Object[nType];
			g_SetObject[nCntObject].nShadow = SetShadow(pos, g_SetObject[nCntObject].fRadius);
			g_SetObject[nCntObject].pos = pos;
			g_SetObject[nCntObject].rot = rot;
			g_SetObject[nCntObject].bUse = true;
			break;
		}
	}
}

void LoadObject(const char *FileName)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 vec;
	D3DXCOLOR col;
	float fDistance;
	int nType;
	int nBlock[2];
	int nSize[2];
	int nCntModel = 0;
	int nCntLight = 0;
	bool bModelSet = false;
	bool bCameraSet = false;
	bool bLightSet = false;
	bool bFieldSet = false;
	bool bWallSet = false;
	char aFile[256];
	FILE *pFile;
	pFile = fopen(FileName, "r");
	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]); //fscanfを繰り返してファイルを読み取っていく
			if (strcmp(&aFile[0], "NUM_MODEL") == 0) //モデル数
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &g_nNumObject);
			}
			if (strcmp(&aFile[0], "MODEL_FILENAME") == 0) //モデルファイル読み込み
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%s", &aFile[0]);
				D3DXLoadMeshFromX(&aFile[0],
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_Object[nCntModel].pBuffMat,
					NULL,
					&g_Object[nCntModel].nNumMat,
					&g_Object[nCntModel].pMesh);
				D3DXMATERIAL *pMat;
				for (nCntModel = 0; nCntModel < g_nNumObject; nCntModel++)
				{
					int nCntTex = 0;
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
				g_Object[nCntModel].fRadius = (g_Object[nCntModel].vtxMaxModel.x - g_Object[nCntModel].vtxMinModel.x) / 2;
				if (g_Object[nCntModel].fRadius < (g_Object[nCntModel].vtxMaxModel.z - g_Object[nCntModel].vtxMinModel.z) / 2)
				{
					g_Object[nCntModel].fRadius = (g_Object[nCntModel].vtxMaxModel.z - g_Object[nCntModel].vtxMinModel.z) / 2;
				}
				nCntModel++;
			}
			if (strcmp(&aFile[0], "MODELSET") == 0) //オブジェクトセット
			{
				bModelSet = true;
			}
			if (strcmp(&aFile[0], "END_MODELSET") == 0)
			{
				//Setobject(pos, nType, rot);
				bModelSet = false;
			}
			if (strcmp(&aFile[0], "CAMERASET") == 0) //カメラセット
			{
				bCameraSet = true;
			}
			if (strcmp(&aFile[0], "END_CAMERASET") == 0)
			{
				//SetCamera(pos, fDistance);
				bCameraSet = false;
			}
			if (strcmp(&aFile[0], "LIGHTSET") == 0) //ライトセット
			{
				bLightSet = true;
			}
			if (strcmp(&aFile[0], "END_LIGHTSET") == 0)
			{
				//SetLight(nCntLight, col, vec);
				nCntLight++;
				bLightSet = false;
			}
			if (strcmp(&aFile[0], "FIELDSET") == 0) //フィールドセット
			{
				bFieldSet = true;
			}
			if (strcmp(&aFile[0], "END_FIELDSET") == 0)
			{
				SetField(pos, rot, nBlock, nSize);
				bFieldSet = false;
			}
			if (strcmp(&aFile[0], "WALLSET") == 0) //カメラセット
			{

				bWallSet = true;
			}
			if (strcmp(&aFile[0], "END_WALLSET") == 0)
			{
				bWallSet = false;
			}
			if (bCameraSet == true)
			{
				if (strcmp(&aFile[0], "POS") == 0) //注視点の場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(&aFile[0], "DISTANCE") == 0) //距離
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &fDistance);
				}
			}
			if (bLightSet == true)
			{
				if (strcmp(&aFile[0], "DIRECTION") == 0) //方向
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &vec.x, &vec.y, &vec.z);
				}
				if (strcmp(&aFile[0], "DIFFUSE") == 0) //拡散光
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &col.r, &col.g, &col.b);
				}
			}
			if (bFieldSet == true)
			{
				if (strcmp(&aFile[0], "POS") == 0) //場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(&aFile[0], "ROT") == 0) //向き
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
					rot.x = (rot.x / 180.0f) * D3DX_PI - D3DX_PI;
					rot.y = (rot.y / 180.0f) * D3DX_PI - D3DX_PI;
					rot.z = (rot.z / 180.0f) * D3DX_PI - D3DX_PI;
				}
				if (strcmp(&aFile[0], "BLOCK") == 0) //場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d %d", &nBlock[0], &nBlock[1]);
				}
				if (strcmp(&aFile[0], "SIZE") == 0) //場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d %d", &nSize[0], &nSize[1]);
				}
			}
			if (bWallSet == true)
			{
				if (strcmp(&aFile[0], "POS") == 0) //場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(&aFile[0], "ROT") == 0) //向き
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
					rot.x = (rot.x / 180.0f) * D3DX_PI - D3DX_PI;
					rot.y = (rot.y / 180.0f) * D3DX_PI - D3DX_PI;
					rot.z = (rot.z / 180.0f) * D3DX_PI - D3DX_PI;
				}
				if (strcmp(&aFile[0], "BLOCK") == 0) //四角の枚数
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d %d", &nBlock[0], &nBlock[1]);
				}
				if (strcmp(&aFile[0], "SIZE") == 0) //大きさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d %d", &nSize[0], &nSize[1]);
				}
			}
			if (bModelSet == true)
			{
				if (strcmp(&aFile[0], "TYPE") == 0) //タイプ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nType);
				}
				if (strcmp(&aFile[0], "POS") == 0) //場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(&aFile[0], "ROT") == 0) //向き
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
					rot.x = (rot.x / 180.0f) * D3DX_PI - D3DX_PI;
					rot.y = (rot.y / 180.0f) * D3DX_PI - D3DX_PI;
					rot.z = (rot.z / 180.0f) * D3DX_PI - D3DX_PI;
				}
			}
		}
	}

}

//円柱状の当たり判定
void CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadius)
{
	float fLengthX; //誰かとオブジェクトのX方向の長さ
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
			if (fDistance <= fRadius + g_SetObject[nCntModel].fRadius) //オブジェクトに当たったとき
			{
				//pMove->x = 0.0f;
				//pMove->z = 0.0f;
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