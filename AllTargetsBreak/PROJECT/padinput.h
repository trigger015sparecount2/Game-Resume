//--------------------------------------
//Author:三上航世
//ゲームパッドの入力情報(padinput.h)
//---------------------------------------
#ifndef _PADINPUT_H_
#define _PADINPUT_H_
#include "main.h"

//プロトタイプ宣言
HRESULT InitGamepad(HINSTANCE hInstance, HWND hWnd);	//初期化
void UninitGamepad(void);				//終了
void UpdateGamepad(void);				//更新
float LeftStickX(void);					//左スティック横
float LeftStickY(void);					//左スティック縦
float CrossKey(void);					//十字キー
float RightStickX(void);				//右スティック横
float RightStickY(void);				//右スティック縦
bool GetGamePadPress(int nButton);		//プレス情報
bool GetGamePadTrigger(int nButton);	//トリガー情報
bool GetGamePadRelease(int nButton);	//リリース情報
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef);	//コールバック関数
bool GamePad(void);					//繋がっているか取得
bool GetAnyButton(void);			//何か押されたか
bool GetGamePadButton(int nButton);	//押されたか
#endif _PADINPUT_H_
