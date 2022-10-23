//=============================================================================
//
// ïóëÆê´íeèàóù [wind.h]
// Author : éOè„çqê¢
//
//=============================================================================
#ifndef _WIND_H_
#define _WIND_H_
#include "main.h"
#include "scene2d.h"

#define WIND_SIZE (200.0f)
#define WIND_MAX_ENEMY (16)

class CWind : public CScene2D
{
public:
	CWind(PRIORITY Priority = PRIORITY_BULLET);
	~CWind();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);
	void SetSize(bool bWind);

	static HRESULT Load(void);
	static void UnLoad(void);
	static CWind *Create(D3DXVECTOR3 pos);

private:
	bool EnemyList(int nEnemy);
	void ListPush(int nEnemy);

	static LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 *m_pPlayerpos;
	float m_fRotY;
	float m_fSize;
	bool m_bSize;
	int m_nListCnt;
	int m_aEnemies[WIND_MAX_ENEMY];
	bool m_bDraw;
};
#endif