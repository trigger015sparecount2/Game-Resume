//--------------------------
//Author:三上航世
//ミサイルの処理(missile.h)
//--------------------------
#ifndef _MISSILE_H_
#define _MISSILE_H_
#include "main.h"

//プロトタイプ宣言
void InitMissile(void);		//初期化
void UninitMissile(void);	//終了
void UpdateMissile(void);	//更新
void DrawMissile(void);		//描画
void SetMissile(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType);	//設定
void ResetMissile(void);	//リセット
#endif _MISSILE_H_
