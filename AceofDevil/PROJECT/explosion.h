//=============================================================================
//
// îöî≠èàóù [explosion.h]
// Author : éOè„çqê¢
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_
#include "main.h"
#include "scene2d.h"
#define EXPLOSION_SIZE (50.0f)
class CExplosion : public CScene2D
{
public:
	CExplosion(PRIORITY Priority = PRIORITY_EFFECT);
	~CExplosion();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);

	static HRESULT Load(void);
	static void UnLoad(void);
	static CExplosion *Create(D3DXVECTOR3 pos);

private:
	int m_nPatternAnim;
	int m_nCounterAnim;
	static LPDIRECT3DTEXTURE9 m_pTexture;
};

#endif