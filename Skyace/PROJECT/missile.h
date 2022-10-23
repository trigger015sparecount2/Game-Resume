//--------------------------
//Author:三上航世
//ミサイルの処理(missile.h)
//--------------------------
#ifndef _MISSILE_H_
#define _MISSILE_H_
#include "main.h"
//プロトタイプ宣言
void InitMissile(void);
void UninitMissile(void);
void UpdateMissile(void);
void DrawMissile(void);
void SetMissile(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType);
void ResetMissile(void);
#endif _MISSILE_H_
