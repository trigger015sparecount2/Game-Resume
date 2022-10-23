//--------------------------------------
//Author:三上航世
//キーボードの入力情報(mouse.cpp)
//---------------------------------------
#include "mouse.h"

//グローバル変数
LPDIRECTINPUT8 g_pMouseInput = NULL;             //DirectInputオブジェクトのポインタ
LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL; //入力デバイス(マウス)へのポインタ
DIMOUSESTATE g_aMouseState;              //マウスの入力情報(プレス情報)
DIMOUSESTATE g_aMouseStateTrigger;
DIMOUSESTATE g_aMouseStateRelease;
bool g_bMouse;


//初期化処理
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInitオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&g_pMouseInput, NULL)))
	{
		return E_FAIL;
	}
	//入力デバイスの生成
	if (FAILED(g_pMouseInput->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		return E_FAIL;
	}
	// データフォーマットを設定
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	//処理を同期させる
	g_pDevMouse->Poll();

	// マウスへのアクセス権を獲得(入力制御開始)
	g_pDevMouse->Acquire();
	g_bMouse = true;
	return S_OK;
}

//終了処理
void UninitMouse(void)
{
	//入力デバイスの開放
	if (g_pDevMouse != NULL)
	{
		g_pDevMouse->Unacquire();
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}
	//DirectInputオブジェクトの開放
	if (g_pMouseInput != NULL)
	{
		g_pMouseInput->Release();
		g_pMouseInput = NULL;
	}
}

//更新処理
void UpdateMouse(void)
{
	DIMOUSESTATE aMouseState;
	int nCntBut;
	//入力デバイスからデータを取得する
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(aMouseState), &aMouseState)))
	{
		for (nCntBut = 0; nCntBut < DIM_MAX; nCntBut++)
		{
			g_aMouseStateTrigger.rgbButtons[nCntBut] = g_aMouseState.rgbButtons[nCntBut] ^ aMouseState.rgbButtons[nCntBut] & aMouseState.rgbButtons[nCntBut];
			g_aMouseStateRelease.rgbButtons[nCntBut] = (g_aMouseState.rgbButtons[nCntBut] ^ aMouseState.rgbButtons[nCntBut]) & g_aMouseState.rgbButtons[nCntBut];
			g_aMouseState.rgbButtons[nCntBut] = aMouseState.rgbButtons[nCntBut]; //マウスの入力情報保存
		}
		g_aMouseState.lX = aMouseState.lX;
		g_aMouseState.lY = aMouseState.lY;
		g_aMouseState.lZ = aMouseState.lZ;
	}
	else
	{
		g_pDevMouse->Acquire();
	}
}

//マウスの入力情報を取得
bool GetMousePress(MouseButton Button)
{
	return (g_aMouseState.rgbButtons[Button] & 0x80) ? true : false;
}

bool GetMouseTrigger(MouseButton Button)
{
	return (g_aMouseStateTrigger.rgbButtons[Button] & 0x80) ? true : false;

}

bool GetMouseRelease(MouseButton Button)
{
	return (g_aMouseStateRelease.rgbButtons[Button] & 0x80) ? true : false;
}

float MouseX(void) //マウス横(右だとプラス、左だとマイナス)
{
	return g_aMouseState.lX;
}

float MouseY(void) //マウス縦(上だとマイナス、下だとプラス)
{
	return g_aMouseState.lY;
}

float MouseZ(void) //マウスのホイール(上に転がすとプラス、下だとマイナス)
{
	return g_aMouseState.lZ;
}

bool Mouse(void)
{
	return g_bMouse;
}

//特定のボタンが押されたか
bool GetMouseButton(MouseButton Button)
{
	if ((g_aMouseStateTrigger.rgbButtons[Button] & 0x80) != 0 && (g_aMouseStateRelease.rgbButtons[Button] & 0x80) == 0)
	{
		return true;
	}
	return false;
}