//---------------------------
//Author:三上航世
//爆発処理(explosion.h)
//---------------------------
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_
#include "main.h"
#define MAX_EXPLOSION (512)
typedef struct
{
	D3DXVECTOR3 pos; //位置
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	int nTime;
	float fTex;
	bool bUse;
} Explosion;
//プロトタイプ宣言
HRESULT InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos);
#endif _EXPLOSION_H_