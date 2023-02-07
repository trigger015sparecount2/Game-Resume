//=============================================================================
//
// ワープ先処理 [warp.h]
// Author : 三上航世
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

//初期化
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

//終了
void CWarp::Uninit(void)
{
	CScene2D::Uninit();
}

//更新
void CWarp::Update(void)
{
	//ポーズ中じゃない
	if (CManager::GetPause() == false)
	{
		//描画する
		if (m_bDraw == true)
		{
			//薄くする
			if (m_bClear == true)
			{
				m_fClear -= 0.01f;
				if (m_fClear <= 0.3f)
				{
					m_fClear = 0.3f;
					m_bClear = false;
				}
			}
			//濃くする
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

//描画
void CWarp::Draw(void)
{
	if (m_bDraw == true)
	{
		CScene2D::Draw();
	}
}

//作成
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