//=============================================================================
//スタートUI管理[startui.cpp]
// Author : 三上　航世
//=============================================================================
//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "startui.h"
#include "manager.h"
#include "sound.h"

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CStartUI::CStartUI(PRIORITY Priority) : CUI::CUI(Priority)
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CStartUI::~CStartUI()
{

}

//*****************************************************************************
// 初期化
//***************************************************************************** 
HRESULT CStartUI::Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, D3DXCOLOR col, int nUIType)
{
	CUI::Init(pos, SizeX, SizeY, nType, col);
	size.x = SizeX;
	size.y = SizeY;
	m_nUIType = nUIType;
	m_nTime = 0;
	m_bStop = false;
	m_nFade = 255;
	if (nType == 0)
	{
		CSound::Play(CSound::SOUND_LABEL_READY);
	}
	return S_OK;
}

//*****************************************************************************
// 終了
//***************************************************************************** 
void CStartUI::Uninit()
{
	CUI::Uninit();
}

//*****************************************************************************
// 更新
//***************************************************************************** 
void CStartUI::Update()
{
	switch (m_nUIType)
	{
	case 0:
		if (m_bStop == true)
		{
			m_nTime++;
			if (m_nTime > 50)
			{
				CStartUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f - 100.0f, 0.0f), 200.0f, 80.0f, 16, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1);
				SetDeath(true);
				CSound::Play(CSound::SOUND_LABEL_GO);
			}
		}
		else
		{
			size.x -= 16.4f;
			size.y -= 4.5f;
			if (size.x < 200.0f || size.y < 50.0f)
			{
				size.x = 200.0f;
				size.y = 50.0f;
				m_bStop = true;
			}
			SetSize(size);
		}
		break;
	case 1:
		if (m_nTime <= 0)
		{
			m_nTime++;
			CManager::SetCountdown(false);
		}
		if (m_bStop == false)
		{
			size.x += 14.0f;
			size.y += 9.0f;
			if (size.x > 600.0f || size.y > 250.0f)
			{
				m_bStop = true;
			}
		}
		else
		{
			size.x += 3.0f;
			size.y += 1.7f;
			m_nFade -= 10;
			if (m_nFade < 0)
			{
				SetDeath(true);
			}
			else
			{
				FadeColorChange(m_nFade);
			}
		}
		SetSize(size);
		break;
	default:
		break;
	}
}

//*****************************************************************************
//描画
//***************************************************************************** 
void CStartUI::Draw()
{
	CUI::Draw();
}

//*****************************************************************************
//作成
//***************************************************************************** 
CStartUI *CStartUI::Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, D3DXCOLOR col, int nUIType)
{
	CStartUI *pStartUI = NULL;
	pStartUI = new CStartUI(PRIORITY_UI);		//メモリ確保
													//NULLチェック
	if (pStartUI != NULL)
	{
		pStartUI->Init(pos, SizeX, SizeY, nType, col, nUIType);
	}

	return pStartUI;
}