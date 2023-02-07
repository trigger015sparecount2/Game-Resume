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
HRESULT InitPlayer(void);	//初期化
void UninitPlayer(void);	//終了
void UpdatePlayer(void);	//更新
void DrawPlayer(void);		//描画

D3DXVECTOR3 PlayerPos(void);	//pos取得
float PlayerRot(void);			//方向取得
float PlayerMove(int nRot);		//移動取得
float PlayerRotDesh(void);		//目的の方向取得
D3DXMATRIX Playermtx(void);		//マトリックス取得
bool TargetRockon(void);		//ロックオンしてるか
void RemovePlayerRock(void);	//ロックオン外す
int NumParts(void);				//パーツ何個か
bool CollisionDamage(D3DXVECTOR3 pos, float fRadius);	//当たり判定当たったか
float CollisionPos(int nNumber, int nRot);	//当たり判定の場所
float PlayerLookEnemy(void);				//敵の方を向く
D3DXVECTOR3 EnemyClash(D3DXVECTOR3 pos);	//敵を吹き飛ばす
float ClashRot(D3DXVECTOR3 pos);			//吹き飛ばす方向
float PlayerClashRot(D3DXVECTOR3 pos);		//プレイヤーが吹き飛ぶ反応
int DamageTime(void);			//当たり判定の時間
int Damage(void);				//ダメージ量
int PlayerRockEnemy(void);		//ロックオンしてる敵
float PlayerPartsRot(void);		//パーツの方向
int PlayerKey(void);			//キー何番目か
float PlayerDistance(D3DXVECTOR3 pos);		//プレイヤーとの距離
float PlayerHeight(void);		//プレイヤーの身長
void PlayerDamageTime(int nDamageTime);		//無敵時間設定
bool PlayerDamage(D3DXVECTOR3 pos, float fRadius);	//プレイヤーのダメージ判定
void PlayerDamage(int nDamage, int nDamageTime, float fRoty, D3DXVECTOR3 Clash);	//ダメージ処理
int PlayerHP(void);				//プレイヤーのHP
float PlayerHPRate(void);		//プレイヤーのHPの割合
void PlayerKillAdd(void);		//倒した数加算
int PlayerKill(void);			//倒した数取得
#endif _PLAYER_H_