//---------------------------
//Author:三上航世
//モデル（階層構造あり）(player.h)
//---------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "main.h"

//プロトタイプ宣言
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
D3DXVECTOR3 PlayerPos(void);
float PlayerRot(void);
float ClashRot(D3DXVECTOR3 pos);
D3DXVECTOR3 PlayerGetMove(void);
float GetPlayerHeight(void);
float GetPlayerRadius(void);
float PlayerRotDesh(void);
bool TargetRockon(void);
int NumParts(void);
D3DXMATRIX CollisionPlayerPos(int nCntParts);
float PlayerLookEnemy(void);
void ScoreAdd(int nScore);
float PlayerDistance(D3DXVECTOR3 pos);
void PlayerDamage(int nDamage, int nDamageTime, D3DXVECTOR3 Clash, float fRot);
bool PlayerDamage(D3DXVECTOR3 pos, float fRadius);
float PlayerClashRot(D3DXVECTOR3 pos);
void ResetPlayer(void);
void AddItem(void);
int PlayerScore(void);
bool GetPlayerDie(void);
#endif _PLAYER_H_