//---------------------------
//Author:三上航世
//壁(wall.h)
//---------------------------
#ifndef _WALL_H_
#define _WALL_H_
#include "main.h"
//プロトタイプ宣言
HRESULT InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void CollisionWall(D3DXVECTOR3 *pPos, float fRadius);
#endif _WALL_H_