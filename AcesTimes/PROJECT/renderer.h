//=============================================================================
//
// レンダリング処理 [renderer.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "main.h"

class CCamera;
class CFade;

class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, bool bWindow);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画
	LPDIRECT3DDEVICE9 GetDevice() { return m_pD3DDevice; }	//デバイス取得

#ifdef _DEBUG
	void DrawFPS();	//FPS書く
#endif

private:
	LPDIRECT3D9				m_pD3D = NULL;			// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice = NULL;	// Deviceオブジェクト(描画に必要)
	LPD3DXFONT				m_pFont = NULL;			// フォントへのポインタ
	static CFade *m_pFade;	//フェード
};

#endif