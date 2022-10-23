//---------------------------
//Author:三上航世
//オブジェクト(object.cpp)
//---------------------------
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"

//静的メンバ変数
CModel *CObject::m_paModel[8] = {};

CObject::CObject(int nPriority) : CScene3D(nPriority)
{

}

CObject::~CObject()
{

}

//初期化処理
HRESULT CObject::Init(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot, int nMoveTime, D3DXVECTOR3 move, bool bDrop)
{
	D3DXVECTOR3 VtxMax, VtxMin;
	m_pModel = m_paModel[nType];

	VtxMax = m_pModel->GetMaxSize();
	VtxMin = m_pModel->GetMinSize();
	float fRadius = (VtxMax.x - VtxMin.x) / 2;
	if (fRadius < (VtxMax.y - VtxMin.y) / 2)
	{
		fRadius = (VtxMax.y - VtxMin.y) / 2;
	}
	if (fRadius < (VtxMax.z - VtxMin.z) / 2)
	{
		fRadius = (VtxMax.z - VtxMin.z) / 2;
	}
	SetVtxMax(VtxMax);
	SetVtxMin(VtxMin);
	SetRadius(fRadius);
	SetCollision(collision);
	SetObjType(CScene::OBJECTTYPE_BLOCK);
	SetRot(rot);
	SetPos(pos);
	m_nMoveTime = nMoveTime;
	m_nTime = 0;
	m_MaxMove = move;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bPlus = true;
	m_bDrop[0] = bDrop;
	m_bDrop[1] = false;
	m_pos = pos;
	return S_OK;
}

//終了処理
void CObject::Uninit()
{
	m_pModel = NULL;
	CScene3D::Uninit();
}

//更新処理
void CObject::Update()
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posOld = pos;
	D3DXVECTOR3 rot = GetRot();
	bool bPause = CManager::GetPause();

	if (bPause == false)
	{

		if (m_nMoveTime > 0)
		{
			if (m_bDrop[0] == false)
			{
				if (m_bPlus == true)
				{
					m_move.x += (m_MaxMove.x - m_move.x) * 0.01f;
					m_move.z += (m_MaxMove.z - m_move.z) * 0.01f;
				}
				else
				{
					m_move.x -= (m_MaxMove.x + m_move.x) * 0.01f;
					m_move.z -= (m_MaxMove.z + m_move.z) * 0.01f;
				}
				m_nTime++;
				if (m_nMoveTime < m_nTime)
				{
					m_nTime = 0;
					if (m_bPlus == true)
					{
						m_bPlus = false;
					}
					else
					{
						m_bPlus = true;
					}
				}
				pos += m_move;
			}
			else
			{
				if (m_bDrop[1] == false)
				{
					m_move.y -= 0.3f;
					pos += m_move;
					if (pos.y <= -50.0f)
					{
						m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						pos = m_pos;
						m_bDrop[1] = false;
					}
				}
				else
				{
					m_move.x += (m_MaxMove.x - m_move.x) * 0.01f;
					m_move.z += (m_MaxMove.z - m_move.z) * 0.01f;
					m_move.y -= 0.3f;
					pos += m_move;
					rot.x -= 0.05f;
				}
				m_bDrop[1] = Land(&pos, posOld);
			}
		}
		SetRot(rot);
		SetPos(pos);
	}
}

//描画処理
void CObject::Draw()
{
	m_pModel->DrawObject(GetPos(), GetRot());
}

CObject *CObject::Create(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot, int nMoveTime, D3DXVECTOR3 move, bool bDrop)
{
	CObject *pObject;
	pObject = new CObject(2);
	if (pObject != NULL)
	{
		pObject->Init(pos, nType, collision, rot, nMoveTime, move, bDrop);
	}
	return pObject;
}

void CObject::Load(int nCnt, const char *aModelName)
{
	m_paModel[nCnt] = CModel::Create(aModelName);
}

void CObject::UnLoad()
{
	int nCntModel;
	for (nCntModel = 0; nCntModel < 8; nCntModel++)
	{
		if (m_paModel[nCntModel] != NULL)
		{
			m_paModel[nCntModel]->Uninit();
			m_paModel[nCntModel] = NULL;
		}
	}
}

bool CObject::Land(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld)
{
	CScene *pScene;
	pScene = GetScene(0);
	while (pScene)
	{
		CScene *pSceneNext;
		pSceneNext = pScene->GetNext();
		if (pScene->GetObjType() == CScene::OBJECTTYPE_FIELD)
		{
			D3DXVECTOR3 pos = pScene->GetPos();
			D3DXVECTOR3 SceneVtxMax = pScene->GetVtxMax();
			D3DXVECTOR3 SceneVtxMin = pScene->GetVtxMin();
			D3DXVECTOR3 VtxMax = GetVtxMax();
			D3DXVECTOR3 VtxMin = GetVtxMin();
			if (pPos->x + VtxMax.x <= pos.x + SceneVtxMax.x && pos.x + SceneVtxMin.x <= pPos->x + VtxMax.x &&
				pPos->z + VtxMax.z <= pos.z + SceneVtxMax.z && pos.z + SceneVtxMin.z <= pPos->z + VtxMax.z)
			{
				float fRotX = atan2f(SceneVtxMax.y - SceneVtxMin.y, SceneVtxMax.z - SceneVtxMin.z);
				float fFieldY = (pPos->z - (pos.z + SceneVtxMin.z * 0.75f)) * tanf(fRotX) + pos.y + SceneVtxMin.y * 0.75f;
				if (posOld.y + VtxMin.y + 3.0f >= fFieldY && fFieldY >= pPos->y + VtxMin.y - 3.0f)
				{
					m_move.y = 0.0f;
					pPos->y = fFieldY - VtxMin.y;
					return true;
				}

			}
		}
		pScene = pSceneNext;
	}
	return false;
}