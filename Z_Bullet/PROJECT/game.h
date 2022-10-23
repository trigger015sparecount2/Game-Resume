//--------------------------
//Author:三上航世
//ゲーム画面の処理(game.h)
//--------------------------
#ifndef _GAME_H_
#define _GAME_H_
#include "main.h"
//プロトタイプ宣言
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
bool GetTuto(void);
bool GetStop(void);
void ResetGame();
#endif _GAME_H_