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
	//テキストのモード
	typedef enum
	{
		TEXTMODE_NONE = 0,
		TEXTMODE_IN,
		TEXTMODE_OUT,
		TEXTMODE_MAX
	} TEXTMODE;

	CTutorialUi(PRIORITY Priority = PRIORITY_UI);
	~CTutorialUi();
	HRESULT Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize);	//初期化
	void Uninit(void);						//終了
	void Update(void);						//更新
	void Draw(void);						//描画
	bool GetDamage(void) { return false; }	//ダメージ受けるか
	void Damage(int) { ; }					//ダメージ処理

	static bool GetClear(void) { return m_bClear; }							//透過させるか
	static void NextText(void) { m_textmode = TEXTMODE_IN; }				//次の文章を表示させる
	static void TutoClear(D3DXVECTOR3 pos, float fWidth, float fHeight);	//チュートリアルUIを透過させる
	static void DrawTutoText(void);		//チュートリアルの文章を表示させる
	static HRESULT Load(void);			//テクスチャ読み込み
	static void UnLoad(void);			//テクスチャ破棄
	static CTutorialUi *Create(void);	//作成

private:
	void TextChange(void);						//文章を変える
	static int Compare(void);					//テキストの文字の濃さと四角形の濃さとを比較、薄いほうを返す

	char m_aText[22][MAX_BYTE];					//ファイルから読み取った文章
	int m_nCntText;								//何番目の文章か
	static bool m_bClear;						//UIが薄いか
	static TEXTMODE m_textmode;					//文章のモード
	static char m_Str[MAX_BYTE];				//表示させる文章
	static LPD3DXFONT m_pFont;					//フォント
	static int m_nTextClear;					//テキストの透明度
	static int m_nSquareClear;					//四角形の透明度
	static LPDIRECT3DTEXTURE9 m_pTexture[2];	//テクスチャへのポインタ
	static CScene2D *m_apScene[MAX_TUTOUI];		//シーン
};

#endif