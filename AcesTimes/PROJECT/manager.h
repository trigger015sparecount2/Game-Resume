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

//前方宣言
class CRenderer;
class CKeyboard;
class CGamePad;

class CTitle;
class CSelect;
class CSave;
class CLoad;
class CGame;
class CResult;

class CManager
{
public:

	typedef enum
	{
		MODE_TITLE,
		MODE_LOAD,
		MODE_SAVE,
		MODE_SELECT,
		MODE_GAME,
		MODE_RESULT,
		MODE_MAX,
	}MODE;

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw();
	static CRenderer *GetRenderer();
	static CKeyboard *GetKeyboard();
	static CGamePad *GetGamepad();

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
	static CSelect *GetSelect() { return m_pSelect; }
	static CLoad *GetLoad() { return m_pLoad; }
	static CSave *GetSave() { return m_pSave; }

	//Pause中かどうか何とかする
	static void SetPause(bool bFlag) { m_bPause = bFlag; }
	static bool GetPause() { return m_bPause; };

	static int GetMoney() { return m_nMoney; }
	static void SetMoney(int nMoney) { m_nMoney = nMoney; }

	static void ResetMoney() { m_nGetMoney = 0; }
	static void AddMoney(int nMoney) { m_nGetMoney += nMoney; }
	static int GetStageMoney() { return m_nGetMoney; }

	static void SetScore(int nScore) { m_nScore = nScore; }
	static int GetScore() { return m_nScore; }

	static bool GetAircraft(int nType) { return m_aAircraft[nType]; }
	static void SetAircraft(int nType,bool bAircraft) { m_aAircraft[nType] = bAircraft; }

	static int GetNumAircraft() { return m_nNumAircraft; }
	static void SetNumAircraft(int nNum) { m_nNumAircraft = nNum; }

	static int GetType() { return m_nType; }
	static void SetType(int nType) { m_nType = nType; }

	static int GetStage() { return m_nStage; }
	static void SetStage(int nStage) { m_nStage = nStage; }

private:
	static CRenderer *m_pRenderer;
	static CKeyboard *m_pKeyboard;
	static CGamePad *m_pGamepad;

	static bool m_bGameEnd;		//ゲーム終了したか

	static MODE m_aMode;		//現在のモード

	//渡すやつ
	static CTitle *m_pTitle;
	static CGame *m_pGame;
	static CResult *m_pResult;
	static CSelect *m_pSelect;
	static CLoad *m_pLoad;
	static CSave *m_pSave;

	static bool m_bPause;		//Pause中かどうか
	static bool m_bCountdown;

	static int m_nStage;		//ステージの種類
	static int m_nType;			//戦闘機のタイプ
	static int m_nMoney;		//所持金
	static int m_nScore;
	static int m_nGetMoney;
	static int m_nNumAircraft;
	static bool m_aAircraft[6];

};

#endif