//=============================================================================
//ポーズ管理[pauseui.h]
// Author : 三上　航世
//=============================================================================
#ifndef _PAUSEUI_H_
#define _PAUSEUI_H_

#include "ui.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CPauseUI : public CUI
{
public:
	CPauseUI(PRIORITY Priority);
	~CPauseUI();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nType, D3DXCOLOR col, D3DXVECTOR3 rot);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CPauseUI *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nType, D3DXCOLOR col, D3DXVECTOR3 rot);	//作成

private:

};

#endif
