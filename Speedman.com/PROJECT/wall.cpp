//---------------------------
//Author:三上航世
//壁(wall.cpp)
//---------------------------
#include "wall.h"
#include "manager.h"
#include "renderer.h"

CWall::CWall(int nPriority) : CPlane::CPlane(nPriority)
{

}

CWall::~CWall()
{

}

//初期化処理
HRESULT CWall::Init(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR2 Tex, D3DXVECTOR3 rot)
{
	CPlane::Init(size, pos, Tex);
	SetRot(rot);
	CScene::SetObjType(CScene::OBJECTTYPE_NONE);
	return S_OK;
}

//終了処理
void CWall::Uninit()
{
	CPlane::Uninit();
}

//更新処理
void CWall::Update()
{

}

//描画処理
void CWall::Draw()
{
	CPlane::Draw();
}

CWall *CWall::Create(D3DXVECTOR3 size, D3DXVECTOR3 pos, int nTex, D3DXVECTOR2 Tex, D3DXVECTOR3 rot)
{
	CWall *pWall;
	pWall = new CWall(0);
	if (pWall != NULL)
	{
		pWall->Init(size, pos, Tex, rot);
		pWall->SetTexture(nTex);
	}
	return pWall;
}
