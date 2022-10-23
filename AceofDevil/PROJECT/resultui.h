//=============================================================================
//
// リザルトのUI処理 [resultui.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _RESULTUI_H_
#define _RESULTUI_H_
#include "main.h"
#include "scene.h"

class CScene2D;
class CInputKeyboard;
class CInputGamePad;

class CResultUi : public CScene
{
public:
	CResultUi(PRIORITY Priority = PRIORITY_UI);
	~CResultUi();
	HRESULT Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);

	static HRESULT Load(void);
	static void UnLoad(void);
	static CResultUi *Create(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[12]; //テクスチャへのポインタ
	static CScene2D *m_apScene[100];
	CInputKeyboard *m_pInputKeyboard;
	CInputGamePad *m_pInputGamePad;
	int m_nTime;
	bool m_bClear;
	float m_fClear;
	int m_nClearTime;
	int m_nKillRank;
	int m_nScoreRank;
	bool m_bRankClear;
};

#endif