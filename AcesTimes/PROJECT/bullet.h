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

class CBullet : public CScene2D
{
public:
	CBullet(PRIORITY Priority);
	~CBullet();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSpeed, bool bEnemy);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	OBJTYPE GetObjType() { return OBJECTTYPE_BULLET; }
	bool GetEnemy() { return m_bEnemy; }
	int Damage() { return 1; }

	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSpeed, bool bEnemy);

private:
	bool CollisionBomb(D3DXVECTOR3 pos);

	D3DXVECTOR3 m_move; //à⁄ìÆë¨ìx
	bool m_bEnemy;
	bool m_bSlow;
};

#endif