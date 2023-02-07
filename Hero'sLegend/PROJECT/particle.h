//---------------------------
//Author:三上航世
//パーティクル処理(particle.h)
//---------------------------
#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include "main.h"

#define MAX_PARTICLE (256)	//最大数

//パーティクルの構造体
typedef struct
{
	D3DXVECTOR3 pos; //位置
	D3DXVECTOR3 move;
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	int nTime;
	bool bUse;
} Particle;

//プロトタイプ宣言
HRESULT InitParticle(void);			//初期化
void UninitParticle(void);			//終了
void UpdateParticle(void);			//更新
void DrawParticle(void);			//描画
void SetParticle(D3DXVECTOR3 pos);	//設定
#endif _PARTICLE_H_