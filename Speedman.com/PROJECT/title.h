//=============================================================================
// タイトル管理[Title.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "manager.h"
#include "Scene.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CGamePad;
class CKeyboard;

class CTitle : public CScene
{
public:
	CTitle();
	~CTitle();

	HRESULT Init(D3DXVECTOR3 pos);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CTitle *Create();	//作成

	//選んだ番号設定
	int GetSerectNum() { return m_nSelectNum; }
	void SetSerectNum(int SelectNum) { m_nSelectNum = SelectNum; }

private:
	static int m_nSelectNum;	//選んだ番号
	static bool m_bFade;		//フェード
	CKeyboard *m_pKeyboard;		//キーボード
	CGamePad *m_pGamePad;		//ゲームパッド

};

#endif // _TITLE_H_
