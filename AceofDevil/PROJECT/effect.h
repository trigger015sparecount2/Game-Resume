//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_
#include "main.h"
#include "scene2d.h"
#define EFFECT_SIZE (20.0f)
class CEffect : public CScene2D
{
public:
	typedef enum
	{
		COLORTYPE_WHITE = 0,
		COLORTYPE_RED,
		COLORTYPE_LIGHTBLUE,
		COLORTYPE_YELLOW,
		COLORTYPE_BLUE,
		COLORTYPE_GREEN,
		COLORTYPE_MAX
	} COLORTYPE;
	CEffect(PRIORITY Priority = PRIORITY_EFFECT);
	~CEffect();
	HRESULT Init(D3DXVECTOR3 pos, COLORTYPE colType);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);

	static HRESULT Load(void);
	static void UnLoad(void);
	static CEffect *Create(D3DXVECTOR3 pos, COLORTYPE colType);

private:
	float m_fSize;
	D3DXCOLOR m_col;
	COLORTYPE m_colType;
	static LPDIRECT3DTEXTURE9 m_pTexture;
};

#endif