//=============================================================================
//
// 入力処理 [gamepad.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_
#include "main.h"
#include "input.h"

class CGamePad : CInput
{
public:
	//ゲームパッドボタン
	typedef enum
	{
		DIP_X = 0,
		DIP_Y,
		DIP_A,
		DIP_B,
		DIP_L1,
		DIP_R1,
		DIP_L2,
		DIP_R2,
		DIP_LS,
		DIP_RS,
		DIP_BACK,
		DIP_START,
		DIP_GUIDE,
		DIP_MAX
	} PadButton;

	CGamePad();
	~CGamePad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	bool GetPress(PadButton Button) { return (m_aPadState.rgbButtons[Button] & 0x80) ? true : false; }			//ボタン押してるか
	bool GetAnyButton();				//何かボタン押されたか
	bool GetTrigger(PadButton Button) { return (m_aPadStateTrigger.rgbButtons[Button] & 0x80) ? true : false; }	//ボタンの状態が変わったか
	bool GetRelease(PadButton Button) { return (m_aPadStateRelease.rgbButtons[Button] & 0x80) ? true : false; }	//
	bool GetButton(PadButton Button);	//ボタン押されたか
	float LeftStickX();					//左スティック横
	float LeftStickY();					//左スティック縦
	float TriggerCrossKey();			//十字キー押された
	float PressCrossKey();				//十字キー押されてるか
	float RightStickX();				//右スティック横
	float RightStickY();				//右スティック縦
	static bool GetGamePad() { return m_bGamePad; }	//ゲームパッド繋がってる

private:
	static bool m_bGamePad;			//ゲームパッド繋がってるか
	DIJOYSTATE m_aPadState;			//ゲームパッドの入力情報(プレス情報)
	DIJOYSTATE m_aPadStateTrigger;	//ゲームパッドの入力情報(トリガー情報)
	DIJOYSTATE m_aPadStateRelease;	//ゲームパッドの入力情報(リリース情報)
};
#endif