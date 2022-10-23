//---------------------------
//Author:三上航世
//ロックオン(rockon.h)
//---------------------------
#ifndef _ROCKON_H_
#define _ROCKON_H_
#include "main.h"
typedef struct
{
	D3DXVECTOR3 pos; //位置
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	int nNumber;
	bool bUse;
} Rockon;
//プロトタイプ宣言
HRESULT InitRockon(void);
void UninitRockon(void);
void UpdateRockon(void);
void DrawRockon(void);
void SetRockon(D3DXVECTOR3 pos, int nNumber);
void MoveRockon(D3DXVECTOR3 pos);
int RemoveRockon(void);
int RockNumber(void);
#endif _ROCKON_H_