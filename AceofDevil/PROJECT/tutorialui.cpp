//=============================================================================
//
// UI処理 [ui.h]
// Author : 三上航世
//
//=============================================================================
#include "tutorialui.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"
#include "fade.h"

//マクロ定義
#define TEXTFILENAME "data/FILE/tutorial.txt"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CTutorialUi::m_pTexture[2] = {};
CScene2D *CTutorialUi::m_apScene[MAX_TUTOUI] = {};
int CTutorialUi::m_nTextClear = 0;
int CTutorialUi::m_nSquareClear = 0;
LPD3DXFONT CTutorialUi::m_pFont = NULL;
char CTutorialUi::m_Str[MAX_BYTE] = {};
CTutorialUi::TEXTMODE CTutorialUi::m_textmode = CTutorialUi::TEXTMODE_NONE;
bool CTutorialUi::m_bClear = false;

CTutorialUi::CTutorialUi(PRIORITY Priority) : CScene::CScene(Priority)
{

}

CTutorialUi::~CTutorialUi(void)
{

}

HRESULT CTutorialUi::Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//チュートリアルファイルロード
	FILE *pFile;
	int nCount;
	pFile = fopen(TEXTFILENAME, "r");
	if (pFile != NULL)
	{
		for (nCount = 0; nCount < 22; nCount++)
		{
			fgets(&m_aText[nCount][0], MAX_BYTE, pFile);
			//fscanf(pFile, "%s", &m_aText[nCount][0]);
		}
		fclose(pFile);
	}

	int nCntLife = 0;
	//撃墜数四角
	//端
	m_apScene[nCntLife] = CScene2D::Create(14.0f, 40.0f, D3DXVECTOR3(355.0f, 40.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[0]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	nCntLife++;
	//真ん中
	m_apScene[nCntLife] = CScene2D::Create(390.0f, 40.0f, D3DXVECTOR3(640.0f, 40.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->BindTexture(m_pTexture[1]);
	nCntLife++;
	//端
	m_apScene[nCntLife] = CScene2D::Create(14.0f, 40.0f, D3DXVECTOR3(925.0f, 40.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[0]);
	m_apScene[nCntLife]->SetRot(1.0f);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->Set(14.0f, 40.0f, D3DXVECTOR3(925.0f, 40.0f, 0.0f));
	nCntLife++;
	m_nTextClear = 255;
	m_nSquareClear = 255;
	m_nCntText = -1;
	m_textmode = TEXTMODE_NONE;
	m_bClear = false;

	TextChange();
	D3DXCreateFont(pDevice, 20, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "PixelMplus10", &m_pFont);
	
	return S_OK;
}

void CTutorialUi::Uninit(void)
{
	int nCntScene;
	for (nCntScene = 0; nCntScene < MAX_TUTOUI; nCntScene++)
	{
		if (m_apScene[nCntScene] != NULL)
		{
			m_apScene[nCntScene] = NULL;
		}
	}
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
	Release();
}

void CTutorialUi::Update(void)
{
	if (m_textmode != TEXTMODE_NONE)
	{
		if (m_textmode == TEXTMODE_IN)
		{
			m_nTextClear += 25;
			if (m_nTextClear >= 255)
			{
				m_nTextClear = 255;
				m_textmode = TEXTMODE_NONE;
			}
		}
		else if (m_textmode == TEXTMODE_OUT)
		{
			m_nTextClear -= 25;
			if (m_nTextClear <= 0)
			{
				TextChange();
			}
		}
	}
}

void CTutorialUi::Draw(void)
{
	
}

HRESULT CTutorialUi::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/squareside.png",
		&m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/squarecenter.png",
		&m_pTexture[1]);
	return S_OK;
}

void CTutorialUi::UnLoad(void)
{
	int nCntLife;
	for (nCntLife = 0; nCntLife < 2; nCntLife++)
	{
		if (m_pTexture[nCntLife] != NULL)
		{
			m_pTexture[nCntLife]->Release();
			m_pTexture[nCntLife] = NULL;
		}
	}
}

CTutorialUi *CTutorialUi::Create(void)
{
	CTutorialUi *pTutoUi;
	pTutoUi = new CTutorialUi(PRIORITY_UI);
	if (pTutoUi != NULL)
	{
		pTutoUi->Init(0.0f, 0.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f);
	}
	return pTutoUi;
}

void CTutorialUi::DrawTutoText(void)
{
	RECT rect = { SCREEN_WIDTH / 2 - 280, 20, SCREEN_WIDTH / 2 + 280, 60 };

	// テキスト描画
	m_pFont->DrawText(NULL, &m_Str[0], -1, &rect, DT_WORDBREAK, D3DCOLOR_ARGB(CTutorialUi::Compare(), 0, 255, 0));
}

void CTutorialUi::TextChange(void)
{
	m_nCntText++;
	m_nTextClear = 0;
	if (m_nCntText < 11)
	{
		m_textmode = TEXTMODE_IN;
		memset(&m_Str[0], 0, sizeof(m_Str));
		memcpy(&m_Str[0], &m_aText[m_nCntText * 2][0], strlen(m_aText[m_nCntText * 2]));
		memcpy(&m_Str[strlen(m_Str)], &m_aText[m_nCntText * 2 + 1][0], strlen(m_aText[m_nCntText * 2 + 1]));
	}
	else
	{
		m_textmode = TEXTMODE_NONE;
		CFade::SetFade(CManager::MODE_GAME);
	}
}

void CTutorialUi::TutoClear(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	if (pos.x - fWidth <= 925.0f + 14.0f * sinf(0.25f * D3DX_PI) && 355.0f - 14.0f * sinf(0.25f * D3DX_PI) <= pos.x + fWidth &&
		pos.y - fHeight <= 40.0f + 40.0f * cosf(0.25f * D3DX_PI) && 40.0f - 40.0f * cosf(0.25f * D3DX_PI) <= pos.y + fHeight)
	{
		if (m_bClear == false)
		{
			m_bClear = true;
		}
		if (m_nSquareClear > 75)
		{
			m_nSquareClear -= 25;
			if (m_nSquareClear <= 75)
			{
				m_nSquareClear = 75;
			}
			int nCntTuto;
			for (nCntTuto = 0; nCntTuto < 3; nCntTuto++)
			{
				m_apScene[nCntTuto]->ChangeColor(D3DCOLOR_RGBA(0, 255, 0, m_nSquareClear));
			}
		}
	}
	else
	{
		if (m_bClear == true)
		{
			m_bClear = false;
		}
		if (m_nSquareClear < 255)
		{
			m_nSquareClear += 25;
			if (m_nSquareClear >= 255)
			{
				m_nSquareClear = 255;
			}
			int nCntTuto;
			for (nCntTuto = 0; nCntTuto < 3; nCntTuto++)
			{
				m_apScene[nCntTuto]->ChangeColor(D3DCOLOR_RGBA(0, 255, 0, m_nSquareClear));
			}
		}
	}
}

int CTutorialUi::Compare(void)
{
	if (m_nSquareClear >= m_nTextClear)
	{
		return m_nTextClear;
	}
	return m_nSquareClear;
}

void CTutorialUi::NextText(void)
{
	m_textmode = TEXTMODE_OUT;
}

bool CTutorialUi::GetClear(void)
{
	return m_bClear;
}

void CTutorialUi::Damage(int nDamage)
{

}

bool CTutorialUi::GetDamage(void)
{
	return false;
}