//--------------------------------------
//Author:三上航世
//マウスの入力情報(mouse.h)
//---------------------------------------
#ifndef _MOUSE_H_
#define _MOUSE_H_
#include "main.h"

//マウスのボタン
typedef enum
{
	DIM_L = 0,	//左クリック
	DIM_R,		//右クリック
	DIM_W,		//ホイールボタン
	DIM_MAX
}MouseButton ;

//プロトタイプ宣言
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd);	//初期化
void UninitMouse(void);						//終了
void UpdateMouse(void);						//更新
bool GetMousePress(MouseButton Button);		//ボタン押してるか
bool GetMouseTrigger(MouseButton Button);	//ボタン押したまたは離れた
bool GetMouseRelease(MouseButton Button);	//ボタン離した
bool Mouse(void);	//マウス繋がっているか
LONG MouseX(void);	//マウス横移動
LONG MouseY(void);	//マウス縦移動
LONG MouseZ(void);	//マウスホイール
bool GetMouseButton(MouseButton Button);	//ボタン押したか
#endif _MOUSE_H_