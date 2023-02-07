//--------------------------
//Author:三上航世
//手裏剣の処理(cross.h)
//--------------------------
#ifndef _CROSS_H_
#define _CROSS_H_
#include "main.h"

//手裏剣の方向
typedef enum
{
	CROSS_UP = 0,
	CROSS_RIGHT,
	CROSS_DOWN,
	CROSS_LEFT,
	MAX_CROSS_ROT
} CROSSROT;

//プロトタイプ宣言
void InitCross(void);	//初期化
void UninitCross(void);	//終了
void UpdateCross(void);	//更新
void DrawCross(void);	//描画
void SetCross(D3DXVECTOR3 pos, CROSSROT CrossRot);	//手裏剣設定
void ResetCross(void);	//全部消す
#endif _CROSS_H_
