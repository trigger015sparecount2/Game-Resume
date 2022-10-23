//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"
#include "scene2d.h"

#define BOSS_SIZE (80.0f)

class CBoss : public CScene2D
{
public:
	CBoss(PRIORITY Priority = PRIORITY_CHARA);
	~CBoss();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);

	static HRESULT Load(void);
	static void UnLoad(void);
	static CBoss *Create(void);

private:
	void Move(D3DXVECTOR3 pos);
	void Shot(D3DXVECTOR3 pos);
	float SearchPlayer(D3DXVECTOR3 pos);

	static LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_move;
	CScene2D *m_pShadowScene;
	int m_nBullet;
	int m_nBulletInter;
	int m_nHP; //体力
	int m_nDamageTime; //ダメージを食らってからの時間
	bool m_bUninit;
	bool m_bSlide;
	bool m_bDrop;
	bool m_bLeft;
	int m_nSlide;
	int m_nScore;
	int m_nDTime;
	int m_nCntBullet;
	int m_nCycle;
	int m_nBTime;
	int m_nType;
	float m_fScale;
	float m_fRotY;
};
#endif