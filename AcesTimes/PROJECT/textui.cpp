//=============================================================================
//
// UIのテキスト部分処理 [textui.h]
// Author : 三上航世
//
//=============================================================================
#include "textui.h"
#include "manager.h"
#include "renderer.h"

CTextUI::CTextUI(CScene::PRIORITY Priority) :CScene2D(Priority)
{

}

CTextUI::~CTextUI(void)
{

}

//初期化
HRESULT CTextUI::Init(int nSize, int nColor[4], TEXTTYPE texttype, char aText[128], RECT rect)
{
	memset(&m_aText[0], 0, sizeof(m_aText));
	strcpy(&m_aText[0], &aText[0]);
	int nCnt;
	for (nCnt = 0; nCnt < 4; nCnt++)
	{
		m_nColor[nCnt] = nColor[nCnt];
	}
	m_texttype = texttype;
	m_rect = rect;
	D3DXCreateFont(CManager::GetRenderer()->CRenderer::GetDevice(), nSize, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "PixelMplus10", &m_pFont);
	return S_OK;
}

//終了
void CTextUI::Uninit(void)
{
	memset(&m_aText[0], 0, sizeof(m_aText));
	//頂点バッファの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
	Release();
}

//更新
void CTextUI::Update(void)
{
	
}

//描画
void CTextUI::Draw(void)
{
	// テキスト描画
	switch (m_texttype)
	{
	case TEXT_WORDBREAK:
		m_pFont->DrawText(NULL, &m_aText[0], -1, &m_rect, DT_WORDBREAK, D3DCOLOR_ARGB(m_nColor[0], m_nColor[1], m_nColor[2], m_nColor[3]));
		break;
	case TEXT_RIGHT:
		m_pFont->DrawText(NULL, &m_aText[0], -1, &m_rect, DT_RIGHT, D3DCOLOR_ARGB(m_nColor[0], m_nColor[1], m_nColor[2], m_nColor[3]));
		break;
	default:
		break;
	}
}

//作成
CTextUI *CTextUI::Create(int nSize, int nColor[4], TEXTTYPE texttype, char aText[128], RECT rect)
{
	CTextUI *pTextUI;
	pTextUI = new CTextUI(PRIORITY_PAUSEUI);
	if (pTextUI != NULL)
	{
		pTextUI->Init(nSize, nColor, texttype, aText, rect);
	}
	return pTextUI;
}

//文章セット
void CTextUI::SetText(char aText[128])
{
	memset(&m_aText[0], 0, sizeof(m_aText));
	strcpy(&m_aText[0], &aText[0]);
}

//色変える
void CTextUI::SetColor(int nColor[4])
{
	int nCnt;
	for (nCnt = 0; nCnt < 4; nCnt++)
	{
		m_nColor[nCnt] = nColor[nCnt];
	}
}