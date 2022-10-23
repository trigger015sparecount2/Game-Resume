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
void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);
void SetFade(MODE modeNext, bool bSetMode);
#endif _FADE_H_