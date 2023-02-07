//--------------------------
//Author:三上航世
//レーザーの処理(laser.h)
//--------------------------
#ifndef _LASER_H_
#define _LASER_H_
#include "main.h"

//プロトタイプ宣言
void InitLaser(void);	//初期化
void UninitLaser(void);	//終了
void UpdateLaser(void);	//更新
void DrawLaser(void);	//描画
void SetLaser(D3DXVECTOR3 pos, int ntype);	//設定
void ResetLaser(void);	//リセット
#endif _LASER_H_

