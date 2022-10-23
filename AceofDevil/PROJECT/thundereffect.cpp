//=============================================================================
//
// 弾丸処理 [bullet.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "thundereffect.h"
#include "renderer.h"
#include "player.h"
#include "pauseui.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CThundereffect::m_pTexture = NULL;

CThundereffect::CThundereffect(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

CThundereffect::~CThundereffect(void)
{

}

HRESULT CThundereffect::Init(D3DXVECTOR3 pos)
{
	CScene2D::Init(THUNDEREFFECT_SIZE, THUNDEREFFECT_SIZE, pos, 1.0f);
	CScene::SetObjType(CScene::OBJTYPE_NONE);
	return S_OK;
}

void CThundereffect::Uninit(void)
{
	CScene2D::Uninit();
}

void CThundereffect::Update(void)
{
	if (CPauseUi::GetPause() == false)
	{
		SetRot(0.9f);
	}
}

void CThundereffect::Draw(void)
{
	CScene2D::Draw();
}

CThundereffect *CThundereffect::Create(D3DXVECTOR3 pos)
{
	CThundereffect *pThundereffect;
	pThundereffect = new CThundereffect(PRIORITY_BG);
	if (pThundereffect != NULL)
	{
		pThundereffect->Init(pos);
		pThundereffect->BindTexture(m_pTexture);
		pThundereffect->ChangeColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f));
	}
	return pThundereffect;
}

HRESULT CThundereffect::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/thundereffect.png",
		&m_pTexture);
	return S_OK;
}

void CThundereffect::UnLoad(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

void CThundereffect::Damage(int nDamage)
{

}

bool CThundereffect::GetDamage(void)
{
	return false;
}