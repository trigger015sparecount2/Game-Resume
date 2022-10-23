//=============================================================================
//
// タイトルのUI処理 [titleui.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _TITLEUI_H_
#define _TITLEUI_H_
#include "main.h"
#include "scene.h"

class CScene2D;
class CTitleUi : public CScene
{
public:
	CTitleUi(PRIORITY Priority = PRIORITY_UI);
	~CTitleUi();
	HRESULT Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);

	static HRESULT Load(void);
	static void UnLoad(void);
	static CTitleUi *Create(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[2]; //テクスチャへのポインタ
	static CScene2D *m_apScene[2];
	bool m_bClear;
	float m_fClear;
};

#endif