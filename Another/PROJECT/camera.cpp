//---------------------------
//Author:三上航世
//カメラ(camera.cpp)
//---------------------------
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "enemy.h"

CCamera::CCamera()
{

}

CCamera::~CCamera()
{

}

//初期化処理
HRESULT CCamera::Init(D3DXVECTOR3 ref, float fDistance, D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する
	m_camera.posV = ref;
	m_camera.posVDest = ref;
	m_camera.posR = pos;
	m_camera.posRDest = pos;
	m_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_camera.rot = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_camera.rotDesh = m_camera.rot;
	m_camera.fLength = fDistance;		//距離
	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_camera.mtxProjection);
	//プロジェクションマトリックス作成
	D3DXMatrixPerspectiveFovLH(&m_camera.mtxProjection, D3DXToRadian(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 30000.0f);
	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_camera.mtxProjection);
	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_camera.mtxView);
	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_camera.mtxView, &m_camera.posV, &m_camera.posR, &m_camera.vecU);
	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_camera.mtxView);
	
	return S_OK;
}

//終了処理
void CCamera::Uninit()
{

}

//更新処理
void CCamera::Update()
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する

	//ゲーム中
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		D3DXVECTOR3 pos = m_pPlayer->GetPos();
		if (m_bRockon == true)
		{
			if (m_camera.bRot[1] == true && m_camera.bAdd[1] == false)
			{
				//Y方向回転
				if (m_camera.rot.y != m_camera.rotDesh.y)
				{
					if (m_camera.rotDesh.y - m_camera.rot.y < -D3DX_PI)
					{
						m_camera.rot.y += (m_camera.rotDesh.y - m_camera.rot.y + D3DX_PI * 2) * 0.1f;
					}
					else if (m_camera.rotDesh.y - m_camera.rot.y > D3DX_PI)
					{
						m_camera.rot.y += (m_camera.rotDesh.y - m_camera.rot.y - D3DX_PI * 2) * 0.1f;
					}
					else
					{
						m_camera.rot.y += (m_camera.rotDesh.y - m_camera.rot.y) * 0.1f;
					}
				}
				//ある程度揃ったらやめる
				if (fabsf(m_camera.rotDesh.y - m_camera.rot.y) < 0.3f)
				{
					m_camera.bRot[1] = false;
				}
			}
			if (m_camera.bRot[0] == true && m_camera.bAdd[0] == false)
			{
				//X方向回転
				if (m_camera.rot.x != m_camera.rotDesh.x)
				{
					if (m_camera.rotDesh.x - m_camera.rot.x < -D3DX_PI)
					{
						m_camera.rot.x += (m_camera.rotDesh.x - m_camera.rot.x + D3DX_PI * 2) * 0.1f;
					}
					else if (m_camera.rotDesh.x - m_camera.rot.x > D3DX_PI)
					{
						m_camera.rot.x += (m_camera.rotDesh.x - m_camera.rot.x - D3DX_PI * 2) * 0.1f;
					}
					else
					{
						m_camera.rot.x += (m_camera.rotDesh.x - m_camera.rot.x) * 0.1f;
					}
				}
				//ある程度揃ったらやめる
				if (fabsf(m_camera.rotDesh.x - m_camera.rot.x) < 0.1f)
				{
					m_camera.bRot[0] = false;
				}
			}
		}

		//ゲームクリア、ゲームオーバー時じゃない
		if (CManager::GetGameClear() == false && CManager::GetGameEnd() == false)
		{
			//カメラ移動させる
			m_camera.posR.x = pos.x;
			m_camera.posR.y = pos.y + m_pPlayer->GetHeight() * tanf(m_camera.rot.x);
			m_camera.posR.z = pos.z;
			m_camera.posV.x = pos.x - (sinf(m_camera.rot.y))/* * tanf(m_camera.rot.x))*/ * m_camera.fLength;
			m_camera.posV.y = pos.y + m_pPlayer->GetHeight() * 1.5f/* + m_camera.fLength * (1.0f - tanf(m_camera.rot.x))*/;
			m_camera.posV.z = pos.z - (cosf(m_camera.rot.y))/* * tanf(m_camera.rot.x))*/ * m_camera.fLength;
		}
	}
	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_camera.mtxView);
	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_camera.mtxView, &m_camera.posV, &m_camera.posR, &m_camera.vecU);
	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_camera.mtxView);
}

//設定処理
CCamera *CCamera::Create(D3DXVECTOR3 ref, float fDistance, D3DXVECTOR3 pos)
{
	CCamera *pCamera;
	pCamera = new CCamera;
	if (pCamera != NULL)
	{
		pCamera->Init(ref, fDistance, pos);
	}
	return pCamera;
}

//視点設定
void CCamera::SetPosV(D3DXVECTOR3 pos)
{
	m_camera.posVDest = pos;
	m_camera.posV = pos;
}

//注視点設定
void CCamera::SetPosR(D3DXVECTOR3 pos)
{
	m_camera.posRDest = pos;
	m_camera.posR = pos;
}

//横向き加算
void CCamera::AddRotY(float fRotY)
{
	m_camera.rot.y += fRotY;
	if (m_camera.rot.y < -D3DX_PI)
	{
		m_camera.rot.y += D3DX_PI * 2.0f;
	}
	else if (m_camera.rot.y > D3DX_PI)
	{
		m_camera.rot.y -= D3DX_PI * 2.0f;
	}
	m_camera.bAdd[1] = true;
}

//縦向き加算
void CCamera::AddRotX(float fRotX)
{
	m_camera.rot.x += fRotX;
	//カメラ高さ制限
	if (m_camera.rot.x > 1.0f)
	{
		m_camera.rot.x = 1.0f;
	}
	else if (m_camera.rot.x < 0.4f)
	{
		m_camera.rot.x = 0.4f;
	}
	m_camera.bAdd[0] = true;
}

//カメラ移動
void CCamera::MoveX(float fMove)
{
	m_camera.posR.x += sinf(m_camera.rot.y + D3DX_PI * 0.5f) * fMove;
	m_camera.posR.z += cosf(m_camera.rot.y + D3DX_PI * 0.5f) * fMove;
	m_camera.posV.x += sinf(m_camera.rot.y + D3DX_PI * 0.5f) * fMove;
	m_camera.posV.z += cosf(m_camera.rot.y + D3DX_PI * 0.5f) * fMove;
}

//カメラ移動
void CCamera::MoveZ(float fMove)
{
	m_camera.posR.x -= sinf(m_camera.rot.y) * fMove;
	m_camera.posR.z -= cosf(m_camera.rot.y) * fMove;
	m_camera.posV.x -= sinf(m_camera.rot.y) * fMove;
	m_camera.posV.z -= cosf(m_camera.rot.y) * fMove;
}

//カメラ向きリセット
void CCamera::ResetRot()
{
	m_camera.rot = D3DXVECTOR3(0.8f, 0.0f, 0.0f);
}

//カメラを向かせる向き設定X
void CCamera::SetRotDeshX(float fRotX)
{
	m_camera.rotDesh.x = fRotX;
	m_camera.bRot[0] = true;
	/*
	if (m_camera.rotDesh.x > 1.0f)
	{
		m_camera.rotDesh.x = 1.0f;
	}
	else if (m_camera.rotDesh.x < 0.4f)
	{
		m_camera.rotDesh.x = 0.4f;
	}
	*/
}

//カメラを向かせる向き設定Y
void CCamera::SetRotDeshY(float fRotY)
{
	m_camera.rotDesh.y = fRotY;
	m_camera.bRot[1] = true;
	if (m_camera.rotDesh.y >= D3DX_PI)
	{
		m_camera.rotDesh.y -= D3DX_PI * 2;
	}
	if (m_camera.rotDesh.y < -D3DX_PI)
	{
		m_camera.rotDesh.y += D3DX_PI * 2;
	}
}

//カメラの向き加算させる
void CCamera::SetAdd(bool bAdd)
{
	m_camera.bAdd[0] = bAdd;
	m_camera.bAdd[1] = bAdd;
}