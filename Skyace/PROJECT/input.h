//--------------------------------------
//Author:三上航世
//ポリゴンの移動(input.h)
//---------------------------------------
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

//プロトタイプ宣言
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);	//初期化
void UninitKeyboard(void);			//終了
void UpdateKeyboard(void);			//更新
bool GetKeyboardPress(int nKey);	//プレス処理
bool GetKeyboardTrigger(int nKey);	//トリガー処理
bool GetKeyboardRelease(int nKey);	//リリース処理
#endif _INPUT_H_

