//---------------------------
//Author:三上航世
//フィールド(field.cpp)
//---------------------------
#include "field.h"
#include "manager.h"
#include "renderer.h"

CField::CField(PRIORITY Priority) : CPlane::CPlane(Priority)
{

}

CField::~CField()
{

}

//初期化処理
HRESULT CField::Init(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR2 Tex, bool bRotX)
{
	CPlane::Init(size, pos, Tex, bRotX);
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//SetPos(pos);
	m_VtxMax = size;
	m_VtxMin = -size;
	m_bRotX = bRotX;
	return S_OK;
}

//終了処理
void CField::Uninit()
{
	CPlane::Uninit();
}

//更新処理
void CField::Update()
{

}

//描画処理
void CField::Draw()
{
	CPlane::Draw();
}

CField *CField::Create(D3DXVECTOR3 size, D3DXVECTOR3 pos, int nTex, D3DXVECTOR2 Tex, bool bRotX)
{
	CField *pField;
	pField = new CField(PRIORITY_PLANE);
	if (pField != NULL)
	{
		pField->Init(size, pos, Tex, bRotX);
		pField->SetTexture(nTex);
	}
	return pField;
}