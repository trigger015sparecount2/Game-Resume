//---------------------------
//Author:三上航世
//残像処理(orbit.h)
//---------------------------
#ifndef _ORBIT_H_
#define _ORBIT_H_
#include "main.h"

#define MAX_ORBIT (256)	//最大残像

//プロトタイプ宣言
HRESULT InitOrbit(void);	//初期化
void UninitOrbit(void);		//終了
void UpdateOrbit(void);		//更新
void DrawOrbit(void);		//描画
void SetOrbit(D3DXVECTOR3 pos, D3DXVECTOR3 Tippos, D3DXVECTOR3 posOld, D3DXVECTOR3 TipposOld, int nTime);	//設定
#endif _ORBIT_H_