//--------------------------
//Author:三上航世
//爆発の処理(explosion.h)
//--------------------------
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_
#include "main.h"

//プロトタイプ宣言
void InitExplosion(void);			//初期化
void UninitExplosion(void);			//終了
void UpdateExplosion(void);			//更新
void DrawExplosion(void);			//描画
void SetExplosion(D3DXVECTOR3 pos);	//設定
void ResetExplosion(void);			//リセット
#endif _EXPLOSION_H_
