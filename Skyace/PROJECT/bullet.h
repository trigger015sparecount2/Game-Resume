//--------------------------
//Author:三上航世
//弾丸の処理(bullet.h)
//--------------------------
#ifndef _BULLET_H_
#define _BULLET_H_
#include "main.h"
//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int ntype, bool bUav);
void ResetBullet(void);
#endif _BULLET_H_
