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
class CTutorial;
class CGameOver;

class CManager
{
public:
	//モード
	typedef enum
	{
		MODE_TITLE,
		MODE_GAME,
		MODE_RESULT,
		MODE_TUTORIAL,
		MODE_GAMEOVER,
		MODE_MAX,
	}MODE;

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	//レンダラー、入力周りのポインタ取得
	static CRenderer *GetRenderer() { return m_pRenderer; }
	static CKeyboard *GetKeyboard() { return m_pKeyboard; }
	static CMouse *GetMouse() { return m_pMouse; }
	static CGamePad *GetGamepad();

	//モード格納・取得
	static void SetMode(MODE mode);
	static MODE GetMode() { return m_aMode; }

	//ゲーム終了のフラグ格納・取得
	static void SetGameEnd(bool bGameEnd) { m_bGameEnd = bGameEnd; }
	static bool GetGameEnd() { return m_bGameEnd; }


	//それぞれのモード渡すやつ
	static CTitle *GetTitle() { return m_pTitle; }
	static CGame *GetGame() { return m_pGame; }
	static CResult *GetResult() { return m_pResult; }
	static CTutorial *GetTutorial() { return m_pTutorial; }
	static CGameOver *GetGameOver() { return m_pGameOver; }


	//スコア計算用
	static int GetGameTime() { return m_nTime; }
	static void SetGameTime(int nScore) { m_nTime = nScore; }

	//ランキングのみかどうか
	static bool GetRankOnly() { return m_bRankOnly; }
	static void SetRankOnly(bool Set) { m_bRankOnly = Set; }

	//ランキング更新関係
	static bool GetSaveRankOne() { return m_bSaveRankOne; }
	static void SetSaveRankOne(bool Set) { m_bSaveRankOne = Set; }

	//ゲームクリア関係
	static bool GetGameClear() { return m_bGameClear; }
	static void SetGameClear(bool Set) { m_bGameClear = Set; }

	//Pause中かどうか何とかする
	static void SetPause(bool bFlag) { m_bPause = bFlag; }
	static bool GetPause() { return m_bPause; };

private:
	static CRenderer *m_pRenderer;	//レンダラー
	static CKeyboard *m_pKeyboard;	//キーボード
	static CMouse *m_pMouse;		//マウス
	static CGamePad *m_pGamepad;	//ゲームパッド

	static int m_nTime;		//ゲーム終了後のスコアを一時的に格納
	static bool m_bGameEnd;	//ゲーム終了したか

	static MODE m_aMode;	//現在のモード

	//渡すやつ
	static CTitle *m_pTitle;
	static CGame *m_pGame;
	static CResult *m_pResult;
	static CTutorial *m_pTutorial;
	static CGameOver *m_pGameOver;

	static bool m_bRankOnly;	//ランキング飲み
	static bool m_bSaveRankOne;	//ランキング一回だけ通すように

	static bool m_bGameClear;	//ゲームクリアしたかどうか
	static bool m_bPause;		//Pause中かどうか

};

#endif