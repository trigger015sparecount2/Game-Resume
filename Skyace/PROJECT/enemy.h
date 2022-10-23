//--------------------------
//Author:三上航世
//敵の処理(enemy.h)
//--------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"

//マクロ定義
#define MAX_ENEMY (128)   //敵の最大数
#define ENEMY_BLOCK (80)  //敵の大きさ
#define SHADOW_ENEMY_BLOCK (30)  //敵の影の大きさ

typedef enum
{
	ENEMY_XU_5 = 0,
	ENEMY_XU_17,
	ENEMY_XU_37,
	ENEMY_XU_57,
	MAX_ENEMY_TYPE
} ENEMYTYPE;

typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_MAX
} ENEMYSTATE;

typedef enum
{
	WAY_UP = 0,
	WAY_RIGHT,
	WAY_DOWN,
	WAY_LEFT,
	WAY_MAX
} WAY;

//敵の構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;   //位置
	ENEMYTYPE type;    //種類
	D3DXVECTOR3 move;  //動き方
	ENEMYSTATE state;  //敵の状態
	float fRoll[4];           //方向
	int nCounterState; //状態管理のカウンター
	int nLife;         //体力
	int nScore;        //得点
	bool bPrize;       //プライズを持っているか
	bool bUse;         //使用中か否か
	bool bMissile;     //ミサイル追尾フラグ
} Enemy;

//プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE enemytype, D3DXVECTOR3 move, bool bPrize, WAY way);
Enemy *GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage, bool bSpecail);
void ResetEnemy(void);
#endif _ENEMY_H_