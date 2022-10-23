//=============================================================================
//
// プライズ処理 [prize.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _PRIZE_H_
#define _PRIZE_H_

#include "main.h"
#include "scene2d.h"

#define PRIZE_SIZE (12.0f)

class CPrize : public CScene2D
{
public:
	CPrize(PRIORITY Priority = PRIORITY_BG);
	~CPrize();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, ELEMENT element, int nHeal);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);

	static HRESULT Load(void);
	static void UnLoad(void);
	static CPrize *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, ELEMENT element, int nHeal);

private:
	D3DXVECTOR3 m_move; //移動速度
	D3DXVECTOR3 *m_pPos;
	float m_fRotY;
	float m_fSize;
	ELEMENT m_Element;
	static LPDIRECT3DTEXTURE9 m_pTexture;
	int m_nTime;
	int m_nHeal;
	bool m_bUninit;
};

#endif