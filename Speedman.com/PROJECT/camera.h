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
		D3DXVECTOR3 posV;     //現在の視点
		D3DXVECTOR3 posVDest; //目的の視点
		D3DXVECTOR3 posR;     //現在の注視点
		D3DXVECTOR3 posRDest; //目的の注視点
		D3DXVECTOR3 vecU;     //上方向ベクトル
		D3DXVECTOR3 rot;     //カメラの向き
		float fLength;
		D3DXMATRIX mtxProjection; //プロジェクションマトリックス
		D3DXMATRIX mtxView; //ビューマトリックス
	} Camera;
	CCamera();
	~CCamera();
	HRESULT Init(D3DXVECTOR3 ref, float fDistance, D3DXVECTOR3 pos);
	void Uninit();
	void Update();

	float GetRotY();
	float GetRotX();
	void SetPosV(D3DXVECTOR3 pos);
	void SetPosR(D3DXVECTOR3 pos);
	void SetRot();
	D3DXVECTOR3 GetPosR() { return m_camera.posR; }
	D3DXVECTOR3 GetPosV() { return m_camera.posV; }
	void AddRotY(float fRotY);
	void AddRotX(float fRotX);
	void ResetRot();

	static CCamera *Create(D3DXVECTOR3 ref, float fDistance, D3DXVECTOR3 pos);

private:
	Camera m_camera;
	/*CKeyboard *m_pKeyboard;
	CMouse *m_pMouse;*/
};

#endif