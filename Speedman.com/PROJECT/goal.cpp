//---------------------------
//Author:三上航世
//ゴール(goal.cpp)
//---------------------------
#include "goal.h"
#include "manager.h"
#include "renderer.h"

CGoal::CGoal(int nPriority) : CPlane::CPlane(nPriority)
{

}

CGoal::~CGoal()
{

}

//初期化処理
HRESULT CGoal::Init(D3DXVECTOR3 size, D3DXVECTOR3 pos)
{
	CPlane::Init(size, pos, D3DXVECTOR2(1.0f, 1.0f));
	CScene::SetObjType(OBJCTTYPE_GOAL);
	return S_OK;
}

//終了処理
void CGoal::Uninit()
{
	CPlane::Uninit();
}

//更新処理
void CGoal::Update()
{

}

//描画処理
void CGoal::Draw()
{
#ifdef _DEBUG
	CPlane::Draw();
#endif
}

CGoal *CGoal::Create(D3DXVECTOR3 size, D3DXVECTOR3 pos)
{
	CGoal *pGoal;
	pGoal = new CGoal(4);
	if (pGoal != NULL)
	{
		pGoal->Init(size, pos);
		pGoal->SetTexture(4);
	}
	return pGoal;
}