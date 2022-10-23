//=============================================================================
//
// ポーズ中UI処理 [pauseui.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _PAUSEUI_H_
#define _PAUSEUI_H_

#include "main.h"
#include "scene.h"

#define MAX_PAUSEDATA (7)

class CScene2D;
class CInputGamePad;
class CInputKeyboard;

class CPauseUi : public CScene
{
public:
	CPauseUi(PRIORITY Priority = PRIORITY_UI);
	~CPauseUi();
	HRESULT Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);
	void Pause(void);
	void PauseLift(void);

	static HRESULT Load(void);
	static void UnLoad(void);
	static CPauseUi *Create(void);
	static void SetPause(bool bPause);
	static bool GetPause(void);

private:
	CInputGamePad *m_pInputGamePad;
	CInputKeyboard *m_pInputKeyboard;
	static LPDIRECT3DTEXTURE9 m_pTexture[6]; //テクスチャへのポインタ
	static CScene2D *m_apScene[MAX_PAUSEDATA];
	static bool m_bUse;
	int m_nSelect;
};

#endif