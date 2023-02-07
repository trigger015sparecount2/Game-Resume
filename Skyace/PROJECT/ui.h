//------------------------
//Author:三上航世
//UI処理(ui.h)
//------------------------
#ifndef _UI_H_
#define _UI_H_
#include "main.h"

//プロトタイプ宣言
void InitUi(void);			//初期化
void UninitUi(void);		//終了
void UpdateUi(void);		//更新
void DrawUi(void);			//描画
void AddScore(int nValue);	//スコア加算
void AddKill(void);			//撃墜数加算
void ResetUi(void);			//リセット
int Score(void);			//スコア取得
int Kill(void);				//撃墜数取得
void NextStage(void);		//次のステージに移行処理
#endif _UI_H_
