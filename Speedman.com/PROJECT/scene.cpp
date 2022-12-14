//=============================================================================
//
// シーン処理 [scene.h]
// Author : 三上航世
//
//=============================================================================
#include "scene.h"
#include "Game.h"

//静的メンバ変数
CScene *CScene::m_pTop[PRIORITY] = {};
CScene *CScene::m_pCur[PRIORITY] = {};
int CScene::m_nNumAll = 0;
bool CScene::m_bKey = false;

//=============================================================================
// コンストラクタ
//=============================================================================
CScene::CScene(int nPriority)
{
	m_nPriority = nPriority;

	m_pNext[m_nPriority] = NULL;
	m_bDeath = false;
	if (m_pTop[m_nPriority] == NULL && m_pCur[m_nPriority] == NULL)
	{
		m_pPrev[m_nPriority] = NULL;
		m_pTop[m_nPriority] = this;
		m_pCur[m_nPriority] = this;
	}
	else if (m_pCur != NULL)
	{
		m_pCur[m_nPriority]->m_pNext[m_nPriority] = this;
		m_pPrev[m_nPriority] = m_pCur[m_nPriority];
		m_pCur[m_nPriority] = this;
	}
	m_nNumAll++;
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene::~CScene()
{
	//エフェクトに不具合が発生して勝手に自分のやつに変えました、すまぬ
	int Priority = m_nPriority;

	//消す対象がTopでありCurだった場合
	if (this == m_pTop[Priority] &&
		this == m_pCur[Priority])
	{
		m_pTop[Priority] = NULL;
		m_pNext[Priority] = NULL;
		m_pCur[Priority] = NULL;
		m_pPrev[Priority] = NULL;
	}
	//消す対象がTop
	else if (this == m_pTop[Priority])
	{
		m_pTop[Priority] = m_pNext[Priority];
		m_pPrev[Priority] = NULL;
		m_pNext[Priority]->m_pPrev[Priority] = NULL;
	}
	//消す対象がCur
	else if (this == m_pCur[Priority])
	{
		m_pCur[Priority] = m_pPrev[Priority];
		m_pNext[Priority] = NULL;
		m_pPrev[Priority]->m_pNext[Priority] = NULL;
	}
	//それ以外
	else
	{
		m_pNext[Priority]->m_pPrev[Priority] = m_pPrev[Priority];
		m_pPrev[Priority]->m_pNext[Priority] = m_pNext[Priority];
	}
}

void CScene::ReleaseAll()
{
	//描画処理順を判別
	for (int nCntPriority = 0; nCntPriority < PRIORITY; nCntPriority++)
	{
		CScene *pScene;
		pScene = m_pTop[nCntPriority];
		while (pScene)
		{
			CScene *pSceneNext;
			pSceneNext = pScene->m_pNext[nCntPriority];
			pScene->Uninit();
			pScene = pSceneNext;
		}
	}
}

void CScene::UpdateAll()
{
	//CManager::MODE Mode = CManager::GetMode();

	//描画処理順を判別
	for (int nCntPriority = 0; nCntPriority < PRIORITY; nCntPriority++)
	{
		CScene *pScene;
		pScene = m_pTop[nCntPriority];
		m_bKey = false;
		while (pScene)
		{
			CScene *pSceneNext;
			pSceneNext = pScene->m_pNext[nCntPriority];
			pScene->Update();
			if (pScene->m_bDeath == true)
			{
				pScene->Uninit();
			}
			pScene = pSceneNext;
		}
	}
}

void CScene::DrawAll()
{
	//描画処理順を判別
	for (int nCntPriority = 0; nCntPriority < PRIORITY; nCntPriority++)
	{

		CScene *pScene;
		pScene = m_pTop[nCntPriority];
		while (pScene)
		{
			CScene *pSceneNext;
			pSceneNext = pScene->m_pNext[nCntPriority];
			pScene->Draw();
			pScene = pSceneNext;
		}
	}
}

void CScene::Release()
{
	delete this;
	m_nNumAll--;
}

CScene *CScene::GetScene(int nPriority)
{
	return m_pTop[nPriority];
}

CScene *CScene::GetNext()
{
	return m_pNext[m_nPriority];
}

void CScene::SetDeath(bool bDeath)
{
	m_bDeath = bDeath;
}

D3DXVECTOR3 CScene::GetPos()
{
	return m_pos;
}

void CScene::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

void CScene::SetRadius(float fRadius)
{
	m_fRadius = fRadius;
}

float CScene::GetRadius()
{
	return m_fRadius;
}

//=============================================================================
// オブジェクトタイプの設定
//=============================================================================
void CScene::SetObjType(OBJTYPE obj)
{
	m_objType = obj;
}

//=============================================================================
// オブジェクトタイプの取得
//=============================================================================
CScene::OBJTYPE CScene::GetObjType()
{
	return m_objType;
}

CScene::COLLISION CScene::GetCollision()
{
	return m_Collision;
}

void CScene::SetCollision(COLLISION collision)
{
	m_Collision = collision;
}

void CScene::SetVtxMax(D3DXVECTOR3 VtxMax)
{
	m_vtxMaxModel = VtxMax;
}

void CScene::SetVtxMin(D3DXVECTOR3 VtxMin)
{
	m_vtxMinModel = VtxMin;
}

void CScene::SetSlack(int nSlack)
{
	m_nSlack = nSlack;
}

void CScene::SetCameraPosV(D3DXVECTOR3 posV)
{
	m_CameraPosV = posV;
}

void CScene::SetCameraPosR(D3DXVECTOR3 posR)
{
	m_CameraPosR = posR;
}

//void CScene::SetDeath(bool bDeath)
//{
//	m_bDeath = bDeath;
//}