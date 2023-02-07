//--------------------------
//Author:三上航世
//的の処理(target.h)
//--------------------------
#ifndef _TARGET_H_
#define _TARGET_H_
#include "main.h"

//プロトタイプ宣言
void InitTarget(void);				//初期化
void UninitTarget(void);			//終了
void UpdateTarget(void);			//更新
void DrawTarget(void);				//描画
void SetTarget(D3DXVECTOR3 pos);	//的設定
bool BreakTarget(D3DXVECTOR3 pos, float fHeight, float fWidth);	//的が壊されたか
void ResetTarget(void);				//的全部消す
int CountTarget(void);				//的の数
#endif _TARGET_H_
