//------------------------
//Author:三上航世
//UI処理(ui.h)
//------------------------
#ifndef _UI_H_
#define _UI_H_
#include "main.h"
//プロトタイプ宣言
void InitUi(void);
void UninitUi(void);
void UpdateUi(void);
void DrawUi(void);
void AddScore(int nValue);
void AddKill(void);
void ResetUi(void);
int Score(void);
int Kill(void);
void NextStage(void);
#endif _UI_H_
