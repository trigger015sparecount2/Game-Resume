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
	//モードの種類
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

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画
	static CRenderer *GetRenderer() { return m_pRenderer; }	//レンダラー返す
	static CKeyboard *GetKeyboard() { return m_pKeyboard; }	//キーボード返す
	static CGamePad *GetGamepad();							//ゲームパッド返す

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

	//金の格納・取得
	static int GetMoney() { return m_nMoney; }
	static void SetMoney(int nMoney) { m_nMoney = nMoney; }

	//ステージ中の金のリセット、増加、取得
	static void ResetMoney() { m_nGetMoney = 0; }
	static void AddMoney(int nMoney) { m_nGetMoney += nMoney; }
	static int GetStageMoney() { return m_nGetMoney; }

	//スコアの格納・取得
	static void SetScore(int nScore) { m_nScore = nScore; }
	static int GetScore() { return m_nScore; }

	//戦闘機の使用状態の格納・取得
	static bool GetAircraft(int nType) { return m_aAircraft[nType]; }
	static void SetAircraft(int nType,bool bAircraft) { m_aAircraft[nType] = bAircraft; }

	//使用可能な戦闘機の数の格納・取得
	static int GetNumAircraft() { return m_nNumAircraft; }
	static void SetNumAircraft(int nNum) { m_nNumAircraft = nNum; }

	//戦闘機の種類の格納・取得
	static int GetType() { return m_nType; }
	static void SetType(int nType) { m_nType = nType; }

	//ステージの種類の格納・取得
	static int GetStage() { return m_nStage; }
	static void SetStage(int nStage) { m_nStage = nStage; }

private:
	static CRenderer *m_pRenderer;	//レンダラー
	static CKeyboard *m_pKeyboard;	//キーボード
	static CGamePad *m_pGamepad;	//ゲームパッド

	static bool m_bGameEnd;			//ゲーム終了したか

	static MODE m_aMode;			//現在のモード

	//各モード
	static CTitle *m_pTitle;
	static CGame *m_pGame;
	static CResult *m_pResult;
	static CSelect *m_pSelect;
	static CLoad *m_pLoad;
	static CSave *m_pSave;

	static bool m_bPause;		//Pause中かどうか

	static int m_nStage;		//ステージの種類
	static int m_nType;			//戦闘機のタイプ
	static int m_nMoney;		//所持金
	static int m_nScore;		//スコア
	static int m_nGetMoney;		//ステージ中で獲得した金
	static int m_nNumAircraft;	//使える戦闘機の数
	static bool m_aAircraft[6];	//どの戦闘機使えるか

};

#endif