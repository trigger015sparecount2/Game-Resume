//--------------------------
//Author:三上航世
//やられたときの処理(death.h)
//--------------------------
#ifndef _DEATH_H_
#define _DEATH_H_
#include "main.h"

//プロトタイプ宣言
void InitDeath(void);	//初期化
void UninitDeath(void);	//終了
void UpdateDeath(void);	//更新
void DrawDeath(void);	//描画
void SetDeath(D3DXVECTOR3 pos, bool bSound);	//設定
void ResetDeath(void);	//リセット
#endif _DEATH_H_
