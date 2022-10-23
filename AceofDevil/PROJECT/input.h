//=============================================================================
//
// 入力処理 [input.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

//マクロ定義
#define NUM_KEY_MAX (256) //キーの最大数

class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;             //DirectInputオブジェクトのポインタ
	LPDIRECTINPUTDEVICE8 m_pDevice; //入力デバイス(キーボード)へのポインタ
};

class CInputKeyboard : CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetAnyKey(void);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetKey(int nKey);

private:
	BYTE m_aKeyState[NUM_KEY_MAX];        //キーボードの入力情報(プレス情報)
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];
};

class CInputGamePad : CInput
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
	CInputGamePad();
	~CInputGamePad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(PadButton Button);
	bool GetAnyButton(void);
	bool GetTrigger(PadButton Button);
	bool GetRelease(PadButton Button);
	bool GetButton(PadButton Button);
	float LeftStickX(void);
	float LeftStickY(void);
	float TriggerCrossKey(void);
	float PressCrossKey(void);
	float RightStickX(void);
	float RightStickY(void);
	static bool GetGamePad(void);

private:
	static bool m_bGamePad;
	DIJOYSTATE m_aPadState;              //ゲームパッドの入力情報(プレス情報)
	DIJOYSTATE m_aPadStateTrigger;
	DIJOYSTATE m_aPadStateRelease;
	//BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef);
};
#endif