//---------------------------
//Author:三上航世
//アイテム(item.h)
//---------------------------
#ifndef _ITEM_H_
#define _ITEM_H_
#include "main.h"
//プロトタイプ宣言
HRESULT InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos);
void CollisionItem(D3DXVECTOR3 pos, float fRadius);
void ResetItem(void);
#endif _ITEM_H_