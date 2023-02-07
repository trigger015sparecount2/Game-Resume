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
bool GetKeyboardPress(int nKey);	//キー押してるか
bool GetKeyboardTrigger(int nKey);	//キー押されたか離された
bool GetKeyboardRelease(int nKey);	//キー離された
bool GetAnyKey(void);				//キー何か押したか
bool GetKeyboardKey(int nKey);		//キー押されたか
#endif _INPUT_H_