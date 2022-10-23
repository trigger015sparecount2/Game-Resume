//---------------------------
//Author:三上航世
//オブジェクト(object.cpp)
//---------------------------
#include "checkpoint.h"
#include "manager.h"
#include "renderer.h"

CCheckPoint::CCheckPoint(int nPriority) : CObject(nPriority)
{

}

CCheckPoint::~CCheckPoint()
{

}

//初期化処理
HRESULT CCheckPoint::Init(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot)
{
	CObject::Init(pos, nType, collision, rot, 0, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false);
	CScene::SetObjType(CScene::OBJCTTYPE_CHECKPOINT);
	return S_OK;
}

//終了処理
void CCheckPoint::Uninit()
{
	CObject::Uninit();
}

//更新処理
void CCheckPoint::Update()
{

}

//描画処理
void CCheckPoint::Draw()
{
	CObject::Draw();
}

CCheckPoint *CCheckPoint::Create(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot)
{
	CCheckPoint *pSlack;
	pSlack = new CCheckPoint(2);
	if (pSlack != NULL)
	{
		pSlack->Init(pos, nType, collision, rot);
	}
	return pSlack;
}