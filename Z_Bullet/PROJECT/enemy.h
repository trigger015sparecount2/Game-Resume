//---------------------------
//Author:三上航世
//敵（階層構造あり）(enemy.h)
//---------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"

typedef enum
{
	ENEMYTYPE_BIRD = 0,
	ENEMYTYPE_DOG,
	ENEMYTYPE_ZOMBE,
	ENEMYTYPE_SPIDER,
	ENEMYTYPE_MAX
} Enemytype;

//プロトタイプ宣言
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void CoillisionEnemy(float fRadius, D3DXVECTOR3 *pPos, D3DXVECTOR3 move);
bool CollisionDamage(D3DXVECTOR3 pos, float fRadius, D3DXVECTOR3 Clash, int nDamage, int nDamageTime);
float EnemyPos(int nRot, int nCntEnemy);
int RockOn(D3DXVECTOR3 pos, float fRotY);
float RockBullet(D3DXVECTOR3 pos, int nCntEnemy);
void SetEnemy(D3DXVECTOR3 pos, Enemytype enemyType);
void ResetEnemy(void);
int EnemyNum(void);
#endif _ENEMY_H_