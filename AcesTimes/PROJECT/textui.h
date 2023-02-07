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
	//テキストの位置
	typedef enum
	{
		TEXT_WORDBREAK = 0,	//左詰め改行される
		TEXT_RIGHT,			//右詰め
		TEXT_MAX
	}TEXTTYPE;

	CTextUI(PRIORITY Priority);
	~CTextUI();
	HRESULT Init(int nSize, int nColor[4], TEXTTYPE texttype, char aText[128], RECT rect);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画
	void SetText(char aText[128]);	//文章設定
	void SetColor(int  nColor[4]);	//色替え

	static CTextUI *Create(int nSize, int nColor[4], TEXTTYPE texttype, char aText[128], RECT rect);	//作成
private:
	char m_aText[128];		//文章
	int m_nColor[4];		//色
	TEXTTYPE m_texttype;	//テキストの描画の仕方
	LPD3DXFONT m_pFont;		//フォントへのポインタ
	RECT m_rect;			//テキストの描画枠
};
#endif