//--------------------------
//Author:三上航世
//フェードアウトの処理(fade.h)
//--------------------------
#ifndef _FADE_H_
#define _FADE_H_
#include "main.h"

//フェードの状態
typedef enum
{
	FADE_NONE = 0,
	FADE_IN,
	FADE_OUT,
	FADE_MAX
} FADE;

//プロトタイプ宣言
void InitFade(MODE modeNext);	//初期化
void UninitFade(void);			//終了
void UpdateFade(void);			//更新
void DrawFade(void);			//描画
void SetFade(MODE modeNext);	//フェード設定
FADE GetFade(void);				//フェードの状態取得
#endif _FADE_H_