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

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nType, D3DXCOLOR col, D3DXVECTOR3 rot);
	void Uninit();
	void Update();
	void Draw();

	static CPauseUI *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nType, D3DXCOLOR col, D3DXVECTOR3 rot);

private:

};

#endif
