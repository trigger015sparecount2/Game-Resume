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
	D3DXVECTOR3 posV;			//現在の視点
	D3DXVECTOR3 posVDest;		//目的の視点
	D3DXVECTOR3 posR;			//現在の注視点
	D3DXVECTOR3 posRDest;		//目的の注視点
	D3DXVECTOR3 vecU;			//上方向ベクトル
	D3DXVECTOR3 rot;			//カメラの向き
	D3DXVECTOR3 rotDesh;		//目的の向き
	float fLength;				//距離
	int nTime;					//時間
	bool bRot;					//向き
	D3DXMATRIX mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX mtxView;			//ビューマトリックス
} Camera;

//プロトタイプ宣言
HRESULT InitCamera(void);		//初期化
void UninitCamera(void);		//終了
void UpdateCamera(void);		//更新
void SetCamera(void);			//設定
float CameraRot(void);			//方向取得
D3DXVECTOR3 CameraPosV(void);	//視点取得
D3DXVECTOR3 CameraPosR(void);	//注視点取得
float CameraDistance(void);		//カメラの距離取得
float RockCamera(void);			//カメラが向いてる方向から反転
D3DXMATRIX CameraMtxPro(void);	//プロジェクション取得
D3DXMATRIX CameraMtxView(void);	//ビューマトリックス取得
float CameraRotH(void);			//垂直距離取得
float CameraRotV(void);			//水平距離取得
void ResetCamera(void);			//リセット
float CameraRotDesh(void);		//目的の向き取得
#endif _CAMERA_H_