//--------------------------
//Author:三上航世
//的の処理(target.h)
//--------------------------
#ifndef _TARGET_H_
#define _TARGET_H_
#include "main.h"
//プロトタイプ宣言
void InitTarget(void);
void UninitTarget(void);
void UpdateTarget(void);
void DrawTarget(void);
void SetTarget(D3DXVECTOR3 pos);
bool BreakTarget(D3DXVECTOR3 pos, float fHeight, float fWidth);
void ResetTarget(void);
int CountTarget(void);
#endif _TARGET_H_
