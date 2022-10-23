//------------------------
//Author:三上航世
//UI処理(gameui.h)
//------------------------
#ifndef _GAMEUI_H_
#define _GAMEUI_H_
#include "main.h"
//マクロ定義
#define TIME_LIMIT_MIN (60)   //制限時間（分）
#define TIME_LIMIT_SEC (60)   //制限時間（秒）
//プロトタイプ宣言
void InitGameUi(void);
void UninitGameUi(void);
void UpdateGameUi(void);
void DrawGameUi(void);
void ResetGameUi(void);
int TimeMin(void);
int TimeSec(void);
void StopTime(void);
bool GameOver(void);
void GameClear(void);
#endif _GAMEUI_H_
