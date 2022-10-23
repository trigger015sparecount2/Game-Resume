//=============================================================================
//
// 背景処理 [bg.h]
// Author : 三上航世
//
//=============================================================================
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"
#include "pauseui.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CBg::m_pTexture = NULL;
bool CBg::m_bScroll = false;

CBg::CBg(PRIORITY Priority) : CScene::CScene(Priority)
{

}

CBg::~CBg(void)
{

}

HRESULT CBg::Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize)
{
	if (m_apScene == NULL)
	{
		m_apScene = CScene2D::Create(fWidth, fHeight, pos, PRIORITY_BG);
		m_apScene->ScrollTexture(0.0f);
		m_apScene->BindTexture(m_pTexture);
	}
	m_fTexY = 0.0f;
	return S_OK;
}

void CBg::Uninit(void)
{
	if (m_apScene != NULL)
	{
		m_apScene->Uninit();
		m_apScene = NULL;
	}
	Release();
}

void CBg::Update(void)
{
	if (m_bScroll == true)
	{
		if (CPauseUi::GetPause() == false)
		{
			m_fTexY -= 0.1f;
			if (m_fTexY < -15.0f)
			{
				m_fTexY = 0.0f;
			}
			m_apScene->ScrollTexture(m_fTexY);
		}
	}
}

void CBg::Draw(void)
{

}

HRESULT CBg::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/sea.png",
		&m_pTexture);
	return S_OK;
}

void CBg::UnLoad(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

CBg *CBg::Create(bool bScroll)
{
	CBg *pBg;
	pBg = new CBg(PRIORITY_BG);
	if (pBg != NULL)
	{
		pBg->Init(SCREEN_WIDTH / 1.4f, SCREEN_HEIGHT / 1.4f , D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 1.0f);
		m_bScroll = bScroll;
	}
	return pBg;
}

void CBg::Damage(int nDamage)
{

}

bool CBg::GetDamage(void)
{
	return false;
}