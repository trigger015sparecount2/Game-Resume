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
	static LPDIRECTINPUT8 m_pInput;	//DirectInputオブジェクトのポインタ
	LPDIRECTINPUTDEVICE8 m_pDevice;	//入力デバイス(キーボード)へのポインタ
};

class CInputKeyboard : CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新

	bool GetPress(int nKey) { return (m_aKeyState[nKey] & 0x80) ? true : false; }			//押されているか
	bool GetAnyKey(void);	//何か押されたか
	bool GetTrigger(int nKey) { return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false; }	//押された、または離されたか
	bool GetRelease(int nKey) { return (m_aKeyStateRelease[nKey] & 0x80) ? true : false; }	//離されたか
	bool GetKey(int nKey);	//押されたか

private:
	BYTE m_aKeyState[NUM_KEY_MAX];        //キーボードの入力情報(プレス情報)
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];
};

class CInputGamePad : CInput
{
public:
	//ゲームパッドのボタンの種類
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
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新

	bool GetPress(PadButton Button) { return (m_aPadState.rgbButtons[Button] & 0x80) ? true : false; }			//押されたか
	bool GetAnyButton(void);			//何か押されたか
	bool GetTrigger(PadButton Button) { return (m_aPadStateTrigger.rgbButtons[Button] & 0x80) ? true : false; }	//押された、または離されたか
	bool GetRelease(PadButton Button) { return (m_aPadStateRelease.rgbButtons[Button] & 0x80) ? true : false; }	//離されたか
	bool GetButton(PadButton Button);	//押されたか
	float LeftStickX(void);				//左スティック横
	float LeftStickY(void);				//左スティック縦
	float TriggerCrossKey(void);		//十字キー押されたか
	float PressCrossKey(void);			//十字キー押されているか
	float RightStickX(void);			//右スティック横
	float RightStickY(void);			//右スティック縦
	static bool GetGamePad(void) { return m_bGamePad; }		//ゲームパッド繋がっているか

private:
	static bool m_bGamePad;
	DIJOYSTATE m_aPadState;              //ゲームパッドの入力情報(プレス情報)
	DIJOYSTATE m_aPadStateTrigger;
	DIJOYSTATE m_aPadStateRelease;
	//BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef);
};
#endif