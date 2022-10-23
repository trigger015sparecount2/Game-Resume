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
	D3DXVECTOR3 pos; //頂点座標
	D3DXVECTOR3 nor; //法線ベクトル
	D3DCOLOR col;    //頂点カラー
	D3DXVECTOR2 tex; //テクスチャ座標
};

class CScene3D : public CScene
{
public:
	CScene3D(int nPriority);
	~CScene3D();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();
	D3DXMATRIX GetMatrix() const;
	void SetMatrix(const D3DXMATRIX& mtx);
	D3DXVECTOR3 GetRot() const;
	void SetRot(const D3DXVECTOR3& rot);
	static CScene3D *Create(const D3DXVECTOR3& pos);


private:
	D3DXVECTOR3 m_rot; //向き
	D3DXMATRIX m_mtxWorld; //ワールドマトリックス
};
#endif
