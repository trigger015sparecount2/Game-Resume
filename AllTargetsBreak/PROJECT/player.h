//--------------------------------
//Author:三上航世
//プレイヤーの処理(player.h)
//--------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "main.h"

//マクロ定義
#define PLAYER_WIDTH (40)  //ポリゴンの幅
#define PLAYER_HEIGHT (60) //ポリゴンの高さ

//自機の構造体定義
typedef struct
{
	D3DXVECTOR3 pos;		//現在位置
	D3DXVECTOR3 posOld;		//前回位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 Blockmove;	//ブロックの移動量
	int nCounterAnim;		//アニメーションカウンタ
	int nPatternAnim;		//アニメーションパターン
	int nCross;				//手裏剣インターバル
	int nStep;				//足音カウント
	float fIce;				//氷に乗ってるか
	float fLeft;			//左に動いてるか
	bool bJump;				//ジャンプが可能か
} Player;

//プロトタイプ宣言
void InitPlayer(void);		//初期化
void UninitPlayer(void);	//終了
void UpdatePlayer(void);	//更新
void DrawPlayer(void);		//描画
Player *GetPlayer(void);	//プレイヤーのポインタ取得
void ResetPlayer(void);		//プレイヤーリセット
void IcePlayer(bool bIce);	//氷に乗った処理
#endif _PLAYER_H_
