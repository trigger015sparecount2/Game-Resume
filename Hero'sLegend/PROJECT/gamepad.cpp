//--------------------------------------
//Author:三上航世
//ゲームパッドの入力情報(padinput.cpp)
//---------------------------------------
#include "gamepad.h"

//グローバル変数
LPDIRECTINPUT8 g_pPadInput = NULL;             //DirectInputオブジェクトのポインタ
LPDIRECTINPUTDEVICE8 g_pDevGamepad = NULL; //入力デバイス(ゲームパッド)へのポインタ
DIJOYSTATE g_aPadState;              //キーボードの入力情報(プレス情報)
DIJOYSTATE g_aPadStateTrigger;
DIJOYSTATE g_aPadStateRelease;
bool g_bGamepad;


//初期化処理
HRESULT InitGamepad(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInitオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&g_pPadInput, NULL)))
	{
		return E_FAIL;
	}
	//接続されてるゲームパッドの列挙
	if (FAILED(g_pPadInput->EnumDevices(DI8DEVCLASS_GAMECTRL, DeviceFindCallBack, NULL, DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}
	//入力デバイスの生成
	if (FAILED(g_pPadInput->CreateDevice(GUID_Joystick, &g_pDevGamepad, NULL)))
	{
		return E_FAIL;
	}
	// データフォーマットを設定
	if (FAILED(g_pDevGamepad->SetDataFormat(&c_dfDIJoystick)))
	{
		return E_FAIL;
	}
	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;

	// 軸モードを変更
	if (FAILED(g_pDevGamepad->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return E_FAIL;
	}

	// X軸の値の範囲設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	if (FAILED(g_pDevGamepad->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return E_FAIL;
	}

	// Y軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(g_pDevGamepad->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(g_pDevGamepad->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	// ゲームパッドへのアクセス権を獲得(入力制御開始)
	g_pDevGamepad->Acquire();
	//処理を同期させる
	g_pDevGamepad->Poll();
	g_bGamepad = true;
	return S_OK;
}

//終了処理
void UninitGamepad(void)
{
	//入力デバイスの開放
	if (g_pDevGamepad != NULL)
	{
		g_pDevGamepad->Unacquire();
		g_pDevGamepad->Release();
		g_pDevGamepad = NULL;
	}
	//DirectInputオブジェクトの開放
	if (g_pPadInput != NULL)
	{
		g_pPadInput->Release();
		g_pPadInput = NULL;
	}
}

//更新処理
void UpdateGamepad(void)
{
	DIJOYSTATE aPadState;
	int nCntPad;
	//入力デバイスからデータを取得する
	if (SUCCEEDED(g_pDevGamepad->GetDeviceState(sizeof(aPadState), &aPadState)))
	{
		for (nCntPad = 0; nCntPad < DIP_MAX; nCntPad++)
		{
			g_aPadStateTrigger.rgbButtons[nCntPad] = g_aPadState.rgbButtons[nCntPad] ^ aPadState.rgbButtons[nCntPad] & aPadState.rgbButtons[nCntPad];
			g_aPadStateRelease.rgbButtons[nCntPad] = (g_aPadState.rgbButtons[nCntPad] ^ aPadState.rgbButtons[nCntPad]) & g_aPadState.rgbButtons[nCntPad];
			g_aPadState.rgbButtons[nCntPad] = aPadState.rgbButtons[nCntPad]; //キーボードの入力情報保存
		}
		g_aPadState.lX = aPadState.lX;
		g_aPadState.lY = aPadState.lY;
		g_aPadState.lZ = aPadState.lZ;
		g_aPadState.lRz = aPadState.lRz;
		g_aPadStateTrigger.rgdwPOV[0] = g_aPadState.rgdwPOV[0];
		g_aPadState.rgdwPOV[0] = aPadState.rgdwPOV[0];
	}
	else
	{
		g_pDevGamepad->Acquire();
	}
}

BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	return DIENUM_CONTINUE;
}

//左スティック横方向
float LeftStickX(void)
{
	float length = 1000.0f;
	float LeftX = 0.0f;
	if (g_aPadState.lX < 0)
	{
		LeftX = (float)g_aPadState.lX / length; //左スティックを左に傾けたとき
	}
	else if (g_aPadState.lX > 0)
	{
		LeftX = (float)g_aPadState.lX / length; //左スティックを右に傾けたとき
	}
	return LeftX;
}

//左スティック縦方向
float LeftStickY(void)
{
	float length = 1000.0f;
	float LeftY = 0.0f;
	if (g_aPadState.lY < 0)
	{
		LeftY = (float)g_aPadState.lY / length; //左スティックを下に傾けたとき
	}
	else if (g_aPadState.lY > 0)
	{
		LeftY = (float)g_aPadState.lY / length; //左スティックを上に傾けたとき
	}
	return LeftY;
}

//右スティック横方向
float RightStickX(void)
{
	float length = 32767.0f;
	float RightX = 0.0f;
	if (g_aPadState.lZ < 32767)
	{
		RightX = (float)(g_aPadState.lZ - length) / length; //右スティックを左に傾けたとき
	}
	else if (g_aPadState.lZ > 32767)
	{
		RightX = (float)(g_aPadState.lZ - length) / length; //右スティックを右に傾けたとき
	}
	return RightX;
}

//右スティック縦方向
float RightStickY(void)
{
	float length = 32767.0f;
	float RightY = 0.0f;
	if (g_aPadState.lRz < 32767)
	{
		RightY = (float)(g_aPadState.lRz - length) / length; //右スティックを上に傾けたとき
	}
	else if (g_aPadState.lRz > 32767)
	{
		RightY = (float)(g_aPadState.lRz - length) / length; //右スティックを下に傾けたとき
	}
	return RightY;
}

//十字キーが押されてるか（上から時計回りに0.0f, 4500.0f, 9000.0f, 13500.0f, 18000.0f, 22500.0f, 27000.0f, 31500.0f）
float CrossKey(void)
{
	//十字キーが入力されてるか
	if (g_aPadState.rgdwPOV[0] != 0xFFFFFFFF)
	{
		//直前に押された方向と違うか（同じだったら無視する）
		if (g_aPadStateTrigger.rgdwPOV[0] != g_aPadState.rgdwPOV[0])
		{
			return g_aPadState.rgdwPOV[0];
		}
	}
	return 10.0f; //何も押されてなかったとき用
}

//キーボードの入力情報を取得
bool GetGamePadPress(PadButton Button)
{
	return (g_aPadState.rgbButtons[Button] & 0x80) ? true : false;
}

bool GetGamePadTrigger(PadButton Button)
{
	return (g_aPadStateTrigger.rgbButtons[Button] & 0x80) ? true : false;

}

bool GetGamePadRelease(PadButton Button)
{
	return (g_aPadStateRelease.rgbButtons[Button] & 0x80) ? true : false;
}

//ゲームパッドが繋がってるかどうか
bool GamePad(void)
{
	return g_bGamepad;
}

//何かボタン押したとき
bool GetAnyButton(void)
{
	int nCntPad;
	for (nCntPad = 0; nCntPad < DIP_MAX; nCntPad++)
	{
		if ((g_aPadStateTrigger.rgbButtons[nCntPad] & 0x80) != 0 && (g_aPadStateRelease.rgbButtons[nCntPad] & 0x80) == 0)
		{
			return true;
		}
	}
	return false;
}

//特定のボタンが押されたか
bool GetGamePadButton(PadButton Button)
{
	if ((g_aPadStateTrigger.rgbButtons[Button] & 0x80) != 0 && (g_aPadStateRelease.rgbButtons[Button] & 0x80) == 0)
	{
		return true;
	}
	return false;
}