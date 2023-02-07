//--------------------------
//Author:三上航世
//バリアの処理(barrier.h)
//--------------------------
#ifndef _BARRIER_H_
#define _BARRIER_H_
#include "main.h"

//マクロ定義
#define BARRIER_WIDTH (180.0f)  //ポリゴンの幅
#define BARRIER_HEIGHT (210.0f) //ポリゴンの高さ
#define MAX_BARRIER (2)

//バリアの状態
typedef enum
{
	BARRIERSTATE_NORMAL = 0,	//通常
	BARRIERSTATE_DAMAGE,		//ダメージ食らった
	BARRIERSTATE_NODAMAGE,		//無敵
	BARRIERSTATE_MAX
} BARRIERSTATE;

//バリアの構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	BARRIERSTATE state;	//状態
	int ntype;			//プレイヤーのものか、敵か
	int nLife;			//耐久値
	bool bBlight;		//透明度を増やすか減らすか
	int nClear;			//バリアの透明度
	int nDClear;		//ダメージ食らった時の透明時間
	float fBig;			//使った直後の膨らみ
	int nDTime;			//ダメージを食らってからの時間
	bool bUse;			//使用中か否か
} Barrier;

//プロトタイプ宣言
void InitBarrier(void);		//初期化
void UninitBarrier(void);	//終了
void UpdateBarrier(void);	//更新
void DrawBarrier(void);		//描画
void SetBarrier(D3DXVECTOR3 pos, int nLife, int ntype);	//設定
void HitBarrier(int nDamage, int nCntBarrier);			//当たった
Barrier *GetBarrier(void);	//ポインタ取得
void ResetBarrier(void);	//リセット
#endif _BARRIER_H_