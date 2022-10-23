//---------------------------
//Author:三上航世
//カメラ(camera.h)
//---------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "main.h"
//カメラの構造体
typedef struct
{
	D3DXVECTOR3 posV;     //現在の視点
	D3DXVECTOR3 posVDest; //目的の視点
	D3DXVECTOR3 posR;     //現在の注視点
	D3DXVECTOR3 posRDest; //目的の注視点
	D3DXVECTOR3 vecU;     //上方向ベクトル
	D3DXVECTOR3 rot;     //カメラの向き
	D3DXVECTOR3 rotDesh;
	float fLength;
	int nTime;
	bool bRot;
	D3DXMATRIX mtxProjection; //プロジェクションマトリックス
	D3DXMATRIX mtxView; //ビューマトリックス
} Camera;
//プロトタイプ宣言
HRESULT InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
float CameraRot(void);
D3DXVECTOR3 CameraPosV(void);
D3DXVECTOR3 CameraPosR(void);
float CameraDistance(void);
float RockCamera(void);
D3DXMATRIX CameraMtxPro(void);
D3DXMATRIX CameraMtxView(void);
float CameraRotH(void);
float CameraRotV(void);
void ResetCamera(void);
float CameraRotDesh(void);
float CameraDistance(D3DXVECTOR3 pos);
#endif _CAMERA_H_