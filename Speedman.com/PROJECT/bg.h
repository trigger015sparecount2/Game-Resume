//*****************************************************************************
// 背景関連処理(BG.h)
// Author : satou syusuke
//*****************************************************************************

#ifndef _BG_H_
#define _BG_H_

#include "Scene2D.h"

class CBG :public CScene2D
{
public:

	CBG(int nPriority);
	~CBG();

	HRESULT Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CBG *Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType);		//出現位置、サイズX、サイズY、テクスチャ）

private:

	D3DXVECTOR3 m_pos;		//現在位置

	bool m_bUninit;			//消すか

};
#endif