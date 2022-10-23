//---------------------------
//Author:三上航世
//メッシュフィールド(mesufield.h)
//---------------------------
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_
#include "main.h"
//プロトタイプ宣言
HRESULT InitMeshfield(void);
void UninitMeshfield(void);
void UpdateMeshfield(void);
void DrawMeshfield(void);
bool CollisionField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadius);
#endif _MESHFIELD_H_