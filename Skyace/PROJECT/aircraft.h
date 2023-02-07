//--------------------------
//Author:三上航世
//自機の処理(aircraft.h)
//--------------------------
#ifndef _AIRCRAFT_H_
#define _AIRCRAFT_H_
#include "main.h"

//マクロ定義
#define AIRCRAFT_WIDTH (100.0f)		//ポリゴンの幅
#define AIRCRAFT_HEIGHT (130.0f)		//ポリゴンの高さ
#define SHADOW_AIRCRAFT_WIDTH (30.0f)	//影の幅
#define SHADOW_AIRCRAFT_HEIGHT (40.0f)	//影の高さ

//自機の状態
typedef enum
{
	AIRCRAFTSTATE_NORMAL = 0,	//通常
	AIRCRAFTSTATE_DAMAGE,		//ダメージ食らった
	AIRCRAFTSTATE_NODAMAGE,		//無敵
	AIRCRAFTSTATE_BARRIER,		//バリア
	AIRCRAFTSTATE_MAX
} AIRCRAFTSTATE;

//自機の構造体定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 normalmove;	//移動量
	AIRCRAFTSTATE state;	//自機の状態
	bool bDeath;			//やられたか
	int nAType;				//機体の種類
	int nSType;				//特殊兵装の種類
	int nLife;				//耐久値
	int nMaxLife;			//最大耐久値
	int nDTime;				//ダメージ食らってからの時間
	int nClear;				//透明、実体化の時間
	int nLag;				//味方無人機の動きのラグ
	int nReload;			//特殊兵装のリロード時間
	int nSpecial;			//特殊兵装残弾数
	int nReSpecial;			//やり直し時の特殊兵装残弾数
} Aircraft;

//プロトタイプ宣言
void InitAircraft(void);		//初期化
void UninitAircraft(void);		//終了
void UpdateAircraft(void);		//更新
void DrawAircraft(void);		//描画
void SetAircraft(void);			//設定
Aircraft *GetAircraft(void);	//ポインタ取得
void SetTypeAircraft(int nAType, int nSType);	//種類取得
void HitAircraft(void);			//ダメージ判定
void ResetAircraft(void);		//リセット
#endif _AIRCRAFT_H_
