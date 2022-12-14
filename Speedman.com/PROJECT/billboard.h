//=============================================================================
//
// シーン処理 [billboard.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"
#include "plane.h"

class CBillboard : public CPlane
{
public:
	CBillboard(int nPriority);
	~CBillboard();
	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();
	static CBillboard *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos);

private:
	LPDIRECT3DTEXTURE9 m_pTexture;
};

#endif