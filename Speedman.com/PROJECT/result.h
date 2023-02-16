//=============================================================================
//リザルト管理[Result.h]
// Author : satou syusuke
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "manager.h"
#include "Scene.h"
//*****************************************************************************
//前方宣言
//*****************************************************************************
class CGamePad;

class CResult : public CScene
{
public:
	CResult();
	~CResult();

	HRESULT Init(D3DXVECTOR3 pos);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CResult *Create();	//作成

private:
	CKeyboard *m_pKeyboard;	//キーボード
	CGamePad *m_pGamePad;	//ゲームパッド

	static bool m_bFade;	//フェード
};
#endif // _RESULT_H_
