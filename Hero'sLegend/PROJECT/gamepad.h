//--------------------------------------
//Author:三上航世
//ゲームパッドの入力情報(padinput.h)
//---------------------------------------
#ifndef _PADINPUT_H_
#define _PADINPUT_H_
#include "main.h"

//ボタンの種類
typedef enum
{
	DIP_X = 0,	//X
	DIP_Y,		//Y
	DIP_A,		//A
	DIP_B,		//B
	DIP_L1,		//LB
	DIP_R1,		//RB
	DIP_L2,		//LT
	DIP_R2,		//RT
	DIP_LS,		//左スティック
	DIP_RS,		//右スティック
	DIP_BACK,	//BACK
	DIP_START,	//START
	DIP_GUIDE,	//GUIDE
	DIP_MAX
}PadButton;

//プロトタイプ宣言
HRESULT InitGamepad(HINSTANCE hInstance, HWND hWnd);	//初期化
void UninitGamepad(void);	//終了
void UpdateGamepad(void);	//更新
float LeftStickX(void);		//左スティック横
float LeftStickY(void);		//左スティック縦
float CrossKey(void);		//十字キー
float RightStickX(void);	//右スティック横
float RightStickY(void);	//右スティック縦
bool GetGamePadPress(PadButton Button);		//ボタン押してる
bool GetGamePadTrigger(PadButton Button);	//ボタン押したまたは離れた
bool GetGamePadRelease(PadButton Button);	//ボタン離した
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef);	//コールバック関数
bool GamePad(void);							//ゲームパッド繋がっているか
bool GetAnyButton(void);					//ボタン何か押したか
bool GetGamePadButton(PadButton Button);	//ボタン押したか
#endif _PADINPUT_H_
