//=============================================================================
//
// 入力処理 [keyboard.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include "main.h"
#include "input.h"

//マクロ定義
#define NUM_KEY_MAX (256) //キーの最大数

class CKeyboard : CInput
{
public:
	CKeyboard();
	~CKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	bool GetPress(int nKey) { return (m_aKeyState[nKey] & 0x80) ? true : false; }			//キーが押されてるか
	bool GetAnyKey();		//何かキーが押されたか
	bool GetTrigger(int nKey) { return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false; }	//キーの状態が変わったか
	bool GetRelease(int nKey) { return (m_aKeyStateRelease[nKey] & 0x80) ? true : false; }	//キーが離されたか
	bool GetKey(int nKey);	//キーが押されたか

private:
	BYTE m_aKeyState[NUM_KEY_MAX];			//キーボードの入力情報(プレス情報)
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	//キーボードの入力情報(トリガー情報)
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];	//キーボードの入力情報(リリース情報)
};
#endif