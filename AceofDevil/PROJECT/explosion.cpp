//=============================================================================
//
// 爆発処理 [explosion.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "explosion.h"
#include "renderer.h"
#include "pauseui.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL;

CExplosion::CExplosion(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

CExplosion::~CExplosion(void)
{

}

HRESULT CExplosion::Init(D3DXVECTOR3 pos)
{
	CScene2D::Init(EXPLOSION_SIZE, EXPLOSION_SIZE, pos, 0.125f);
	CScene2D::SetObjType(CScene::OBJTYPE_EXPLOSION);
	m_nPatternAnim = 0;
	m_nCounterAnim = 0;
	return S_OK;
}

void CExplosion::Uninit(void)
{
	CScene2D::Uninit();
}

void CExplosion::Update(void)
{
	if (CPauseUi::GetPause() == false)
	{
		m_nCounterAnim++;
		if (m_nCounterAnim >= 4)
		{
			/*D3DXVECTOR3 pos;
			pos = GetPos();*/
			m_nCounterAnim = 0;
			m_nPatternAnim++;
			SetTexture(m_nPatternAnim, 8);
			//Set(EXPLOSION_SIZE, EXPLOSION_SIZE, pos);
			if (m_nPatternAnim >= 8)
			{
				Uninit();
			}
		}
	}
}

void CExplosion::Draw(void)
{
	CScene2D::Draw();
}

CExplosion *CExplosion::Create(D3DXVECTOR3 pos)
{
	CExplosion *pExplosion;
	pExplosion = new CExplosion(PRIORITY_EFFECT);
	if (pExplosion != NULL)
	{
		pExplosion->Init(pos);
		pExplosion->BindTexture(m_pTexture);
	}
	return pExplosion;
}

HRESULT CExplosion::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/explosion000.png",
		&m_pTexture);
	return S_OK;
}

void CExplosion::UnLoad(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

void CExplosion::Damage(int nDamage)
{

}

bool CExplosion::GetDamage(void)
{
	return false;
}