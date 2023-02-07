//---------------------------
//Author:三上航世
//ライト(light.h)
//---------------------------
#ifndef _LIGHT_H_
#define _LIGHT_H_
#include "main.h"

//プロトタイプ宣言
HRESULT InitLight(void);	//初期化
void UninitLight(void);		//終了
void UpdateLight(void);		//更新
void SetLight(int nCntLight, D3DXCOLOR col, D3DXVECTOR3 vec);	//設定
#endif _LIGHT_H_