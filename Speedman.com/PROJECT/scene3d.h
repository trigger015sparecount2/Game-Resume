//=============================================================================
//
// シーン処理 [scene3d.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include "scene.h"

struct VERTEX_3D
{
	D3DXVECTOR3 pos;	//頂点座標
	D3DXVECTOR3 nor;	//法線ベクトル
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
};

class CScene3D : public CScene
{
public:
	CScene3D(int nPriority);
	~CScene3D();
	HRESULT Init(D3DXVECTOR3 pos);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	//マトリックス設定
	D3DXMATRIX GetMatrix() { return m_mtxWorld; }
	void SetMatrix(const D3DXMATRIX& mtx) { m_mtxWorld = mtx; }

	//rot設定
	D3DXVECTOR3 GetRot() { return m_rot; }
	void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }

	static CScene3D *Create(const D3DXVECTOR3& pos);	//作成

private:
	D3DXVECTOR3 m_rot;		//向き
	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス
};
#endif
