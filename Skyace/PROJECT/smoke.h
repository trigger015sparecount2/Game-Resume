//--------------------------
//Author:三上航世
//煙の処理(smoke.h)
//--------------------------
#ifndef _SMOKE_H_
#define _SMOKE_H_
#include "main.h"
//プロトタイプ宣言
void InitSmoke(void);
void UninitSmoke(void);
void UpdateSmoke(void);
void DrawSmoke(void);
void SetSmoke(D3DXVECTOR3 pos);
void ResetSmoke(void);
#endif _SMOKE_H_
