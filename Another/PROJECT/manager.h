//=============================================================================
//
// マネージャ [Manager.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "main.h"
//=============================================================================
//マクロ
//=============================================================================
#define TIME_LIMIT (300)
#define MAX_LIFE (10)

//前方宣言
class CRenderer;
class CKeyboard;
class CMouse;
class CGamePad;
class CTitle;
class CGame;
class CResult;
//class CTutorial;
//class CGameOver;

class CManager
{
public:
	//モードの種類
	typedef enum
	{
		MODE_TITLE,
		MODE_GAME,
		MODE_RESULT,
		//MODE_TUTORIAL,
		//MODE_GAMEOVER,
		MODE_MAX,
	}MODE;

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画
	static CRenderer *GetRenderer() { return m_pRenderer; }	//レンダラー取得
	static CKeyboard *GetKeyboard() { return m_pKeyboard; }	//キーボード取得
	static CMouse *GetMouse() { return m_pMouse; }			//マウス取得
	static CGamePad *GetGamepad();							//ゲームパッド取得

	//モード格納・取得
	static void SetMode(MODE mode);
	static MODE GetMode() { return m_aMode; }

	//ゲーム終了のフラグ格納・取得
	static void SetGameEnd(bool GameEnd) { m_bGameEnd = GameEnd; }
	static bool GetGameEnd() { return m_bGameEnd; }


	//それぞれのモード渡すやつ
	static CTitle *GetTitle() { return m_pTitle; }
	static CGame *GetGame() { return m_pGame; }
	static CResult *GetResult() { return m_pResult; }
	/*static CTutorial *GetTutorial() { return m_pTutorial; }
	static CGameOver *GetGameOver() { return m_pGameOver; }*/

	//ゲームクリア関係
	static bool GetGameClear() { return m_bGameClear; }
	static void SetGameClear(bool Set) { m_bGameClear = Set; }

	//Pause中かどうか何とかする
	static void SetPause(bool bFlag) { m_bPause = bFlag; }
	static bool GetPause() { return m_bPause; };

	//カウントダウン関連
	static void SetCountdown(bool bCountdown) { m_bCountdown = bCountdown; }
	static bool GetCountdown() { return m_bCountdown; }

	//クリア後のストップ
	static void SetEnd(bool bFlag) { m_bEnd = bFlag; }
	static bool GetEnd() { return m_bEnd; };

private:
	static CRenderer *m_pRenderer;	//レンダラー
	static CKeyboard *m_pKeyboard;	//キーボード
	static CMouse *m_pMouse;		//マウス
	static CGamePad *m_pGamepad;	//ゲームパッド

	static int m_nTime;			//ゲーム終了後のスコアを一時的に格納
	static bool m_bGameEnd;		//ゲーム終了したか

	static MODE m_aMode;		//現在のモード

	//渡すやつ
	static CTitle *m_pTitle;
	static CGame *m_pGame;
	static CResult *m_pResult;
	/*static CTutorial *m_pTutorial;
	static CGameOver *m_pGameOver;*/

	static bool m_bGameClear;	//ゲームクリアしたかどうか
	static bool m_bPause;		//Pause中かどうか
	static bool m_bEnd;			//終わったか
	static bool m_bCountdown;	//カウントダウンか

};

#endif