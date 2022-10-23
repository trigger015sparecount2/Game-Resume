//=============================================================================
//
// íeä€èàóù [bullet.h]
// Author : éOè„çqê¢
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
#include "scene2d.h"

#define BULLET_SIZE (10.0f)

class CBullet : public CScene2D
{
public:
	CBullet(PRIORITY Priority = PRIORITY_BULLET);
	~CBullet();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, bool bEnemy, CScene::ELEMENT element);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);

	static HRESULT Load(void);
	static void UnLoad(void);
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, bool bEnemy, CScene::ELEMENT element);

private:
	D3DXVECTOR3 m_move; //à⁄ìÆë¨ìx
	static LPDIRECT3DTEXTURE9 m_pTexture;
	int m_nEffect;
	bool m_bUninit;
};

#endif