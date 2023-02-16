//=============================================================================
//
// モデル処理 [model.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

class CModel
{
public:
	CModel();
	~CModel();
	HRESULT Init(const char *aModelName);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画
	void DrawObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	//オブジェクト描画

	//親設定
	void SetParent(CModel *model) { m_pParent = model; }
	CModel *GetParent() { return m_pParent; }

	//マトリックス設定
	D3DXMATRIX GetMatrix() { return m_mtxWorld; }
	void SetMatrix(D3DXMATRIX Matrix) { m_mtxWorld = Matrix; }

	//pos設定
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos() { return m_pos; }

	//rot設定
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot() { return m_rot; }

	//元のposとrotの設定
	void SetOriPos(D3DXVECTOR3 pos) { m_posOri = pos; }
	void SetOriRot(D3DXVECTOR3 rot) { m_rotOri = rot; }

	//最大最小サイズ取得
	D3DXVECTOR3 GetMinSize();
	D3DXVECTOR3 GetMaxSize();
	
	static CModel *Create(const char *aModelName);	//作成

private:
	LPD3DXMESH m_pMesh;			//メッシュ（頂点情報）へのポインタ
	LPD3DXBUFFER m_pBuffMat;	//マテリアル（材質情報）へのポインタ
	DWORD m_nNumMat;			//マテリアルの数
	LPDIRECT3DTEXTURE9 m_aTextureModel[4] = {};	//テクスチャ
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_posOri;		//元々の位置
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_rotOri;		//元々の向き
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	int m_nIdxModelParent;		//親モデルのインデックス

	CModel *m_pParent;			//親モデル
};

#endif