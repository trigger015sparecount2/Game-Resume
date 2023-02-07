//---------------------------
//Author:三上航世
//影(shadow.h)
//---------------------------
#ifndef _SHADOW_H_
#define _SHADOW_H_
#include "main.h"

#define MAX_SHADOW (128)	//最大数の影

//影の構造体
typedef struct
{
	D3DXVECTOR3 pos; //位置
	D3DXVECTOR3 rot; //向き
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	int nLength;
	float fRadius;
	bool bUse;
}Shadow;

//プロトタイプ宣言
HRESULT InitShadow(void);	//初期化
void UninitShadow(void);	//終了
void UpdateShadow(void);	//更新
void DrawShadow(void);		//描画
int SetShadow(D3DXVECTOR3 pos, float fRadius);	//設定
void MoveShadow(int nShadow, D3DXVECTOR3 pos);	//移動
void RemoveShadow(int nShadow);	//外す
void ResetShadow(void);		//リセット
#endif _SHADOW_H_