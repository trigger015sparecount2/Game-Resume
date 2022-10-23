//---------------------------
//Author:三上航世
//フィールド(field.h)
//---------------------------
#ifndef _FIELD_H_
#define _FIELD_H_
#include "main.h"
//プロトタイプ宣言
HRESULT InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);
bool LandField(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, D3DXVECTOR3 *pMove, float fRadius);
#endif _FIELD_H_