//=============================================================================
//
// 背景処理 [bg.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_
#include "main.h"
#include "scene.h"
class CScene2D;
class CBg : public CScene
{
public:
	CBg(PRIORITY Priority = PRIORITY_BG);
	~CBg();
	HRESULT Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);
	
	static HRESULT Load(void);
	static void UnLoad(void);
	static CBg *Create(bool bScroll);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture; //テクスチャへのポインタ
	CScene2D *m_apScene;
	float m_fTexY;
	static bool m_bScroll;
};

#endif