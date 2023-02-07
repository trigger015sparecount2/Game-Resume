//=============================================================================
//
// マネージャ [Manager.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "main.h"
//前方宣言
class CRenderer;
class CInputKeyboard;
class CInputGamePad;
class CTitle;
class CTutorial;
class CGame;
class CResult;
class CFade;
class CManager
{
public:
	typedef enum
	{
		MODE_TITLE = 0,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_MAX
	} MODE;
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);	//初期化処理
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画
	static void SetMode(MODE mode);				//モード設定
	static MODE GetMode() { return m_mode; }	//モードを返す
	static CRenderer *GetRenderer(void) { return m_pRenderer; }					//レンダラーのポインタ
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }	//キーボードのポインタ
	static CInputGamePad *GetInputGamePad(void);								//ゲームパッドのポインタ

private:
	static CRenderer *m_pRenderer;				//レンダラーのポインタ
	static CInputKeyboard *m_pInputKeyboard;	//キーボードのポインタ
	static CInputGamePad *m_pInputGamePad;		//ゲームパッドのポインタ
	static CTitle *m_pTitle;					//タイトル
	static CTutorial *m_pTutorial;				//チュートリアル
	static CGame *m_pGame;						//ゲーム
	static CResult *m_pResult;					//リザルト
	static MODE m_mode;							//モード
};

#endif