//---------------------------
//Author:三上航世
//オブジェクト管理(main.h)
//---------------------------
#ifndef _MAIN_H_
#define _MAIN_H_
#define DIRECTINPUT_VERSION (0x0800)
#define _CRT_SECURE_NO_WARNINGS
//インクルードファイル
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "d3dx9.h"   //描画に必要
#include "dinput.h"  //入力処理に必要
#include "xaudio2.h"   //サウンド関連

//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")    //描画に必要
#pragma comment(lib,"d3dx9.lib")   //[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")  //DirectXコンポーネント(部品)使用に必要
#pragma comment(lib,"dinput8.lib") //入力処理に必要
#pragma comment(lib,"winmm.lib")

//マクロ定義
#define SCREEN_WIDTH (1280)                                          //ウィンドウの幅
#define SCREEN_HEIGHT (720)                                          //ウィンドウの高さ　16:9のサイズ
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1) //頂点フォーマット

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos; //頂点座標
	D3DXVECTOR3 nor; //法線ベクトル
	D3DCOLOR col;    //頂点カラー
	D3DXVECTOR2 tex; //テクスチャ座標
} VERTEX_3D;

//画面(モード)の種類
typedef enum
{
	MODE_TITLE = 0,
	MODE_TUTORIAL,
	MODE_GAME,
	MODE_RESULT,
	MODE_MAX
} MODE;


//プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);
LPD3DXSPRITE GetSprite(void);
void SetMode(MODE mode);
MODE GetMode(void);
void DrawFPS(void);
#endif _MAIN_H_