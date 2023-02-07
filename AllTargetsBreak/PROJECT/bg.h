//--------------------------
//Author:三上航世
//背景の処理(bg.h)
//--------------------------
#ifndef _BG_H_
#define _BG_H_
#include "main.h"

//プロトタイプ宣言
void InitBg(void);		//初期化
void UninitBg(void);	//終了
void UpdateBg(void);	//更新
void DrawBg(void);		//描画
void SetBg(int ntype, bool bscroll);	//背景設定
void ScrollBg(bool bscroll);			//スクロール設定
#endif _BG_H_
