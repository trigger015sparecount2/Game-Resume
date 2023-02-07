//--------------------------
//Author:三上航世
//アイテムの処理(prize.h)
//--------------------------
#ifndef _PRIZE_H_
#define _PRIZE_H_
#include "main.h"

//プロトタイプ宣言
void InitPrize(void);			//初期化
void UninitPrize(void);			//終了
void UpdatePrize(void);			//更新
void DrawPrize(void);			//描画
void SetPrize(D3DXVECTOR3 pos);	//設定
void ResetPrize(void);			//リセット
#endif _PRIZE_H_
