//=============================================================================
//
// シーン処理 [light.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

class CLight
{
public:
	CLight();
	~CLight();
	HRESULT Init(D3DXCOLOR col, D3DXVECTOR3 vec, int nCntLight);	//初期化
	void Uninit();	//終了
	void Update();	//更新

	static CLight *Create(D3DXCOLOR col, D3DXVECTOR3 vec, int nCntLight);	//作成

private:
	D3DLIGHT9 m_light[8];	//光
};

#endif