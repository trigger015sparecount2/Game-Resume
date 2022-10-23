//=============================================================================
//
// 雷エフェクト処理 [thundereffect.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _THUNDEREFFECT_H_
#define _THUNDEREFFECT_H_
#include "main.h"
#include "scene2d.h"
#define THUNDEREFFECT_SIZE (60.0f)
class CThundereffect : public CScene2D
{
public:
	CThundereffect(PRIORITY Priority = PRIORITY_BG);
	~CThundereffect();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);

	static HRESULT Load(void);
	static void UnLoad(void);
	static CThundereffect *Create(D3DXVECTOR3 pos);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
};

#endif