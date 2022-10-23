//---------------------------
// Author:三上航世
// ロックオン(rockon.cpp)
//---------------------------

#include "plane.h"
#include "manager.h"
#include "renderer.h"
#include "rockon.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CRockon::m_pTexture = NULL;

CRockon::CRockon(PRIORITY Priority) : CUI::CUI(Priority)
{

}

CRockon::~CRockon(void)
{

}

//初期化処理
HRESULT CRockon::Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType)
{
	CUI::Init(pos, SizeX, SizeY, nType, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_bUse = false;
	m_bScreen = false;
	return S_OK;
}

//終了処理
void CRockon::Uninit(void)
{
	if (m_pScene != NULL)
	{
		m_pScene = NULL;
	}
	CUI::Uninit();
}

//更新処理
void CRockon::Update(void)
{	
	if (m_bUse == true)
	{
		if (m_pScene->GetDeath() == true)
		{
			m_pScene = NULL;
			m_bUse = false;
		}
		else
		{
			LPDIRECT3DDEVICE9 pDevice;   //デバイスのポインタ
			D3DXMATRIX mtxView, mtxProjection, mtxEnemy, mtxParent, mtxTrans;
			D3DXMATRIX mtxViewport =
			{ SCREEN_WIDTH / 2, 0, 0, 0, 0, -SCREEN_HEIGHT / 2, 0, 0, 0, 0, 1, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, 1 }; //ビューポート配列
			pDevice = CManager::GetRenderer()->GetDevice();		 //デバイスを取得する
			//敵のワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxEnemy);
			//敵のロックオン位置を反映
			D3DXMatrixTranslation(&mtxTrans, 0.0f, m_fHeight, 0.0f);
			D3DXMatrixMultiply(&mtxEnemy, &mtxEnemy, &mtxTrans);
			//敵のパーツマトリックスの取得、反映
			mtxParent = m_pScene->GetMatrix(0);
			D3DXMatrixMultiply(&mtxEnemy, &mtxEnemy, &mtxParent);
			//D3DXMatrixTranslation(&mtxParent, m_pScene->GetPos().x, m_pScene->GetPos().y, m_pScene->GetPos().z);
			//D3DXMatrixMultiply(&mtxEnemy, &mtxEnemy, &mtxParent);
			//ビュー変換
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			D3DXMatrixMultiply(&mtxEnemy, &mtxEnemy, &mtxView);
			//射影変換
			pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjection);
			D3DXMatrixMultiply(&mtxEnemy, &mtxEnemy, &mtxProjection);
			//ビューポート変換
			D3DXMatrixMultiply(&mtxEnemy, &mtxEnemy, &mtxViewport);
			mtxEnemy._41 /= mtxEnemy._44;
			mtxEnemy._42 /= mtxEnemy._44;
			//mtxEnemy._43 /= mtxEnemy._44;
			if (mtxEnemy._41 <= 0.0f || SCREEN_WIDTH <= mtxEnemy._41 ||
				mtxEnemy._42 <= 0.0f || SCREEN_HEIGHT <= mtxEnemy._42 || mtxEnemy._43 < 0.0f)
			{
				m_bScreen = false;
			}
			else
			{
				SetPos(D3DXVECTOR3(mtxEnemy._41, mtxEnemy._42, 0.0f));
				m_bScreen = true;
				
			}
		}
	}
	
}

//描画処理
void CRockon::Draw(void)
{
	if (m_bUse == true && m_bScreen == true)
	{
		CUI::Draw();
	}
}

void CRockon::SetRockon(CScene *pScene)
{
	m_pScene = pScene;
	
	m_fHeight = m_pScene->GetHeight() * 0.2f;
	m_bUse = true;
}

void CRockon::RemoveRockon(void)
{
	m_pScene = NULL;
	m_bUse = false;
}

CRockon *CRockon::Create()
{
	CRockon *pRockon;
	pRockon = new CRockon(PRIORITY_UI);
	if (pRockon != NULL)
	{
		pRockon->Init(D3DXVECTOR3(200.0f, 200.0f, 0.0f), 35.0f, 35.0f, 0);
	}
	return pRockon;
}