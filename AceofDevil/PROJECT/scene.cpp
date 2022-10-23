//=============================================================================
//
// シーン処理 [scene.h]
// Author : 三上航世
//
//=============================================================================
#include "scene.h"

//静的メンバ変数
CScene *CScene::m_apScene[PRIORITY_MAX][MAX_POLYGON] = {};
int CScene::m_nNumAll = 0;
CScene *CScene::m_pTop[PRIORITY_MAX] = {};
CScene *CScene::m_pCur[PRIORITY_MAX] = {};

CScene::CScene(PRIORITY Priority)
{
	int nCntScene;
	for (nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
	{
		if (m_apScene[Priority][nCntScene] == NULL)
		{
			m_apScene[Priority][nCntScene] = this;
			m_nID = nCntScene;
			m_nPriority = Priority;
			m_nNumAll++;
			break;
		}
	}
}

CScene::~CScene(void)
{

}

void CScene::ReleaseAll(void)
{
	int nCntScene1;
	int nCntScene2;
	for (nCntScene1 = 0; nCntScene1 < PRIORITY_MAX; nCntScene1++)
	{
		for (nCntScene2 = 0; nCntScene2 < MAX_POLYGON; nCntScene2++)
		{
			if (m_apScene[nCntScene1][nCntScene2] != NULL)
			{
				m_apScene[nCntScene1][nCntScene2]->Uninit();
			}
		}
	}
}

void CScene::UpdateAll(void)
{
	int nCntScene1;
	int nCntScene2;
	for (nCntScene1 = 0; nCntScene1 < PRIORITY_MAX; nCntScene1++)
	{
		for (nCntScene2 = 0; nCntScene2 < MAX_POLYGON; nCntScene2++)
		{
			if (m_apScene[nCntScene1][nCntScene2] != NULL)
			{
				m_apScene[nCntScene1][nCntScene2]->Update();
			}
		}
	}
}

void CScene::DrawAll(void)
{
	int nCntScene1;
	int nCntScene2;
	for (nCntScene1 = 0; nCntScene1 < PRIORITY_MAX; nCntScene1++)
	{
		for (nCntScene2 = 0; nCntScene2 < MAX_POLYGON; nCntScene2++)
		{
			if (m_apScene[nCntScene1][nCntScene2] != NULL)
			{
				m_apScene[nCntScene1][nCntScene2]->Draw();
			}
		}
	}
}

void CScene::Release(void)
{
	if (m_apScene[m_nPriority][m_nID] != NULL)
	{
		int nID = m_nID;
		int nPriority = m_nPriority;
		delete m_apScene[nPriority][nID];
		m_apScene[nPriority][nID] = NULL;
		m_nNumAll--;
	}
}

void CScene::SetObjType(OBJTYPE objType)
{
	m_objType = objType;
}

CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_objType;
}

CScene *CScene::GetScene(PRIORITY Priority, int nCntScene)
{
	return m_apScene[Priority][nCntScene];
}

CScene *CScene::GetScene(PRIORITY Priority)
{
	return m_pTop[Priority];
}

CScene *CScene::GetNext(void)
{
	return m_pNext;
}

void CScene::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

D3DXVECTOR3 CScene::GetPos(void)
{
	return m_pos;
}

D3DXVECTOR3 *CScene::GetPlayerPos(void) 
{
	int nCntScene;
	for (nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
	{
		if (m_apScene[PRIORITY_CHARA][nCntScene] != NULL)
		{
			if (m_apScene[PRIORITY_CHARA][nCntScene]->m_objType == OBJTYPE_PLAYER)
			{
				return &m_apScene[PRIORITY_CHARA][nCntScene]->m_pos;
			}
		}
	}
	return NULL;
}

void CScene::SetEnemy(bool bEnemy)
{
	m_bEnemy = bEnemy;
}

bool CScene::GetEnemy(void)
{
	return m_bEnemy;
}

void CScene::SetElement(CScene::ELEMENT element)
{
	m_Element = element;
}

CScene::ELEMENT CScene::GetElement(void)
{
	return m_Element;
}

D3DXVECTOR3 *CScene::GetPointerPos(int nCntScene)
{
	return &m_apScene[PRIORITY_CHARA][nCntScene]->m_pos;
}

bool CScene::GetbEnemy(int nEnemy)
{
	if (m_apScene[PRIORITY_CHARA][nEnemy] != NULL && m_apScene[PRIORITY_CHARA][nEnemy]->GetDamage() == true)
	{
		return true;
	}
	return false;
}