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
//bool CScene::m_bKey = false;

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

//全消去
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

//全部更新
void CScene::UpdateAll()
{
	//CManager::MODE Mode = CManager::GetMode();

	//描画処理順を判別
	for (int nCntPriority = 0; nCntPriority < PRIORITY; nCntPriority++)
	{
		CScene *pScene;
		pScene = m_pTop[nCntPriority];
		//m_bKey = false;
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

//全部描画
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

//リリース
void CScene::Release()
{
	delete this;
	m_nNumAll--;
}