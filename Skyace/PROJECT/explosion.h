//--------------------------
//Author:三上航世
//爆発の処理(explosion.h)
//--------------------------
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_
#include "main.h"
//プロトタイプ宣言
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos);
void ResetExplosion(void);
#endif _EXPLOSION_H_
