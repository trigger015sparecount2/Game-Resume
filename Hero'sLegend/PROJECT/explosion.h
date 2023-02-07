//---------------------------
//Author:三上航世
//爆発処理(explosion.h)
//---------------------------
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_
#include "main.h"
#define MAX_EXPLOSION (512)

//爆発エフェクトの構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nTime;				//時間
	float fTex;				//テクスチャ座標
	bool bUse;				//使用中か
} Explosion;

//プロトタイプ宣言
HRESULT InitExplosion(void);		//初期化
void UninitExplosion(void);			//終了
void UpdateExplosion(void);			//更新
void DrawExplosion(void);			//描画
void SetExplosion(D3DXVECTOR3 pos);	//爆発設定
#endif _EXPLOSION_H_