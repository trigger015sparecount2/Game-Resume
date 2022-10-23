//--------------------------
//Author:三上航世
//味方無人機の処理(uav.h)
//--------------------------
#ifndef _UAV_H_
#define _UAV_H_
#include "main.h"
//プロトタイプ宣言
void InitUav(void);
void UninitUav(void);
void UpdateUav(D3DXVECTOR3 pos, int nLag);
void DrawUav(void);
void SetUav(D3DXVECTOR3 pos, D3DXVECTOR3 move);
void ResetUav(void);
#endif _UAV_H_
