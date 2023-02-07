//---------------------------
// Author:三上航世
// 影(shadow.cpp)
//---------------------------
#include "shadow.h"
#include "manager.h"
#include "renderer.h"

CShadow::CShadow(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

CShadow::~CShadow()
{

}

//初期化処理
HRESULT CShadow::Init(D3DXVECTOR3 size, D3DXVECTOR3 pos)
{
	CScene2D::Init(pos);
	CScene2D::SetSize(D3DXVECTOR2(size.x, size.y));
	CScene2D::Set();
	return S_OK;
}

//終了処理
void CShadow::Uninit()
{
	CScene2D::Uninit();
}

//更新処理
void CShadow::Update()
{

}

//描画処理
void CShadow::Draw()
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する
	//減算合成の設定
	/*pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);*/
	CScene2D::Draw();
	//通常合成に戻す
	/*pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);*/
}

CShadow *CShadow::Create(D3DXVECTOR3 size, D3DXVECTOR3 pos, int nType)
{
	CShadow *pShadow;
	pShadow = new CShadow(PRIORITY_SHADOW);
	if (pShadow != NULL)
	{
		pShadow->Init(size, pos);
		pShadow->SetTexture(nType);
	}
	return pShadow;
}