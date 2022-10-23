//=============================================================================
//ゲーム管理[Game.h]
// Author : satou syusuke
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "manager.h"
#include "Scene.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CGamePad;

class CGame : public CScene
{
public:
	CGame(int nPriority);
	~CGame();

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();

	static CGame *Create();

	static void SetCountFlag(bool bFlag);
	static bool GetCountFlag() { return m_bCountFlag; };

	//選択番号を何とかできるやつ
	static void SetSelectNum(int m_SelectNum);
	static int GetSelectNum() { return m_SelectNum; };

private:
	static bool m_bFade;
	CKeyboard *m_pKeyboard;
	CGamePad *m_pGamePad;

	static bool m_bCountFlag;		//カウントダウン中かどうか
	static int m_SelectNum;			//選択番号
};

#endif // _GAME_H_
