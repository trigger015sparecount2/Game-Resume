//--------------------------
//Author:三上航世
//リザルト画面の処理(result.h)
//--------------------------
#ifndef _RESULT_H_
#define _RESULT_H_
#include "main.h"

//プロトタイプ宣言
void InitResult(void);			//初期化
void UninitResult(void);		//終了
void UpdateResult(void);		//更新
void DrawResult(void);			//描画
void Demoresult(bool bDemo);	//デモか設定する
bool Demo(void);				//デモか取得
int ResultTime(void);			//時間取得
#endif _RESULT_H_
