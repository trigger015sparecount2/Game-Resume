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
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();

	bool GetPress(PadButton Button) { return (m_aPadState.rgbButtons[Button] & 0x80) ? true : false; }			//プレス
	bool GetAnyButton();	//何かボタン押した
	bool GetTrigger(PadButton Button){ return (m_aPadStateTrigger.rgbButtons[Button] & 0x80) ? true : false; }	//トリガー
	bool GetRelease(PadButton Button){ return (m_aPadStateRelease.rgbButtons[Button] & 0x80) ? true : false; }	//リリース
	bool GetButton(PadButton Button);	//ボタン押した
	float LeftStickX();			//左スティック横
	float LeftStickY();			//左スティック縦
	float TriggerCrossKey();	//十字キー押した
	float PressCrossKey();		//十字キー押されてる
	float RightStickX();		//右スティック横
	float RightStickY();		//右スティック縦
	static bool GetGamePad() { return m_bGamePad; }	//ゲームパッド繋がってる

private:
	static bool m_bGamePad;			//ゲームパッド繋がってるか
	DIJOYSTATE m_aPadState;			//ゲームパッドの入力情報(プレス情報)
	DIJOYSTATE m_aPadStateTrigger;	//ゲームパッドの入力情報(トリガー情報)
	DIJOYSTATE m_aPadStateRelease;	//ゲームパッドの入力情報(リリース情報)
};
#endif