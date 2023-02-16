//=============================================================================
//
// シーン処理 [camera.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

class CKeyboard;
class CMouse;

class CCamera
{
public:
	//カメラの構造体
	typedef struct
	{
		D3DXVECTOR3 posV;			//現在の視点
		D3DXVECTOR3 posVDest;		//目的の視点
		D3DXVECTOR3 posR;			//現在の注視点
		D3DXVECTOR3 posRDest;		//目的の注視点
		D3DXVECTOR3 vecU;			//上方向ベクトル
		D3DXVECTOR3 rot;			//カメラの向き
		float fLength;				//カメラの距離
		D3DXMATRIX mtxProjection;	//プロジェクションマトリックス
		D3DXMATRIX mtxView;			//ビューマトリックス
	} Camera;

	CCamera();
	~CCamera();
	HRESULT Init(D3DXVECTOR3 ref, float fDistance, D3DXVECTOR3 pos);	//初期化
	void Uninit();	//終了
	void Update();	//更新

	float GetRotY() { return m_camera.rot.y; }	//Y方向取得
	float GetRotX() { return m_camera.rot.x; }	//X方向取得
	void SetPosV(D3DXVECTOR3 pos);		//視点設定
	void SetPosR(D3DXVECTOR3 pos);		//注視点設定
	void SetRot(){ m_camera.rot.y = atan2f(m_camera.posR.x - m_camera.posV.x, m_camera.posR.z - m_camera.posV.z); }	//方向設定
	D3DXVECTOR3 GetPosR() { return m_camera.posR; }	//注視点取得
	D3DXVECTOR3 GetPosV() { return m_camera.posV; }	//視点取得
	void AddRotY(float fRotY);						//Y方向加算
	void AddRotX(float fRotX);						//X方向加算
	void ResetRot() { m_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	//方向リセット

	static CCamera *Create(D3DXVECTOR3 ref, float fDistance, D3DXVECTOR3 pos);	//作成

private:
	Camera m_camera;	//カメラ
};

#endif