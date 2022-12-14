//---------------------------
//Author:三上航世
//ゴール(goal.cpp)
//---------------------------
#include "trampoline.h"
#include "manager.h"
#include "renderer.h"

CTrampoline::CTrampoline(int nPriority) : CPlane::CPlane(nPriority)
{

}

CTrampoline::~CTrampoline()
{

}

//初期化処理
HRESULT CTrampoline::Init(D3DXVECTOR3 size, D3DXVECTOR3 pos)
{
	CPlane::Init(size, pos, D3DXVECTOR2(1.0f, 1.0f));
	CScene::SetRadius(size.x);
	CScene::SetObjType(OBJCTTYPE_TRAMPOLINE);
	return S_OK;
}

//終了処理
void CTrampoline::Uninit()
{
	CPlane::Uninit();
}

//更新処理
void CTrampoline::Update()
{

}

//描画処理
void CTrampoline::Draw()
{
#ifdef _DEBUG
	CPlane::Draw();
#endif
}

CTrampoline *CTrampoline::Create(D3DXVECTOR3 size, D3DXVECTOR3 pos)
{
	CTrampoline *pTrampoline;
	pTrampoline = new CTrampoline(4);
	if (pTrampoline != NULL)
	{
		pTrampoline->Init(size, pos);
		pTrampoline->SetTexture(-1);
	}
	return pTrampoline;
}