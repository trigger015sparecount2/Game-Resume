//---------------------------
//Author:三上航世
//モデル(model.cpp)
//---------------------------
#include "model.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "gamepad.h"
#include "bullet.h"
#include "particle.h"
#include "enemy.h"

//グローバル変数
LPD3DXMESH g_pMeshModel = NULL; //メッシュ（頂点情報）へのポインタ
LPD3DXBUFFER g_pBuffMatModel = NULL; //マテリアル（材質情報）へのポインタ
DWORD g_nNumMatModel = 1; //マテリアルの数
LPDIRECT3DTEXTURE9 g_aTextureModel[1] = {}; //テクスチャ
D3DXVECTOR3 g_posModel; //位置
D3DXVECTOR3 g_rotModel; //向き
D3DXVECTOR3 g_rotModelDesh; //目的の向き
D3DXMATRIX g_mtxWorldModel; //ワールドマトリックス
D3DXVECTOR3 g_movePolygon;
D3DXVECTOR3 g_vtxMinModel, g_vtxMaxModel; //モデルの最大値,最小値
int g_nShadow;
bool g_bArrow;

//初期化処理
HRESULT InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	//Xファイルの読み込み
	D3DXLoadMeshFromX("data/Model/ball.x",
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&g_pBuffMatModel,
						NULL,
						&g_nNumMatModel,
						&g_pMeshModel);
	int nCntTex = 0;
	g_bArrow = false;
	D3DXMATERIAL *pMat;
	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_aTextureModel[nCntTex]);
			nCntTex++;
		}
	}
	int nNumVtx; //頂点数
	DWORD sizeFVF; //頂点フォーマットのサイズ
	BYTE *pVtxBuff; //頂点バッファへのポインタ
	//頂点数を取得
	nNumVtx = g_pMeshModel->GetNumVertices();
	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(g_pMeshModel->GetFVF());
	g_vtxMaxModel.x = -10000.0f;
	g_vtxMaxModel.y = -10000.0f;
	g_vtxMaxModel.z = -10000.0f;
	g_vtxMinModel.x = 10000.0f;
	g_vtxMinModel.y = 10000.0f;
	g_vtxMinModel.z = 10000.0f;
	//頂点バッファのロック
	g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標の代入
		if (g_vtxMinModel.x > vtx.x)
		{
			g_vtxMinModel.x = vtx.x;
		}
		if (g_vtxMinModel.y > vtx.y)
		{
			g_vtxMinModel.y = vtx.y;
		}
		if (g_vtxMinModel.z > vtx.z)
		{
			g_vtxMinModel.z = vtx.z;
		}
		if (g_vtxMaxModel.x < vtx.x)
		{
			g_vtxMaxModel.x = vtx.x;
		}
		if (g_vtxMaxModel.y < vtx.y)
		{
			g_vtxMaxModel.y = vtx.y;
		}
		if (g_vtxMaxModel.z < vtx.z)
		{
			g_vtxMaxModel.z = vtx.z;
		}
		pVtxBuff += sizeFVF; //頂点フォーマットのサイズ分ポインタを進める
	}
	//頂点バッファをアンロック
	g_pMeshModel->UnlockVertexBuffer();
	g_posModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotModelDesh = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nShadow = SetShadow(g_posModel, 10.0f);
	g_movePolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	return S_OK;
}

//終了処理
void UninitModel(void)
{
	//テクスチャの破棄
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}
	//頂点バッファの破棄
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//更新処理
void UpdateModel(void)
{
	g_posModel = ECollisionPos();
}

//描画処理
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice;   //デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
	D3DMATERIAL9 matDef;		 //現在のマテリアル保存用
	D3DXMATERIAL *pMat;			 //マテリアルデータへのポインタ
	pDevice = GetDevice();		 //デバイスを取得する
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldModel);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotModel.y, g_rotModel.x, g_rotModel.z);
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posModel.x, g_posModel.y, g_posModel.z);
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModel);
	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);
	//マテリアルデータへのポインタの反映
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();
	int nCntTex = 0;
	int nCntMat;
	for (nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_aTextureModel[nCntTex]);
			nCntMat++;
		}
		else
		{
			pDevice->SetTexture(0, NULL);
		}
		//モデル（パーツ）の描画
		g_pMeshModel->DrawSubset(nCntMat);
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//モデルの場所
float ModelPos(int nRot)
{
	switch (nRot)
	{
	case 0:
		return g_posModel.x;
		break;
	case 1:
		return g_posModel.y;
		break;
	case 2:
		return g_posModel.z;
		break;
	}
	return 0.0f;
}

//モデルの向き
float ModelRot(void)
{
	return g_rotModel.y;
}

//移動速度横
void ModelMoveX(void)
{
	g_movePolygon.x -= (sinf(g_rotModelDesh.y) * 5.0f + g_movePolygon.x) * 0.1f;
}

//移動速度縦
void ModelMoveZ(void)
{
	g_movePolygon.z -= (cosf(g_rotModelDesh.y) * 5.0f + g_movePolygon.z) * 0.1f;
}

//モデルの目的の向き取得
float ModelRotDesh(void)
{
	return g_rotModelDesh.y;
}

//モデルの移動量取得
float ModelMove(int nRot)
{
	switch (nRot)
	{
	case 0:
		return g_movePolygon.x;
		break;
	case 1:
		return g_movePolygon.y;
		break;
	case 2:
		return g_movePolygon.z;
		break;
	}
	return 0.0f;
}

//モデルのサイズの最小値
float ModelMinSize(int nRot)
{
	switch (nRot)
	{
	case 0:
		return g_vtxMinModel.x;
		break;
	case 1:
		return g_vtxMinModel.y;
		break;
	case 2:
		return g_vtxMinModel.z;
		break;
	}
	return 0.0f;
}

//モデルのサイズの最大値
float ModelMaxSize(int nRot)
{
	switch (nRot)
	{
	case 0:
		return g_vtxMaxModel.x;
		break;
	case 1:
		return g_vtxMaxModel.y;
		break;
	case 2:
		return g_vtxMaxModel.z;
		break;
	}
	return 0.0f;
}

//移動速度低下横
void ModelMoveXslow(void)
{
	g_movePolygon.x -= g_movePolygon.x * 0.1f;
}

//移動速度低下縦
void ModelMoveZslow(void)
{
	g_movePolygon.z -= g_movePolygon.z * 0.1f;
}