//=============================================================================
//
// UI処理 [tutorialui.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _TUTORIALUI_H_
#define _TUTORIALUI_H_
#include "main.h"
#include "scene.h"

//マクロ定義
#define MAX_TUTOUI (3)
#define MAX_BYTE (256)

class CScene2D;

class CTutorialUi : public CScene
{
public:
	typedef enum
	{
		TEXTMODE_NONE = 0,
		TEXTMODE_IN,
		TEXTMODE_OUT,
		TEXTMODE_MAX
	} TEXTMODE;
	CTutorialUi(PRIORITY Priority = PRIORITY_UI);
	~CTutorialUi();
	HRESULT Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);

	static bool GetClear(void);
	static void NextText(void);
	static void TutoClear(D3DXVECTOR3 pos, float fWidth, float fHeight);
	static void DrawTutoText(void);
	static HRESULT Load(void);
	static void UnLoad(void);
	static CTutorialUi *Create(void);

private:
	void TextChange(void);
	static int Compare(void);

	char m_aText[22][MAX_BYTE];
	int m_nCntText;
	static bool m_bClear;
	static TEXTMODE m_textmode;
	static char m_Str[MAX_BYTE];
	static LPD3DXFONT m_pFont;
	static int m_nTextClear;
	static int m_nSquareClear;
	static LPDIRECT3DTEXTURE9 m_pTexture[2]; //テクスチャへのポインタ
	static CScene2D *m_apScene[MAX_TUTOUI];
};

#endif