//--------------------------
//Author:三上航世
//バリアの処理(barrier.h)
//--------------------------
#ifndef _BARRIER_H_
#define _BARRIER_H_
#include "main.h"

//マクロ定義
#define BARRIER_WIDTH (180)  //ポリゴンの幅
#define BARRIER_HEIGHT (210) //ポリゴンの高さ
#define MAX_BARRIER (2)

typedef enum
{
	BARRIERSTATE_NORMAL = 0,
	BARRIERSTATE_DAMAGE,
	BARRIERSTATE_NODAMAGE,
	BARRIERSTATE_MAX
} BARRIERSTATE;

//バリアの構造体の定義
typedef struct
{
	D3DXVECTOR3 pos; //位置
	BARRIERSTATE state;
	int ntype;
	int nLife;       //耐久値
	bool bBlight;    //透明度を増やすか減らすか
	int nClear;      //バリアの透明度
	int nDClear;     //ダメージ食らった時の透明時間
	float fBig;      //使った直後の膨らみ
	int nDTime;     //ダメージを食らってからの時間
	bool bUse;       //使用中か否か
} Barrier;
//プロトタイプ宣言
void InitBarrier(void);
void UninitBarrier(void);
void UpdateBarrier(void);
void DrawBarrier(void);
void SetBarrier(D3DXVECTOR3 pos,int nLife, int ntype);
void HitBarrier(int nDamage, int nCntBarrier);
Barrier *GetBarrier(void);
void ResetBarrier(void);
#endif _BARRIER_H_