//=============================================================================
//
// シーン処理 [model.h]
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
	HRESULT Init(const char *aModelName);
	void Uninit();
	void Update();
	void Draw();
	void DrawObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void SetParent(CModel *model) { m_pParent = model; }
	CModel *GetParent() { return m_pParent; }
	D3DXMATRIX GetMatrix();
	void SetMatrix(D3DXMATRIX Matrix);
	void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos() { return m_pos; }
	void SetRot(D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRot() { return m_rot; }
	void SetOriPos(D3DXVECTOR3 pos);
	void SetOriRot(D3DXVECTOR3 rot);
	D3DXVECTOR3 GetMinSize();
	D3DXVECTOR3 GetMaxSize();
	
	static CModel *Create(const char *aModelName);

private:
	LPD3DXMESH m_pMesh; //メッシュ（頂点情報）へのポインタ
	LPD3DXBUFFER m_pBuffMat; //マテリアル（材質情報）へのポインタ
	DWORD m_nNumMat; //マテリアルの数
	LPDIRECT3DTEXTURE9 m_aTextureModel[4] = {};
	D3DXVECTOR3 m_pos; //位置
	D3DXVECTOR3 m_posOri; //元々の位置
	D3DXVECTOR3 m_rot; //向き
	D3DXVECTOR3 m_rotOri; //元々の向き
	D3DXMATRIX m_mtxWorld; //ワールドマトリックス
	int m_nIdxModelParent; //親モデルのインデックス

	CModel *m_pParent;
};

#endif