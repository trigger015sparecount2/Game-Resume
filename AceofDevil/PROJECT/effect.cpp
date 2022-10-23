//=============================================================================
//
// 爆発処理 [explosion.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "effect.h"
#include "renderer.h"
#include "pauseui.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;

CEffect::CEffect(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

CEffect::~CEffect(void)
{

}

HRESULT CEffect::Init(D3DXVECTOR3 pos, COLORTYPE colType)
{
	CScene2D::Init(EFFECT_SIZE, EFFECT_SIZE, pos, 1.0f);
	CScene2D::SetObjType(CScene::OBJTYPE_EXPLOSION);
	switch (colType)
	{
	case COLORTYPE_WHITE:
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
		break;
	case COLORTYPE_RED:
		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f);
		break;
	case COLORTYPE_LIGHTBLUE:
		m_col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.6f);
		break;
	case COLORTYPE_YELLOW:
		m_col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.6f);
		break;
	case COLORTYPE_BLUE:
		m_col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.6f);
		break;
	case COLORTYPE_GREEN:
		m_col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.6f);
		break;
	}
	m_colType = colType;
	CScene2D::ChangeColor(m_col);
	CScene::SetPos(pos);
	m_fSize = EFFECT_SIZE;
	return S_OK;
}

void CEffect::Uninit(void)
{
	CScene2D::Uninit();
}

void CEffect::Update(void)
{
	if (CPauseUi::GetPause() == false)
	{
		D3DXVECTOR3 pos;
		pos = GetPos();
		m_fSize -= 2.0f;
		Set(m_fSize, m_fSize, pos);
		m_col.a -= 0.05f;
		CScene2D::ChangeColor(m_col);
		if (m_fSize <= 0.0f || m_col.a <= 0.0f)
		{
			Uninit();
		}
	}
}

void CEffect::Draw(void)
{
	CScene2D::Draw();
}

CEffect *CEffect::Create(D3DXVECTOR3 pos, COLORTYPE colType)
{
	CEffect *pEffect;
	pEffect = new CEffect(PRIORITY_EFFECT);
	if (pEffect != NULL)
	{
		pEffect->Init(pos, colType);
		pEffect->BindTexture(m_pTexture);
	}
	return pEffect;
}

HRESULT CEffect::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/particle.png",
		&m_pTexture);
	return S_OK;
}

void CEffect::UnLoad(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

void CEffect::Damage(int nDamage)
{

}

bool CEffect::GetDamage(void)
{
	return false;
}