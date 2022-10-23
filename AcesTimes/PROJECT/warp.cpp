//=============================================================================
//
// ÉèÅ[ÉvêÊèàóù [warp.h]
// Author : éOè„çqê¢
//
//=============================================================================
#include "warp.h"
#include "manager.h"

CWarp::CWarp(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

CWarp::~CWarp(void)
{

}

HRESULT CWarp::Init(D3DXVECTOR2 size)
{
	CScene2D::Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CScene2D::SetSize(size);
	CScene2D::ColorChange(D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.7f));
	CScene2D::Set();
	m_fClear = 0.7f;
	m_bClear = true;
	m_bDraw = false;
	return S_OK;
}

void CWarp::Uninit(void)
{
	CScene2D::Uninit();
}

void CWarp::Update(void)
{
	if (CManager::GetPause() == false)
	{
		if (m_bDraw == true)
		{
			if (m_bClear == true)
			{
				m_fClear -= 0.01f;
				if (m_fClear <= 0.3f)
				{
					m_fClear = 0.3f;
					m_bClear = false;
				}
			}
			else
			{
				m_fClear += 0.01f;
				if (m_fClear >= 0.7f)
				{
					m_fClear = 0.7f;
					m_bClear = true;
				}
			}
			CScene2D::ColorChange(D3DXCOLOR(1.0f, 0.0f, 1.0f, m_fClear));
		}
	}
}

void CWarp::Draw(void)
{
	if (m_bDraw == true)
	{
		CScene2D::Draw();
	}
}

CWarp *CWarp::Create(D3DXVECTOR2 size)
{
	CWarp *pWarp;
	pWarp = new CWarp(PRIORITY_CHARA);
	if (pWarp != NULL)
	{
		pWarp->Init(size);
		pWarp->SetTexture(8);
	}
	return pWarp;
}