//=============================================================================
//チュートリアル管理[Tutorial.h]
// Author : satou syusuke
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "manager.h"
#include "Scene.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CGamePad;

class CTutorial : public CScene
{
public:
	CTutorial();
	~CTutorial();

	HRESULT Init(D3DXVECTOR3 pos);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CTutorial *Create();	//作成

	//選択番号を何とかできるやつ
	static void SetSelectNum(int nSelectNum) { m_nSelectNum = nSelectNum; }
	static int GetSelectNum() { return m_nSelectNum; };

private:
	static bool m_bFade;		//フェード
	CKeyboard *m_pKeyboard;		//キーボード
	CGamePad *m_pGamePad;		//ゲームパッド

	static int m_nSelectNum;	//選択番号
};

#endif // _TUTORIAL_H_
