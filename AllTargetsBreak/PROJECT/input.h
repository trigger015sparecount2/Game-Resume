//--------------------------------------
//Author:三上航世
//キーボードの入力情報(input.h)
//---------------------------------------
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

//プロトタイプ宣言
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);	//初期化
void UninitKeyboard(void);			//終了
void UpdateKeyboard(void);			//更新
bool GetKeyboardPress(int nKey);	//プレス情報
bool GetKeyboardTrigger(int nKey);	//トリガー情報
bool GetKeyboardRelease(int nKey);	//リリース情報
bool GetAnyKey(void);				//何か押されたか
bool GetKeyboardKey(int nKey);		//押された情報
#endif _INPUT_H_