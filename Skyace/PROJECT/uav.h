//--------------------------
//Author:三上航世
//味方無人機の処理(uav.h)
//--------------------------
#ifndef _UAV_H_
#define _UAV_H_
#include "main.h"

//プロトタイプ宣言
void InitUav(void);		//初期化
void UninitUav(void);	//終了
void UpdateUav(D3DXVECTOR3 pos, int nLag);		//更新
void DrawUav(void);		//描画
void SetUav(D3DXVECTOR3 pos, D3DXVECTOR3 move);	//設定
void ResetUav(void);	//リセット
#endif _UAV_H_
