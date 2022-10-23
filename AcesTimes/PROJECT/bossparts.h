//=============================================================================
//
// ボスパーツ処理 [bossparts.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _BOSSPARTS_H_
#define _BOSSPARTS_H_

#include "main.h"
#include "scene2d.h"

class CBossParts : public CScene2D
{
public:
	CBossParts(PRIORITY Priority = PRIORITY_CHARA);
	~CBossParts();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nFireTex, int nHP, int nMoney, int nScore);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	OBJTYPE GetObjType() { return OBJECTTYPE_ENEMY; }
	void Damage(int nDamage);
	void SetDestroy(bool bDeath) { m_bDeath = bDeath; }
	bool GetDestroy() { return m_bDeath; }

	static CBossParts *Create(D3DXVECTOR3 pos, int nTexType, int nFireTex, D3DXVECTOR2 size, int nHP, int nMoney, int nScore);

private:
	void CollisionBullet(D3DXVECTOR3 pos);

	CScene2D *m_pFire;
	int m_nHP; //体力
	int m_nDamageTime; //ダメージを食らってからの時間
	int m_nMoney;
	int m_nScore;
	int m_nCntFire;
	bool m_bFire;
	bool m_bDeath;
	int m_nCntDeath;
};
#endif