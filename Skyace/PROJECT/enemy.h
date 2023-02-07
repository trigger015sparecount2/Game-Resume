//--------------------------
//Author:三上航世
//敵の処理(enemy.h)
//--------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"

//マクロ定義
#define MAX_ENEMY (128)				//敵の最大数
#define ENEMY_BLOCK (80.0f)			//敵の大きさ
#define SHADOW_ENEMY_BLOCK (30.0f)		//敵の影の大きさ

//敵の種類
typedef enum
{
	ENEMY_XU_5 = 0,
	ENEMY_XU_17,
	ENEMY_XU_37,
	ENEMY_XU_57,
	MAX_ENEMY_TYPE
} ENEMYTYPE;

//敵の状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	//通常
	ENEMYSTATE_DAMAGE,		//ダメージ食らった
	ENEMYSTATE_MAX
} ENEMYSTATE;

//敵の方向
typedef enum
{
	WAY_UP = 0,	//上
	WAY_RIGHT,	//右
	WAY_DOWN,	//下
	WAY_LEFT,	//左
	WAY_MAX
} WAY;

//敵の構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	ENEMYTYPE type;		//種類
	D3DXVECTOR3 move;	//動き方
	ENEMYSTATE state;	//敵の状態
	float fRoll[4];		//方向
	int nCounterState;	//状態管理のカウンター
	int nLife;			//体力
	int nScore;			//得点
	bool bPrize;		//プライズを持っているか
	bool bUse;			//使用中か否か
	bool bMissile;		//ミサイル追尾フラグ
} Enemy;

//プロトタイプ宣言
void InitEnemy(void);	//初期化
void UninitEnemy(void);	//終了
void UpdateEnemy(void);	//更新
void DrawEnemy(void);	//描画
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE enemytype, D3DXVECTOR3 move, bool bPrize, WAY way);	//設定
Enemy *GetEnemy(void);	//ポインタ取得
void HitEnemy(int nCntEnemy, int nDamage, bool bSpecail);	//当たった
void ResetEnemy(void);	//リセット
#endif _ENEMY_H_