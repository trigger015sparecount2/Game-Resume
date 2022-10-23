//---------------------------
//Author:Oãq¢
//ƒS[ƒ‹(goal.cpp)
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

//‰Šú‰»ˆ—
HRESULT CTrampoline::Init(D3DXVECTOR3 size, D3DXVECTOR3 pos)
{
	CPlane::Init(size, pos, D3DXVECTOR2(1.0f, 1.0f));
	CScene::SetRadius(size.x);
	CScene::SetObjType(OBJCTTYPE_TRAMPOLINE);
	return S_OK;
}

//I—¹ˆ—
void CTrampoline::Uninit()
{
	CPlane::Uninit();
}

//XVˆ—
void CTrampoline::Update()
{

}

//•`‰æˆ—
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