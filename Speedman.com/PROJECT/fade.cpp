//*****************************************************************************
// フェード関連処理(Fade.cpp)
// Author : satou syusuke
//*****************************************************************************
#include "Fade.h"
#include "Scene2D.h"
#include "manager.h"
#include "Title.h"

//*****************************************************************************
//静的
//*****************************************************************************
CFade::FADE CFade::m_afade = CFade::FADE_NONE;
CManager::MODE CFade::m_modenext = CManager::MODE_TITLE;
int CFade::m_ncolorFade = 255;
bool CFade::m_bUninit = false;

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CFade::CFade(int nPriority) : CScene2D(nPriority)
{
	m_afade = CFade::FADE_IN;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CFade::~CFade()
{

}

//*****************************************************************************
//初期化
//*****************************************************************************
HRESULT CFade::Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType)
{
	CScene2D::Init(pos);

	m_afade = CFade::FADE_IN;
	m_bUninit = false;

	CScene2D::SetTexture(nType);
	CScene2D::SetWidth(SizeX);
	CScene2D::SetHeight(SizeY);

	return S_OK;
}

//*****************************************************************************
//終了
//*****************************************************************************
void CFade::Uninit()
{
	CScene2D::Uninit();
}
//*****************************************************************************
//更新
//*****************************************************************************
void CFade::Update()
{
	if (m_afade != FADE_NONE)
	{
		if (m_afade == FADE_IN)
		{
			m_ncolorFade -= 10;
			if (m_ncolorFade <= 0)
			{
				m_ncolorFade = 0;
				m_afade = FADE_NONE;
			}
		}
		else if (m_afade == FADE_OUT)
		{
			m_ncolorFade += 10;
			if (m_ncolorFade >= 255)
			{
				m_ncolorFade = 255;
				m_afade = FADE_IN;
				m_bUninit = true;
			}
		}
	}
	CScene2D::FadeColorChange(m_ncolorFade);

	if (m_bUninit == true)
	{
		//Uninit();
		//SetDeath(true);
		CManager::SetMode(m_modenext);
	}

}

//*****************************************************************************
//描画
//*****************************************************************************
void CFade::Draw()
{
	CScene2D::Draw();
}

//*****************************************************************************
//作成
//*****************************************************************************
CFade *CFade::Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType)
{
	CFade * pFade = NULL;
	pFade = new CFade(6);
	if (pFade != NULL)
	{
		pFade->Init(pos, SizeX, SizeY, nType);
	}
	return pFade;
}

//*****************************************************************************
// フェードセット
//*****************************************************************************
void CFade::SetFade(CManager::MODE ModeNext)
{
	if (m_afade == FADE_NONE)
	{
		m_afade = FADE_OUT;
		m_modenext = ModeNext;
		m_ncolorFade = 0;
		m_bUninit = false;
	}
}
