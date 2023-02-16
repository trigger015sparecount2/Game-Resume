//---------------------------
// Author:三上航世
// 影(shadow.cpp)
//---------------------------
#include "shadow.h"
#include "manager.h"
#include "renderer.h"

CShadow::CShadow(int nPriority) : CPlane::CPlane(nPriority)
{

}

CShadow::~CShadow()
{

}

//初期化処理
HRESULT CShadow::Init(D3DXVECTOR3 size, D3DXVECTOR3 pos)
{
	CPlane::Init(size, pos, D3DXVECTOR2(1.0f, 1.0f));
	m_size = size;
	CScene::SetObjType(OBJECTTYPE_EFFECT);
	m_fLength = 0.0f;
	return S_OK;
}

//終了処理
void CShadow::Uninit()
{
	CPlane::Uninit();
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
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	CPlane::Draw();
	//通常合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//作成
CShadow *CShadow::Create(D3DXVECTOR3 size, D3DXVECTOR3 pos)
{
	CShadow *pShadow;
	pShadow = new CShadow(4);
	if (pShadow != NULL)
	{
		pShadow->Init(size, pos);
		pShadow->SetTexture(4);
	}
	return pShadow;
}

//移動高さ
void CShadow::Move(float fHeight)
{
	D3DXVECTOR3 size;
	size = CPlane::GetSize();
	m_fLength = (fHeight - m_fPosY) * 0.04f;
	size.x += m_fLength;
	size.z += m_fLength;
	SetSize(size);
	ChangeColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_fLength * 0.1f));
}

//移動
void CShadow::MoveY(D3DXVECTOR3 pos, float fRotX)
{
	m_fPosY = pos.y + 1.0f;
	SetSize(m_size);
	SetPos(D3DXVECTOR3(pos.x, m_fPosY, pos.z));
	SetRot(D3DXVECTOR3(fRotX, 0.0f, 0.0f));
}