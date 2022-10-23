//--------------------------
//Author:三上航世
//自機の処理(block.h)
//--------------------------
#ifndef _BLOCK_H_
#define _BLOCK_H_
#include "main.h"

//マクロ定義
#define MAX_BLOCK (64)

typedef enum
{
	BLOCK_NORMAL = 0,
	BLOCK_DOWN,
	BLOCK_UP,
	BLOCK_ICE,
	BLOCK_CARRY,
	BLOCK_WARP,
	BLOCK_MAX
} BLOCKTYPE;

//自機の構造体定義
typedef struct
{
	D3DXVECTOR3 pos;  //位置
	D3DXVECTOR3 Startpos; //初期位置
	D3DXVECTOR3 move; //移動量
	D3DXVECTOR3 Maxmove; //移動量最大値
	BLOCKTYPE BlockType; //ブロックの種類
	float fWidth;     //幅
	float fHeight;    //高さ
	int nTime; //移動時間
	int nMaxTime; //移動時間最大値
	bool bUse;        //使用しているかどうか
} Block;

//プロトタイプ宣言
HRESULT InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void ResetBlock(void);
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXVECTOR3 Maxmove, int nMaxTime, BLOCKTYPE BlockType);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight, D3DXVECTOR3 *pBlockMove, int nType);
D3DXVECTOR3 WarpBlock(int nMaxTime);
#endif _PLAYER_H_
