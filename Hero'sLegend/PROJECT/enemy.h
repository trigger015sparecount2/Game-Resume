//---------------------------
//Author:三上航世
//敵（階層構造あり）(enemy.h)
//---------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"

//モーションの種類
typedef enum
{
	ENEMYMOTION_NEUTRAL = 0,
	ENEMYMOTION_WALK,
	ENEMYMOTION_ACTION,
	ENEMYMOTION_DAMAGE,
	ENEMYMOTION_LANDING,
	ENEMYMOTION_RETURN, //ニュートラルモーションに戻る際のモーション。txtファイルに書く必要はない
	MAX_ENEMYMOTION
} ENEMYMOTION;

//プロトタイプ宣言
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void CoillisionEnemy(float fRadius, D3DXVECTOR3 *pPos, float fHeight);
float EnemyPos(int nRot, int nCntEnemy);
int RockOn(D3DXVECTOR3 pos);
float RockAttack(D3DXVECTOR3 pos, int nCntEnemy);
void SetEnemy(D3DXVECTOR3 pos, int nType);
bool EndBattle(void);
float RockHDistance(D3DXVECTOR3 pos, int nEnemy);
float RockVDistance(D3DXVECTOR3 pos, int nEnemy, float fHeight);
float RockEnemyHeight(int nCntEnemy);
bool EnemyNear(D3DXVECTOR3 pos, float fHeight);
D3DXVECTOR3 ECollisionPos(void);
void ResetEnemy(void);
int EnemyNum(void);
#endif _ENEMY_H_