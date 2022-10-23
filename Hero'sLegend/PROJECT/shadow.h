//---------------------------
//Author:三上航世
//影(shadow.h)
//---------------------------
#ifndef _SHADOW_H_
#define _SHADOW_H_
#include "main.h"
#define MAX_SHADOW (128)
typedef struct
{
	D3DXVECTOR3 pos; //位置
	D3DXVECTOR3 rot; //向き
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	int nLength;
	float fRadius;
	bool bUse;
}Shadow ;
//プロトタイプ宣言
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 pos, float fRadius);
void MoveShadow(int nShadow, D3DXVECTOR3 pos);
void RemoveShadow(int nShadow);
void ResetShadow(void);
#endif _SHADOW_H_