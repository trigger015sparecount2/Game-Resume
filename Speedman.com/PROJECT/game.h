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

	HRESULT Init(D3DXVECTOR3 pos);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CGame *Create();	//作成

	//カウントダウンフラグ設定
	static void SetCountFlag(bool bFlag) { m_bCountFlag = bFlag; }
	static bool GetCountFlag() { return m_bCountFlag; };

	//選択番号を何とかできるやつ
	static void SetSelectNum(int nSelectNum) { m_SelectNum = nSelectNum; }
	static int GetSelectNum() { return m_SelectNum; };

private:
	static bool m_bFade;	//フェードしてるか
	CKeyboard *m_pKeyboard;	//キーボード
	CGamePad *m_pGamePad;	//ゲームパッド

	static bool m_bCountFlag;	//カウントダウン中かどうか
	static int m_SelectNum;		//選択番号
};

#endif // _GAME_H_
