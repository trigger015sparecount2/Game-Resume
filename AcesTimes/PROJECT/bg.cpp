//=============================================================================
//
// 背景処理 [bg.cpp]
// Author : 三上航世
//
//=============================================================================
#include "bg.h"
#include "scene2d.h"
#include "manager.h"

CBg::CBg(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

CBg::~CBg(void)
{

}

//初期化
HRESULT CBg::Init(D3DXVECTOR2 size, D3DXVECTOR3 pos, D3DXVECTOR2 TexSize)
{
	CScene2D::Init(pos);
	CScene2D::SetSize(size);
	CScene2D::SetTex(D3DXVECTOR2(0.0f, 0.0f), TexSize);
	CScene2D::Set();
	m_TexSize[0] = D3DXVECTOR2(0.0f, 0.0f);
	m_TexSize[1] = TexSize;
	m_fMaxTex = -TexSize.y;
	return S_OK;
}

//終了
void CBg::Uninit(void)
{
	CScene2D::Uninit();
}

//更新
void CBg::Update(void)
{
	if (CManager::GetPause() == false)
	{
		//スクロールさせていく
		m_TexSize[0].y -= 0.05f;
		m_TexSize[1].y -= 0.05f;
		if (m_TexSize[0].y <= m_fMaxTex)
		{
			m_TexSize[0].y -= m_fMaxTex;
			m_TexSize[1].y -= m_fMaxTex;
		}
		CScene2D::SetTex(m_TexSize[0], m_TexSize[1]);
	}
}

//描画
void CBg::Draw(void)
{
	CScene2D::Draw();
}

//作成
CBg *CBg::Create(D3DXVECTOR2 size, D3DXVECTOR3 pos, D3DXVECTOR2 TexSize, int nTex)
{
	CBg *pBg;
	pBg = new CBg(PRIORITY_PLANE);
	if (pBg != NULL)
	{
		pBg->Init(size, pos, TexSize);
		pBg->SetTexture(nTex);
	}
	return pBg;
}