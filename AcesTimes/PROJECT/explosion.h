//=============================================================================
//
// 撃破処理 [explosion.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"
#include "scene2d.h"

class CExplosion : public CScene2D
{
public:
	CExplosion(PRIORITY Priority);
	~CExplosion();
	HRESULT Init(D3DXVECTOR3 pos, float fRadius, int nMaxCount);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画
	OBJTYPE GetObjType() { return OBJECTTYPE_NONE; }	//種類

	static CExplosion *Create(D3DXVECTOR3 pos, float fRadius, int nMaxCount);	//作成

private:
	int m_nCount;	//テクスチャパターンカウント
	int m_nMax;		//最大テクスチャパターン
	int m_nTexPat;	//テクスチャパターン
};

#endif