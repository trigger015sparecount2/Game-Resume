//------------------------------
//Author:三上航世
//ゲーム(main.cpp)
//------------------------------
#include "main.h"
#include "input.h"
#include "sound.h"
#include "choice.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "bg.h"

//マクロ定義
#define CLASS_NAME "WindowsClass"    //ウィンドウクラスの名前
#define WINDOW_NAME "SKYACE" //ウィンドウの名前(キャプション名)

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//グローバル変数
LPDIRECT3D9 g_pD3D = NULL;             //Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL; //Direct3Dデバイスへのポインタ
LPD3DXFONT g_pFont = NULL;
MODE g_mode = MODE_TITLE; //現在のモード
int g_nCountFPS;

//メイン関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, PSTR lpCmdLine, int nCmdShow)
{
	//構造体の設定(ウィンドウクラスへの指示)
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),             //WNDCLASSEXのメモリサイズの指定
		CS_CLASSDC,                     //表示するウィンドウのスタイルを設定
		WindowProc,                     //ウィンドウプロシージャのアドレス(関数名)を指定
		0,                              //通常は使用しないので0を指定(ほぼ特例なく0)
		0,                              //通常は使用しないので0を指定(ほぼ特例なく0)
		hInstance,                      //Windowsの引数のインスタンスハンドルを設定
		LoadIcon(NULL,IDI_APPLICATION), //ファイルアイコンに使用するアイコンを指定
		LoadCursor(NULL,IDC_ARROW),     //使用するマウスカーソルを指定
		(HBRUSH)(COLOR_WINDOW + 1),     //ウィンドウのクライアント領域の背景色を指定
		NULL,                           //Windowにつけるメニューを設定
		CLASS_NAME,                     //ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)  //ファイルアイコンに使用するアイコンを設定
	};
	HWND hWnd; //ウィンドウハンドル(識別子)
	MSG msg;   //メッセージを格納する変数
	DWORD dwFPSLastTime;
	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);
	//ウィンドウの生成(作ったウィンドウをウィンドウハンドルに納めて管理する)
	hWnd = CreateWindowEx(0, //拡張ウィンドウスタイル
		CLASS_NAME,          //ウィンドウクラスの名前
		WINDOW_NAME,         //ウィンドウの名前
		WS_OVERLAPPEDWINDOW, //ウィンドウスタイル
		CW_USEDEFAULT,       //ウィンドウの左上x座標
		CW_USEDEFAULT,       //ウィンドウの左上y座標
		SCREEN_WIDTH,        //ウィンドウの幅
		SCREEN_HEIGHT,       //ウィンドウの高さ
		NULL,                //親ウィンドウのハンドル
		NULL,                //メニューウィンドウまたは子ウィンドウ
		hInstance,           //インスタンスハンドル
		NULL);               //ウィンドウ作成データ
							 //初期化処理(ウィンドウを生成してから行う)(DirectX本体の生成を行っている)
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}
	dwExecLastTime = timeGetTime();
	dwFPSLastTime = timeGetTime();
	dwCurrentTime = 0;
	dwFrameCount = 0;
	//ウィンドウ表示
	ShowWindow(hWnd, nCmdShow); //ウィンドウの表示(ウィンドウの表示状態を設定)
	UpdateWindow(hWnd);         //ウィンドウの反映(ウィンドウのクライアント領域を更新)
	//メッセージループ(メッセージキューからメッセージを取得)
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//Windowの処理
			if (msg.message == WM_QUIT)
			{//"WM_QUIT"メッセージが送られてきたらループを抜ける
				break;
			}
			else
			{
				//メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒ごとに実行
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 1/60秒経過
				dwExecLastTime = dwCurrentTime;
				// 更新処理
				Update();
				// 描画処理
				Draw();
				dwFrameCount++;
			}
		}
	}
	//終了処理
	Uninit();
	//ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return (int)msg.wParam;
}

//ウィンドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID; //メッセージ(YES,NO)のID
	switch (uMsg)
	{
	case WM_DESTROY: //ウィンドウ破棄メッセージ
		//WM_QUITメッセージを返す
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN: //[ESC]キーが押された
		//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
		switch (wParam)
		{
		case VK_ESCAPE:
			//メッセージBoxの表示
			nID = MessageBox(hWnd, "本当に終了しますか?", "終了確認", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		}
		break;
	case WM_CLOSE: //x(閉じる)ボタン押下のメッセージ
		//メッセージBoxの表示
		nID = MessageBox(hWnd, "本当に終了しますか?", "終了確認", MB_YESNO);
		if (nID == IDYES)
		{
			//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0; //0を返さないと終了してしまうので0を返す
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//初期化処理
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;        //ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp; //プレゼンテーションパラメータ
								 //Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							            // バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;	              			// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				            // ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;			            	// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		                // 映像信号に同期してフリップする
	d3dpp.Windowed = bWindow;						                // ウィンドウモード
	d3dpp.EnableAutoDepthStencil = TRUE;						    // デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					    // デプスバッファとして16bitを使う
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	    // インターバル

																	// デバイスの生成
																	// ディスプレイアダプタを表すためのデバイスを作成
																	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングを行わない
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

																			// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	//オブジェクト初期化
	InitKeyboard(hInstance, hWnd);
	InitSound(hWnd);
	InitBg();
	InitFade(g_mode);
	srand((unsigned int)time(NULL));
	SetMode(g_mode);
	return S_OK;
}

//終了処理
void Uninit(void)
{
	if (g_pFont != NULL)
	{// デバッグ情報表示用フォントを設定
		g_pFont->Release();
		g_pFont = NULL;
	}
	//オブジェクト終了処理
	UninitKeyboard();
	UninitSound();
	UninitBg();
	UninitTitle();
	UninitChoice();
	UninitGame();
	UninitResult();
	UninitFade();
	//Direct3Dデバイス開放
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}
	//Direct3Dオブジェクトの開放
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//更新処理
void Update(void)
{
	//オブジェクト更新処理
	UpdateKeyboard();
	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();
		break;
	case MODE_CHOICE:
		UpdateChoice();
		break;
	case MODE_GAME:
		UpdateBg();
		UpdateGame();
		break;
	case MODE_RESULT:
		UpdateResult();
		break;
	}
	UpdateFade();
}

//描画処理
void Draw(void)
{
	//バックバッファ&Zバッファのクリア
	g_pD3DDevice->Clear(0,
		                NULL,
		                (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		                D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	//描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		DrawBg();
		//オブジェクト描画処理
		switch (g_mode)
		{
		case MODE_TITLE:
			DrawTitle();
			break;
		case MODE_CHOICE:
			DrawChoice();
			break;
		case MODE_GAME:
			DrawGame();
			break;
		case MODE_RESULT:
			DrawResult();
			break;
		}
		DrawFPS();
		DrawFade();
		//描画の終了
		g_pD3DDevice->EndScene();
	}
	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

void DrawFPS(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];
	wsprintf(str, "FPS:%d\n", g_nCountFPS);
	// テキスト描画
	g_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}


//モードの設定
void SetMode(MODE mode)
{
	switch (g_mode)
	{
	case MODE_TITLE:
		UninitTitle();
		break;
	case MODE_CHOICE:
		UninitChoice();
		StopSound();
		break;
	case MODE_GAME:
		UninitGame();
		StopSound();
		break;
	case MODE_RESULT:
		UninitResult();
		StopSound();
		break;
	}
	switch (mode)
	{
	case MODE_TITLE:
		InitTitle();
		break;
	case MODE_CHOICE:
		InitChoice();
		break;
	case MODE_GAME:
		InitGame();
		break;
	case MODE_RESULT:
		InitResult();
		break;
	}
	g_mode = mode;
}

MODE GetMode(void)
{
	return g_mode;
}