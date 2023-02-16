//=============================================================================
//
// フィールド処理 [field.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"
#include "plane.h"

class CField : public CPlane
{
public:
	CField(int nPriority);
	~CField();
	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR2 Tex);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CField *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos, int nTex, D3DXVECTOR2 Tex);	//作成

private:
	
};

#endif