//=============================================================================
//
// 壁処理 [wall.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"
#include "plane.h"

class CWall : public CPlane
{
public:
	CWall(PRIORITY Priority);
	~CWall();
	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR2 Tex, D3DXVECTOR3 rot);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画
	OBJTYPE GetObjType() { return OBJECTTYPE_WALL; }	//種類
	D3DXVECTOR3 GetVtxMax() { return m_VtxMax; }		//最大値取得
	D3DXVECTOR3 GetVtxMin() { return m_VtxMin; }		//最小値取得

	static CWall *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos, int nTex, D3DXVECTOR2 Tex, D3DXVECTOR3 rot);	//作成

private:
	D3DXVECTOR3 m_VtxMax;	//最大値
	D3DXVECTOR3 m_VtxMin;	//最小値
};

#endif