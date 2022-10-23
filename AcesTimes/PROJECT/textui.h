//=============================================================================
//
// UIのテキスト部分処理 [textui.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _TEXTUI_H_
#define _TEXTUI_H_

#include "main.h"
#include "scene2d.h"

class CTextUI : public CScene2D
{
public:
	typedef enum
	{
		TEXT_WORDBREAK = 0,
		TEXT_RIGHT,
		TEXT_MAX
	}TEXTTYPE;

	CTextUI(PRIORITY Priority);
	~CTextUI();
	HRESULT Init(int nSize, int nColor[4], TEXTTYPE texttype, char aText[128], RECT rect);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetText(char aText[128]);
	void SetColor(int  nColor[4]);
	void SetNum(int nText) { m_nText = nText; }

	static CTextUI *Create(int nSize, int nColor[4], TEXTTYPE texttype, char aText[128], RECT rect);
private:
	char m_aText[128];
	int m_nColor[4];
	TEXTTYPE m_texttype;
	LPD3DXFONT m_pFont; // フォントへのポインタ
	RECT m_rect;
	int m_nText;
};
#endif