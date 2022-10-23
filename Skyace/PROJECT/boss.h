//--------------------------
//Author:三上航世
//ボスの処理(boss.h)
//--------------------------
#ifndef _BOSS_H_
#define _BOSS_H_
#include "main.h"

//マクロ定義
#define MAX_BOSS (16)   //敵の最大数

typedef enum
{
	BOSS_XU_44 = 0,
	BOSS_SEAGULL,
	BOSS_XU_80,
	BOSS_ENGINE,
	MAX_BOSS_TYPE
} BOSSTYPE;

typedef enum
{
	BOSSSTATE_NORMAL = 0,
	BOSSSTATE_DAMAGE,
	BOSSSTATE_BARRIER,
	BOSSSTATE_DEATH,
	BOSSSTATE_MAX
} BOSSSTATE;

//敵の構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;   //位置
	BOSSTYPE type;     //種類
	BOSSSTATE state;   //敵の状態
	int nCounterState; //状態管理のカウンター
	int nfire;
	int nLife;         //体力
	int nScore;        //得点
	int nTime;
	int nBulletTime;
	int nLaserTime;
	float fSmall;
	bool bLeft;
	bool bUp;
	bool bUse;         //使用中か否か
	bool bMissile;     //ミサイル追尾フラグ
	bool bBarrier;
} Boss;

//プロトタイプ宣言
void InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);
void SetBoss(D3DXVECTOR3 pos, BOSSTYPE enemytype);
Boss *GetBoss(void);
void HitBoss(int nCntBoss, int nDamage, bool bSpecail);
void ResetBoss(void);
#endif _BOSS_H_