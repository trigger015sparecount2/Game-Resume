//---------------------------
//Author:三上航世
//ビルボード(billbaurd.h)
//---------------------------
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_
#include "main.h"
#define MAX_BILLBOURD (32)

//構造体
typedef struct
{
	D3DXVECTOR3 pos; //位置
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	bool bUse;
} Billboard;

//プロトタイプ宣言
HRESULT InitBillboard(void);		//初期化
void UninitBillboard(void);			//終了
void UpdateBillboard(void);			//更新
void DrawBillboard(void);			//描画
void SetBillboard(D3DXVECTOR3 pos);	//ビルボード設定
int CountBoard(void);				//ビルボードの数
#endif _BILLBOARD_H_