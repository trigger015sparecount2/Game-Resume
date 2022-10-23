//=============================================================================
//
// óãíeèàóù [thunder.h]
// Author : éOè„çqê¢
//
//=============================================================================
#ifndef _THUNDER_H_
#define _THUNDER_H_
#include "main.h"
#include "scene2d.h"

#define THUNDER_SIZE (25.0f)
#define BAND_LENGTH (300.0f)

class CThunder : public CScene2D
{
public:
	CThunder(PRIORITY Priority = PRIORITY_BULLET);
	~CThunder();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);

	static HRESULT Load(void);
	static void UnLoad(void);
	static CThunder *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move);

private:
	void BandCreate(int nEnemy, D3DXVECTOR3 pos);

	int m_nEffectTime;
	D3DXVECTOR3 m_move; //à⁄ìÆë¨ìx
	static LPDIRECT3DTEXTURE9 m_pTexture;
	CScene2D *m_pThunderEffect;
	bool m_bUninit;
};

#endif