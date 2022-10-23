//=============================================================================
//
// âŒëÆê´íeèàóù [fire.h]
// Author : éOè„çqê¢
//
//=============================================================================
#ifndef _FIRE_H_
#define _FIRE_H_
#include "main.h"
#include "scene2d.h"
#define FIRE_SIZE (20.0f)
class CFire : public CScene2D
{
public:
	CFire(PRIORITY Priority = PRIORITY_BULLET);
	~CFire();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);

	static HRESULT Load(void);
	static void UnLoad(void);
	static CFire *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move);
	D3DXVECTOR3 *RockEnemy(void);
	void Homing(D3DXVECTOR3 pos);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 *m_pPos;
	int m_nEnemy;
	int m_nEffect;
	float m_fRotY;
	bool m_bUninit;
	D3DXVECTOR3 m_move; //à⁄ìÆë¨ìx
	int m_nLife;
};

#endif