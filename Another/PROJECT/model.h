//=============================================================================
//
// モデル [model.h]
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
	void DrawObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	//オブジェクトの描画

	//親モデルの格納・取得
	void SetParent(CModel *model) { m_pParent = model; }
	CModel *GetParent() { return m_pParent; }

	//親モデルの番号の格納・取得
	void SetIdxParent(int nIdx) { m_nIdxModelParent = nIdx; }
	int GetIdxParent() { return m_nIdxModelParent; }

	//マトリックスの格納・取得
	D3DXMATRIX GetMatrix() { return m_mtxWorld; }
	void SetMatrix(D3DXMATRIX Matrix) { m_mtxWorld = Matrix; }

	//posの格納・取得
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos() { return m_pos; }

	//方向の格納・取得
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot() { return m_rot; }

	void SetOriPos(D3DXVECTOR3 pos) { m_posOri = pos; }	//元々のpos設定
	void SetOriRot(D3DXVECTOR3 rot) { m_rotOri = rot; }	//元々の方向設定
	D3DXVECTOR3 GetMinSize();	//最小のサイズ
	D3DXVECTOR3 GetMaxSize();	//最大のサイズ
	void Copy(CModel *pModel);	//モデルのコピー

	static CModel *Create(const char *aModelName);	//作成

private:
	LPD3DXMESH m_pMesh;							//メッシュ（頂点情報）へのポインタ
	LPD3DXBUFFER m_pBuffMat;					//マテリアル（材質情報）へのポインタ
	DWORD m_nNumMat;							//マテリアルの数
	LPDIRECT3DTEXTURE9 m_aTextureModel[4] = {};	//テクスチャ
	D3DXVECTOR3 m_pos;							//位置
	D3DXVECTOR3 m_posOri;						//元々の位置
	D3DXVECTOR3 m_rot;							//向き
	D3DXVECTOR3 m_rotOri;						//元々の向き
	D3DXMATRIX m_mtxWorld;						//ワールドマトリックス
	int m_nIdxModelParent;						//親モデルのインデックス

	CModel *m_pParent;
};

#endif