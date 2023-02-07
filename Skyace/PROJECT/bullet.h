//--------------------------
//Author:三上航世
//弾丸の処理(bullet.h)
//--------------------------
#ifndef _BULLET_H_
#define _BULLET_H_
#include "main.h"

//プロトタイプ宣言
void InitBullet(void);		//初期化
void UninitBullet(void);	//終了
void UpdateBullet(void);	//更新
void DrawBullet(void);		//描画
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int ntype, bool bUav);	//設定
void ResetBullet(void);		//リセット
#endif _BULLET_H_
