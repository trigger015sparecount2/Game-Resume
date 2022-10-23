//--------------------------
//Author:三上航世
//背景の処理(bg.h)
//--------------------------
#ifndef _BG_H_
#define _BG_H_
#include "main.h"
//プロトタイプ宣言
void InitBg(void);
void UninitBg(void);
void UpdateBg(void);
void DrawBg(void);
void SetBg(int ntype, bool bscroll);
void ScrollBg(bool bscroll);
#endif _BG_H_
