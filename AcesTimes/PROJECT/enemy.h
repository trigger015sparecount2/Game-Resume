//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "scene2d.h"

class CShadow;

class CEnemy : public CScene2D
{
public:
	CEnemy(PRIORITY Priority = PRIORITY_CHARA);
	~CEnemy();
	HRESULT Init(D3DXVECTOR3 pos, float fRotY, float fInterval, float fSpeed, D3DXVECTOR2 size, int nShadowTex, int nHP, float fBulletMax, int nMoney, int nScore, float fBulletSpeed, int nBulletType);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	OBJTYPE GetObjType() { return OBJECTTYPE_ENEMY; }
	void Damage(int nDamage);

	static CEnemy *Create(D3DXVECTOR3 pos, int nTexType, int nShadowTex, float fRotY, float fInterval, float fSpeed, D3DXVECTOR2 size, int nHP, float fBulletMax, int nMoney, int nScore, float fBulletSpeed, int nBulletType);
	//static int GetEnemy() { return m_nEnemy; }


	//static int m_nEnemy;

private:
	D3DXVECTOR3 Move();
	D3DXVECTOR3 SlowMove();
	void CollisionBullet(D3DXVECTOR3 pos);
	bool CollisionBomb(D3DXVECTOR3 pos);
	void Shot(D3DXVECTOR3 pos);

	CShadow *m_pShadow;
	float m_fBullet;
	float m_fBulletMax;
	int m_nBulletInter;
	int m_nHP; //体力
	int m_nDamageTime; //ダメージを食らってからの時間
	bool m_bShot;
	float m_fSpeed;
	float m_fRot;
	int m_nType;
	bool m_bSlow;
	int m_nMoney;
	int m_nScore;
	float m_fBulletSpeed;
	int m_nBulletType;
};
#endif