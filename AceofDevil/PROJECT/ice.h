//=============================================================================
//
// ïXíeèàóù [ice.h]
// Author : éOè„çqê¢
//
//=============================================================================
#ifndef _ICE_H_
#define _ICE_H_
#include "main.h"
#include "scene2d.h"

#define ICE_SIZE (20.0f)
#define ICE_MAX_ENEMY (16)

class CIce : public CScene2D
{
public:
	CIce(PRIORITY Priority = PRIORITY_BULLET);
	~CIce();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRotY);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);

	static HRESULT Load(void);
	static void UnLoad(void);
	static CIce *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRotY);

private:
	bool EnemyList(int nEnemy);
	void ListPush(int nEnemy);

	D3DXVECTOR3 m_move; //à⁄ìÆë¨ìx
	static LPDIRECT3DTEXTURE9 m_pTexture;
	float m_fRotY;
	bool m_bUninit;
	int m_nListCnt;
	int m_nEffect;
	int m_aEnemies[ICE_MAX_ENEMY];
};

#endif