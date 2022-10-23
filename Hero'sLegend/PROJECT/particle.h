//---------------------------
//Author:三上航世
//パーティクル処理(particle.h)
//---------------------------
#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include "main.h"
#define MAX_PARTICLE (256)
typedef struct
{
	D3DXVECTOR3 pos; //位置
	D3DXVECTOR3 move;
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	int nTime;
	bool bUse;
} Particle;
//プロトタイプ宣言
HRESULT InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos);
#endif _PARTICLE_H_