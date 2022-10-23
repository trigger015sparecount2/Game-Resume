//=============================================================================
//
// É{ÉÄèàóù [bomb.h]
// Author : éOè„çqê¢
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
	HRESULT Init(D3DXVECTOR3 pos, float fRadius, int nCount);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	OBJTYPE GetObjType() { return OBJECTTYPE_BOMB; }

	static CBomb *Create(D3DXVECTOR3 pos, float fRadius, int nCount);

private:
	int m_nCount;
	float m_fClear;
	bool m_bClear;
};

#endif