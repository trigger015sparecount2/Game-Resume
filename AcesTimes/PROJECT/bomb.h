//=============================================================================
//
// ボム処理 [bomb.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _BOMB_H_
#define _BOMB_H_

#include "main.h"
#include "scene2d.h"

class CBomb : public CScene2D
{
public:
	CBomb(PRIORITY Priority);
	~CBomb();
	HRESULT Init(D3DXVECTOR3 pos, float fRadius, int nCount);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画
	OBJTYPE GetObjType() { return OBJECTTYPE_BOMB; }	//種類

	static CBomb *Create(D3DXVECTOR3 pos, float fRadius, int nCount);	//作成

private:
	int m_nCount;	//寿命
	float m_fClear;	//透明度
	bool m_bClear;	//点滅
};

#endif