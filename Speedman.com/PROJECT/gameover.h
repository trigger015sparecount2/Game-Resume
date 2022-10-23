//=============================================================================
//ゲームオーバー処理[GameOver.h]
// Author : satou syusuke
//=============================================================================
#ifndef _GAMEOVER_H_
#define _GAMEOVER_H_

#include "manager.h"
#include "Scene.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CGamePad;

class CGameOver : public CScene
{
public:
	CGameOver();
	~CGameOver();

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();

	static CGameOver *Create();

	int GetSerectNum() { return m_SerectNam; }
	void SetSerectNum(int SerectNum);

private:
	static bool m_bFade;
	CKeyboard *m_pKeyboard;
	CGamePad *m_pGamePad;

	static int m_SerectNam;

};

#endif // _GAMEOVER_H_
