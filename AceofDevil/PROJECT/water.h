//=============================================================================
//
// êÖëÆê´íeèàóù [water.h]
// Author : éOè„çqê¢
//
//=============================================================================
#ifndef _WATER_H_
#define _WATER_H_

#include "main.h"
#include "scene2d.h"

#define WATER_SIZE (240.0f)
#define WATER_MAX_ENEMY (16)

class CWater : public CScene2D
{
public:
	CWater(PRIORITY Priority = PRIORITY_BG);
	~CWater();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);
	void SetUninit(void);
	void SetRotDesh(float fRotDeshY);
	
	static HRESULT Load(void);
	static void UnLoad(void);
	static CWater *Create(D3DXVECTOR3 pos);

private:
	bool EnemyList(int nEnemy);
	void ListPush(int nEnemy);

	static LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 *m_pPlayerpos;
	float m_fRotY;
	float m_fRotDeshY;
	float m_fSize;
	bool m_bSize;
	int m_nListCnt;
	int m_aEnemies[WATER_MAX_ENEMY];
	bool m_bDraw;
};

#endif