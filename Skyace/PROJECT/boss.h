//--------------------------
//Author:三上航世
//ボスの処理(boss.h)
//--------------------------
#ifndef _BOSS_H_
#define _BOSS_H_
#include "main.h"

//マクロ定義
#define MAX_BOSS (16)   //敵の最大数

//ボスの種類
typedef enum
{
	BOSS_XU_44 = 0,	//最初のボス
	BOSS_SEAGULL,	//2番目のボス本体
	BOSS_XU_80,		//最後のボス
	BOSS_ENGINE,	//2番目のボスエンジン
	MAX_BOSS_TYPE
} BOSSTYPE;

//ボスの状態
typedef enum
{
	BOSSSTATE_NORMAL = 0,	//通常
	BOSSSTATE_DAMAGE,		//ダメージ食らった
	BOSSSTATE_BARRIER,		//バリア
	BOSSSTATE_DEATH,		//やられた
	BOSSSTATE_MAX
} BOSSSTATE;

//敵の構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	BOSSTYPE type;		//種類
	BOSSSTATE state;	//敵の状態
	int nCounterState;	//状態管理のカウンター
	int nfire;			//エンジンのバックファイア
	int nLife;			//体力
	int nScore;			//得点
	int nTime;			//最初の登場時の時間
	int nBulletTime;	//撃つまでの時間
	int nLaserTime;		//レーザー撃つまでの時間
	float fSmall;		//大きさ
	bool bLeft;			//左に行くか
	bool bUp;			//上に行くか
	bool bUse;			//使用中か否か
	bool bMissile;		//ミサイル追尾フラグ
	bool bBarrier;		//バリア使ったか
} Boss;

//プロトタイプ宣言
void InitBoss(void);	//初期化
void UninitBoss(void);	//終了
void UpdateBoss(void);	//更新
void DrawBoss(void);	//描画
void SetBoss(D3DXVECTOR3 pos, BOSSTYPE enemytype);		//設定
Boss *GetBoss(void);	//ポインタ取得
void HitBoss(int nCntBoss, int nDamage, bool bSpecail);	//当たった
void ResetBoss(void);	//リセット
#endif _BOSS_H_