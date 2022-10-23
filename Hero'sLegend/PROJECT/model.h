//---------------------------
//Author:三上航世
//モデル(model.h)
//---------------------------
#ifndef _MODEL_H_
#define _MODEL_H_
#include "main.h"
//プロトタイプ宣言
HRESULT InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
float ModelPos(int nRot);
float ModelRot(void);
void ModelMoveX(void);
void ModelMoveZ(void);
void ModelMoveXslow(void);
void ModelMoveZslow(void);
float ModelMove(int nRot);
float ModelRotDesh(void);
float ModelMinSize(int nRot);
float ModelMaxSize(int nRot);
bool GetMove(void);
#endif _MODEL_H_