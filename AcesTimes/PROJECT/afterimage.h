//=============================================================================
//
// écëúèàóù [afterimage.h]
// Author : éOè„çqê¢
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
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col);
	void Uninit();
	void Update();
	void Draw();
	OBJTYPE GetObjType() { return OBJECTTYPE_NONE; }

	static CAfterImage *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, int nTex);

private:
	D3DXCOLOR m_Col;
};
#endif