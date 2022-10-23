//---------------------------
//Author:三上航世
//ビルボード(billbaurd.h)
//---------------------------
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_
#include "main.h"
#define MAX_BILLBOURD (32)
typedef struct
{
	D3DXVECTOR3 pos; //位置
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	bool bUse;
} Billboard;
//プロトタイプ宣言
HRESULT InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);
void SetBillboard(D3DXVECTOR3 pos);
int CountBoard(void);
#endif _BILLBOARD_H_