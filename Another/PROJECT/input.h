//=============================================================================
//
// 入力処理 [input.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//初期化
	virtual void Uninit();		//終了
	virtual void Update() = 0;	//更新

protected:
	static LPDIRECTINPUT8 m_pInput;	//DirectInputオブジェクトのポインタ
	LPDIRECTINPUTDEVICE8 m_pDevice;	//入力デバイス(キーボード)へのポインタ
};
#endif