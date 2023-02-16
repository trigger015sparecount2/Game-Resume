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

	HRESULT Init(D3DXVECTOR3 pos);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CGameOver *Create();	//作成
	
	//選択肢設定
	int GetSerectNum() { return m_SerectNam; }
	void SetSerectNum(int SerectNum) { m_SerectNam = SerectNum; }

private:
	static bool m_bFade;	//フェード
	CKeyboard *m_pKeyboard;	//キーボード
	CGamePad *m_pGamePad;	//ゲームパッド

	static int m_SerectNam;	//選択肢数字

};

#endif // _GAMEOVER_H_
