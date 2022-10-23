//=============================================================================
//
// åÇîjèàóù [explosion.h]
// Author : éOè„çqê¢
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
	HRESULT Init(D3DXVECTOR3 pos, float fRadius, int nMaxCount);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	OBJTYPE GetObjType() { return OBJECTTYPE_NONE; }

	static CExplosion *Create(D3DXVECTOR3 pos, float fRadius, int nMaxCount);

private:
	int m_nCount;
	int m_nMax;
	int m_nTexPat;
};

#endif