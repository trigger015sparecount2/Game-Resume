//---------------------------
//Author:三上航世
//モデル(model.h)
//---------------------------
#ifndef _MODEL_H_
#define _MODEL_H_
#include "main.h"

//プロトタイプ宣言
HRESULT InitModel(void);		//初期化
void UninitModel(void);			//終了
void UpdateModel(void);			//更新
void DrawModel(void);			//描画
float ModelPos(int nRot);		//pos取得
float ModelRot(void);			//rot取得
void ModelMoveX(void);			//モデル移動X
void ModelMoveZ(void);			//モデル移動Z
void ModelMoveXslow(void);		//モデル移動遅くするX
void ModelMoveZslow(void);		//モデル移動遅くするZ
float ModelMove(int nRot);		//移動
float ModelRotDesh(void);		//目的の方向設定
float ModelMinSize(int nRot);	//最小サイズ取得
float ModelMaxSize(int nRot);	//最大サイズ取得
#endif _MODEL_H_