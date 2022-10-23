//--------------------------------------
//Author:三上航世
//マウスの入力情報(mouse.h)
//---------------------------------------
#ifndef _MOUSE_H_
#define _MOUSE_H_
#include "main.h"
typedef enum
{
	DIM_L = 0, //左クリック
	DIM_R,     //右クリック
	DIM_W,      //ホイールボタン
	DIM_MAX
}MouseButton;
//プロトタイプ宣言
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);
bool GetMousePress(MouseButton Button);
bool GetMouseTrigger(MouseButton Button);
bool GetMouseRelease(MouseButton Button);
bool Mouse(void);
float MouseX(void);
float MouseY(void);
float MouseZ(void);
bool GetMouseButton(MouseButton Button);
#endif _MOUSE_H_