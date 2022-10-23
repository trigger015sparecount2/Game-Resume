//--------------------------------------
//Author:三上航世
//ポリゴンの移動(input.cpp)
//---------------------------------------
#include "input.h"

//マクロ定義
#define NUM_KEY_MAX (256) //キーの最大数

//グローバル変数
LPDIRECTINPUT8 g_pInput = NULL;             //DirectInputオブジェクトのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL; //入力デバイス(キーボード)へのポインタ
BYTE g_aKeyState[NUM_KEY_MAX];              //キーボードの入力情報(プレス情報)
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];
BYTE g_aKeyStateRelease[NUM_KEY_MAX];
bool g_bKeyboard;


//初期化処理
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInitオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&g_pInput, NULL)))
	{
		return E_FAIL;
	}
	//入力デバイスの生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}
	// データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDevKeyboard->Acquire();
	g_bKeyboard = true;
	return S_OK;
}

//終了処理
void UninitKeyboard(void)
{
	//入力デバイスの開放
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
	//DirectInputオブジェクトの開放
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//更新処理
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;
	//入力デバイスからデータを取得する
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = g_aKeyState[nCntKey] ^ aKeyState[nCntKey] & aKeyState[nCntKey];
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey];
			g_aKeyState[nCntKey] = aKeyState[nCntKey]; //キーボードの入力情報保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();
	}
}

//キーボードの入力情報を取得
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;

}

bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}