//=============================================================================
//開始UI管理[startui.h]
// Author : 三上　航世
//=============================================================================
#ifndef _STARTUI_H_
#define _STARTUI_H_

#include "ui.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CStartUI : public CUI
{
public:
	CStartUI(PRIORITY Priority);
	~CStartUI();

	HRESULT Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, D3DXCOLOR col, int nUIType);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CStartUI *Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, D3DXCOLOR col, int nUIType);	//作成

private:
	int m_nUIType;		//UIの種類
	int m_nTime;		//作成してからのカウント
	bool m_bStop;		//止める
	int m_nFade;		//透明度
	D3DXVECTOR2 size;	//サイズ
};

#endif
