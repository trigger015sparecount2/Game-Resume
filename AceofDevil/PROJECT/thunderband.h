//=============================================================================
//
// óãÇÃë—èàóù [thunderband.h]
// Author : éOè„çqê¢
//
//=============================================================================
#ifndef _THUNDERBAND_H_
#define _THUNDERBAND_H_
#include "main.h"
#include "scene2d.h"
#define BAND_WIDTH (40.0f)
class CThunderBand : public CScene2D
{
public:
	CThunderBand(PRIORITY Priority = PRIORITY_CHARA);
	~CThunderBand();
	HRESULT Init(D3DXVECTOR3 pos, float fRotY, float fLength);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);

	static HRESULT Load(void);
	static void UnLoad(void);
	static CThunderBand *Create(D3DXVECTOR3 pos, float fRotY, float fLength);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	int m_nLife;
	int m_nTex;
};

#endif