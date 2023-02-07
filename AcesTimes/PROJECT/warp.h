//=============================================================================
//
// ワープ先処理 [warp.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _WARP_H_
#define _WARP_H_

#include "main.h"
#include "scene2d.h"

class CWarp : public CScene2D
{
public:
	CWarp(PRIORITY Priority);
	~CWarp();
	HRESULT Init(D3DXVECTOR2 size);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }		//描画するか設定
	OBJTYPE GetObjType() { return OBJECTTYPE_NONE; }	//種類

	static CWarp *Create(D3DXVECTOR2 size);	//作成

private:
	float m_fClear;	//透明度
	bool m_bClear;	//点滅濃くするか薄くするか
	bool m_bDraw;	//描画するか
};
#endif