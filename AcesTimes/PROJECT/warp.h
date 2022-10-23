//=============================================================================
//
// ÉèÅ[ÉvêÊèàóù [warp.h]
// Author : éOè„çqê¢
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
	HRESULT Init(D3DXVECTOR2 size);
	void Uninit();
	void Update();
	void Draw();
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }
	OBJTYPE GetObjType() { return OBJECTTYPE_NONE; }

	static CWarp *Create(D3DXVECTOR2 size);

private:
	float m_fClear;
	bool m_bClear;
	bool m_bDraw;
};
#endif