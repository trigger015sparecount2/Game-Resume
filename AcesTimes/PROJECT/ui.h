//*****************************************************************************
// UI関連処理(UI.h)
// Author : 三上航世
//*****************************************************************************

#ifndef _UI_H_
#define _UI_H_

#include "Scene2d.h"

class CUI :public CScene2D
{
public:
	CUI(PRIORITY Priority);
	~CUI();
	
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nType, D3DXCOLOR col, D3DXVECTOR3 rot);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画
	OBJTYPE GetObjType() { return OBJECTTYPE_UI; }	//種類

	static CUI *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nType, D3DXCOLOR col, D3DXVECTOR3 rot);	//作成
private:
	
};
#endif