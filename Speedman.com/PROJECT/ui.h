//*****************************************************************************
// UI関連処理(UI.h)
// Author : 佐藤秀亮
//*****************************************************************************

#ifndef _UI_H_
#define _UI_H_

#include "Scene2D.h"

class CUI :public CScene2D
{
public:

	CUI(int nPriority);
	~CUI();

	HRESULT Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CUI *Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType);	//作成
private:

	D3DXVECTOR3 m_pos;			//現在位置

	D3DCOLORVALUE m_Color;		//カラー
	bool m_bUninit;				//消すか


	//大きさ情報
	float m_fSizeX;
	float m_fSizeY;

};
#endif