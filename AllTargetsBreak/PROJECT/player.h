//--------------------------
//Author:三上航世
//自機の処理(player.h)
//--------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "main.h"
//マクロ定義
#define PLAYER_WIDTH (40)  //ポリゴンの幅
#define PLAYER_HEIGHT (60) //ポリゴンの高さ

//自機の構造体定義
typedef struct
{
	D3DXVECTOR3 pos;    //現在位置
	D3DXVECTOR3 posOld; //前回位置
	D3DXVECTOR3 move;   //移動量
	D3DXVECTOR3 Blockmove;
	int nCounterAnim;   //アニメーションカウンタ
	int nPatternAnim;   //アニメーションパターン
	int nknife;
	int nStep;
	float fIce;
	float fLeft;        //左に動いてるか
	bool bJump;         //ジャンプが可能か
} Player;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void ResetPlayer(void);
void IcePlayer(bool bIce);
#endif _PLAYER_H_
