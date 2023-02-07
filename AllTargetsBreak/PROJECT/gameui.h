//------------------------
//Author:三上航世
//UI処理(gameui.h)
//------------------------
#ifndef _GAMEUI_H_
#define _GAMEUI_H_
#include "main.h"

//マクロ定義
#define TIME_LIMIT_SEC (60)	//制限時間（秒）
#define TIME_LIMIT_COM (60)	//制限時間（コンマ）

//プロトタイプ宣言
void InitGameUi(void);		//初期化
void UninitGameUi(void);	//終了
void UpdateGameUi(void);	//更新
void DrawGameUi(void);		//描画
void ResetGameUi(void);		//UIリセット
int TimeMin(void);			//時間取得(分)
int TimeSec(void);			//時間取得(秒)
void StopTime(void);		//時間停止
bool GameOver(void);		//ゲームオーバー取得
void GameClear(void);		//ゲームクリア―
#endif _GAMEUI_H_
