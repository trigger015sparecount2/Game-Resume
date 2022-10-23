//---------------------------
//Author:三上航世
//残像処理(orbit.h)
//---------------------------
#ifndef _ORBIT_H_
#define _ORBIT_H_
#include "main.h"
#define MAX_ORBIT (256)
//プロトタイプ宣言
HRESULT InitOrbit(void);
void UninitOrbit(void);
void UpdateOrbit(void);
void DrawOrbit(void);
void SetOrbit(D3DXVECTOR3 pos, D3DXVECTOR3 Tippos, D3DXVECTOR3 posOld, D3DXVECTOR3 TipposOld, int nTime);
#endif _ORBIT_H_