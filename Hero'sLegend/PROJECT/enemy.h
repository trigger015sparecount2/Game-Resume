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
	ENEMYMOTION_NEUTRAL = 0,	//ニュートラル
	ENEMYMOTION_WALK,			//歩き
	ENEMYMOTION_ACTION,			//攻撃
	ENEMYMOTION_DAMAGE,			//ダメージ
	ENEMYMOTION_LANDING,		//着地
	ENEMYMOTION_RETURN,			//ニュートラルモーションに戻る際のモーション。txtファイルに書く必要はない
	MAX_ENEMYMOTION
} ENEMYMOTION;

//プロトタイプ宣言
HRESULT InitEnemy(void);	//初期化
void UninitEnemy(void);		//終了
void UpdateEnemy(void);		//更新
void DrawEnemy(void);		//描画
void CoillisionEnemy(float fRadius, D3DXVECTOR3 *pPos, float fHeight);	//敵との当たり判定
float EnemyPos(int nRot, int nCntEnemy);			//敵の場所取得
int RockOn(D3DXVECTOR3 pos);						//ロックオン処理
float RockAttack(D3DXVECTOR3 pos, int nCntEnemy);	//ロックオン中の攻撃
void SetEnemy(D3DXVECTOR3 pos, int nType);			//敵設定
bool EndBattle(void);		//戦い終了
float RockHDistance(D3DXVECTOR3 pos, int nEnemy);	//水平方向の距離
float RockVDistance(D3DXVECTOR3 pos, int nEnemy, float fHeight);		//垂直方向の距離
float RockEnemyHeight(int nCntEnemy);				//敵の身長
bool EnemyNear(D3DXVECTOR3 pos, float fHeight);		//敵が近くにいるか
D3DXVECTOR3 ECollisionPos(void);	//敵の攻撃の当たり判定のマトリックス取得
void ResetEnemy(void);		//敵リセット
int EnemyNum(void);			//敵の数
#endif _ENEMY_H_