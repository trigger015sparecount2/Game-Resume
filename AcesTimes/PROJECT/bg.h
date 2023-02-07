//=============================================================================
//
// 背景処理 [bg.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_
#include "main.h"
#include "scene2d.h"

class CBg : public CScene2D
{
public:
	CBg(PRIORITY Priority);
	~CBg();
	HRESULT Init(D3DXVECTOR2 size, D3DXVECTOR3 pos, D3DXVECTOR2 TexSize);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画

	static CBg *Create(D3DXVECTOR2 size, D3DXVECTOR3 pos, D3DXVECTOR2 TexSize, int nTex);	//作成

private:
	D3DXVECTOR2 m_TexSize[2];	//テクスチャ座標
	float m_fMaxTex;			//テクスチャスクロール
};

#endif