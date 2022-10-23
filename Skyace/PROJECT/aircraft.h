//--------------------------
//Author:三上航世
//自機の処理(aircraft.h)
//--------------------------
#ifndef _AIRCRAFT_H_
#define _AIRCRAFT_H_
#include "main.h"
//マクロ定義
#define AIRCRAFT_WIDTH (100)  //ポリゴンの幅
#define AIRCRAFT_HEIGHT (130) //ポリゴンの高さ
#define SHADOW_AIRCRAFT_HEIGHT (40) //影の高さ
#define SHADOW_AIRCRAFT_WIDTH (30) //影の横幅

typedef enum
{
	AIRCRAFTSTATE_NORMAL = 0,
	AIRCRAFTSTATE_DAMAGE,
	AIRCRAFTSTATE_NODAMAGE,
	AIRCRAFTSTATE_BARRIER,
	AIRCRAFTSTATE_MAX
} AIRCRAFTSTATE;

//自機の構造体定義
typedef struct
{
	D3DXVECTOR3 pos;        //位置
	D3DXVECTOR3 move;       //移動量
	D3DXVECTOR3 normalmove; //移動量
	AIRCRAFTSTATE state;    //自機の状態
	bool bDeath;            //やられたか
	int nAType;             //機体の種類
	int nSType;             //特殊兵装の種類
	int nLife;              //耐久値
	int nMaxLife;           //最大耐久値
	int nDTime;             //ダメージ食らってからの時間
	int nClear;             //透明、実体化の時間
	int nLag;               //味方無人機の動きのラグ
	int nReload;            //特殊兵装のリロード時間
	int nSpecial;           //特殊兵装残弾数
	int nReSpecial;         //やり直し時の特殊兵装残弾数
} Aircraft;

//プロトタイプ宣言
void InitAircraft(void);
void UninitAircraft(void);
void UpdateAircraft(void);
void DrawAircraft(void);
void SetAircraft(void);
Aircraft *GetAircraft(void);
void SetTypeAircraft(int nAType, int nSType);
void HitAircraft(void);
void ResetAircraft(void);
#endif _AIRCRAFT_H_
