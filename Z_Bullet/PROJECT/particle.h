//---------------------------
//Author:三上航世
//パーティクル処理(particle.h)
//---------------------------
#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include "main.h"
//プロトタイプ宣言
HRESULT InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, float fHeight);
void ResetParticle(void);
#endif _PARTICLE_H_