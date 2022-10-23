//---------------------------
//Author:三上航世
//モデル（階層構造あり）(player.h)
//---------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "main.h"

//モーションの種類
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,
	MOTIONTYPE_WALK,
	MOTIONTYPE_NORMAL,
	MOTIONTYPE_RUSH,
	MOTIONTYPE_ROUND,
	MOTIONTYPE_ARIAIL,
	MOTIONTYPE_GROUND,
	MOTIONTYPE_FINISH,
	MOTIONTYPE_JUMP,
	MOTIONTYPE_LANDING,
	MOTIONTYPE_DAMAGE,
	MOTIONTYPE_DIE,
	MOTIONTYPE_RETURN, //ニュートラルモーションに戻る際のモーション。txtファイルに書く必要はない
	MAX_MOTION
} MOTIONTYPE;

//プロトタイプ宣言
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
D3DXVECTOR3 PlayerPos(void);
float PlayerRot(void);
float PlayerMove(int nRot);
float PlayerRotDesh(void);
D3DXMATRIX Playermtx(void);
bool TargetRockon(void);
void RemovePlayerRock(void);
int NumParts(void);
bool CollisionDamage(D3DXVECTOR3 pos, float fRadius);
float CollisionPos(int nNumber, int nRot);
float PlayerLookEnemy(void);
D3DXVECTOR3 EnemyClash(D3DXVECTOR3 pos);
float ClashRot(D3DXVECTOR3 pos);
float PlayerClashRot(D3DXVECTOR3 pos);
int DamageTime(void);
int Damage(void);
int PlayerRockEnemy(void);
float PlayerPartsRot(void);
int PlayerKey(void);
float PlayerDistance(D3DXVECTOR3 pos);
float PlayerHeight(void);
void PlayerDamageTime(int nDamageTime);
bool PlayerDamage(D3DXVECTOR3 pos, float fRadius);
void PlayerDamage(int nDamage, int nDamageTime, float fRoty, D3DXVECTOR3 Clash);
int PlayerHP(void);
float PlayerHPRate(void);
void PlayerKillAdd(void);
int PlayerKill(void);
#endif _PLAYER_H_