//--------------------------
//Author:三上航世
//煙の処理(smoke.h)
//--------------------------
#ifndef _SMOKE_H_
#define _SMOKE_H_
#include "main.h"

//プロトタイプ宣言
void InitSmoke(void);			//初期化
void UninitSmoke(void);			//終了
void UpdateSmoke(void);			//更新
void DrawSmoke(void);			//描画
void SetSmoke(D3DXVECTOR3 pos);	//設定
void ResetSmoke(void);			//リセット
#endif _SMOKE_H_
