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
	bool GetPress(PadButton Button);
	bool GetAnyButton();
	bool GetTrigger(PadButton Button);
	bool GetRelease(PadButton Button);
	bool GetButton(PadButton Button);
	float LeftStickX();
	float LeftStickY();
	float TriggerCrossKey();	//上から時計回りに0.0f,4500.0f,9000.0f,13500.0f,18000.0f,22500.0f,27000.0f,31500.0f
	float PressCrossKey();		//上から時計回りに0.0f,4500.0f,9000.0f,13500.0f,18000.0f,22500.0f,27000.0f,31500.0f
	float RightStickX();
	float RightStickY();
	static bool GetGamePad();

private:
	static bool m_bGamePad;
	DIJOYSTATE m_aPadState;              //ゲームパッドの入力情報(プレス情報)
	DIJOYSTATE m_aPadStateTrigger;
	DIJOYSTATE m_aPadStateRelease;
};
#endif