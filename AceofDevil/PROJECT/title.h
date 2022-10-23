//=============================================================================
//
// タイトルシーン [title.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "scene.h"

//前方宣言
class CInputKeyboard;
class CInputGamePad;
class CScene2D;

class CTitle : public CScene
{
public:
	CTitle(PRIORITY Priority = PRIORITY_BG);
	~CTitle();

	HRESULT Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	static CScene2D *m_pScene2D;
	CInputKeyboard *m_pInputKeyboard;
	CInputGamePad *m_pInputGamePad;
};
#endif