//---------------------------
//Author:三上航世
//ロックオン(rockon.h)
//---------------------------
#ifndef _ROCKON_H_
#define _ROCKON_H_
#include "main.h"

//ロックオン構造体
typedef struct
{
	D3DXVECTOR3 pos; //位置
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	int nNumber;
	bool bUse;
} Rockon;

//プロトタイプ宣言
HRESULT InitRockon(void);	//初期化
void UninitRockon(void);	//終了
void UpdateRockon(void);	//更新
void DrawRockon(void);		//描画
void SetRockon(D3DXVECTOR3 pos, int nNumber);	//設定する
void MoveRockon(D3DXVECTOR3 pos);	//移動
int RemoveRockon(void);		//ロックオン外す
int RockNumber(void);		//ロックオンしてる敵の番号
#endif _ROCKON_H_