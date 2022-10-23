//=============================================================================
//
// âeèàóù [shadow.h]
// Author : éOè„çqê¢
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "scene2d.h"

class CShadow : public CScene2D
{
public:
	CShadow(PRIORITY Priority = PRIORITY_BG);
	~CShadow();
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);

	static HRESULT Load(void);
	static void UnLoad(void);
	static CShadow *Create(D3DXVECTOR3 pos, float fWidth, float fHeight, SHADOW shadow);

private:
	float m_fWidth;
	float m_fHeight;
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_SHADOW];
	bool m_bUninit;
};

#endif