//=============================================================================
//
// トランポリン処理 [trampoline.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _TRAMPOLINE_H_
#define _TRAMPOLINE_H_

#include "main.h"
#include "plane.h"

class CTrampoline : public CPlane
{
public:
	CTrampoline(int nPriority);
	~CTrampoline();

	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CTrampoline *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos);	//作成

private:

};

#endif