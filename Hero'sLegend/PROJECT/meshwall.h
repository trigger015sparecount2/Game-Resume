//---------------------------
//Author:三上航世
//メッシュウォール(meshwall.h)
//---------------------------
#ifndef _MESHWALL_H_
#define _MESHWALL_H_
#include "main.h"

//プロトタイプ宣言
HRESULT InitMeshwall(void);	//初期化
void UninitMeshwall(void);	//終了
void UpdateMeshwall(void);	//更新
void DrawMeshwall(void);	//描画
#endif _MESHWALL_H_