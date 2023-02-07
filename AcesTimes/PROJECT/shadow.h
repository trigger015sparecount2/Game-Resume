//=============================================================================
//
// 影処理 [shadow.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "scene2d.h"

class CShadow : public CScene2D
{
public:
	CShadow(PRIORITY Priority);
	~CShadow();
	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CShadow *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos, int nType);	//作成

private:
	
};

#endif