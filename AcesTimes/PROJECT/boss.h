//=============================================================================
//
// ボス処理 [boss.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"
#include "scene2d.h"

class CBossParts;
class CShadow;
class CBoss : public CScene2D
{
public:
	CBoss(PRIORITY Priority = PRIORITY_CHARA);
	~CBoss();
	HRESULT Init(D3DXVECTOR3 pos, float fInterval, D3DXVECTOR2 size, int nShadowTex, int nHP, float fBulletMax, int nMoney, int nScore, bool bParts, int nBGMType);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	OBJTYPE GetObjType() { return OBJECTTYPE_ENEMY; }
	void Damage(int nDamage);

	static CBoss *Create(D3DXVECTOR3 pos, int nTexType, int nShadowTex, float fInterval, D3DXVECTOR2 size, int nHP, float fBulletMax, int nMoney, int nScore, bool bParts, int nBGMType);

private:
	D3DXVECTOR3 Move();
	D3DXVECTOR3 SlowMove();
	void CollisionBullet(D3DXVECTOR3 pos);
	bool CollisionBomb(D3DXVECTOR3 pos);
	void Shot(D3DXVECTOR3 pos);
	void PartsShot(D3DXVECTOR3 pos, int nCntBullet);

	CShadow *m_pShadow;
	float m_fBullet[7];
	float m_fBulletMax;
	int m_nBulletInter;
	int m_nHP; //体力
	int m_nDamageTime; //ダメージを食らってからの時間
	bool m_bShot;
	float m_fRot;
	int m_nType;
	bool m_bSlow;
	int m_nMoney;
	int m_nScore;
	bool m_bParts;
	bool m_bMove[2];
	int m_nCntStart;
	int m_nBullet;
	bool m_bDeath;
	int m_nCntDeath;
	CBossParts *m_pBossParts[8];
};
#endif