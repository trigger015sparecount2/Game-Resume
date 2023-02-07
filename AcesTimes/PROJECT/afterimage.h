//=============================================================================
//
// 残像処理 [afterimage.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _AFTERIMAGE_H_
#define _AFTERIMAGE_H_

#include "main.h"
#include "scene2d.h"

class CAfterImage : public CScene2D
{
public:
	CAfterImage(PRIORITY Priority);
	~CAfterImage();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画
	OBJTYPE GetObjType() { return OBJECTTYPE_NONE; }	//種類

	static CAfterImage *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, int nTex);	//作成

private:
	D3DXCOLOR m_Col;	//色
};
#endif