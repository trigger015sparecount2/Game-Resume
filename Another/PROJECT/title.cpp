//=============================================================================
// タイトル管理[Title.cpp]
// Author : 三上航世
//=============================================================================
//*****************************************************************************
//インクルード
//*****************************************************************************
#if 1
#include "Title.h"
#include "keyboard.h"
#include "UI.h"

#include "game.h"
#include "gamepad.h"
#include "fade.h"

//#include "player.h"
//#include "light.h"

#include "sound.h"
#endif

//*****************************************************************************
//静的
//*****************************************************************************
//int CTitle::m_SerectNam = 1;

//*****************************************************************************
//マクロ
//*****************************************************************************
//#define MAX_TITLESERECT (4)

#if 1
//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CTitle::CTitle(PRIORITY Priority) : CScene(Priority)
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTitle::~CTitle()
{
}
#endif

//*****************************************************************************
// 初期化
//***************************************************************************** 
HRESULT CTitle::Init(D3DXVECTOR3 /*pos*/)
{
	m_pKeyboard = CManager::GetKeyboard();
	m_pGamePad = CManager::GetGamepad();

	//m_SerectNam = 1;
	//SetSerectNum(m_SerectNam);
	CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, 19, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 100.0f, 0.0f), 600.0f, 160.0f, 18, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pUI = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 560.0f, 0.0f), 660.0f, 70.0f, 17, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	m_fClear = 1.0f;
	m_bClear = true;
	m_bButton = false;
	CManager::SetPause(false);
	CSound::Play(CSound::SOUND_LABEL_TITLEBGM);
	m_size = D3DXVECTOR2(660.0f, 70.0f);
	return S_OK;
}

//*****************************************************************************
// 終了
//***************************************************************************** 
void CTitle::Uninit()
{
	CScene::Release();
}

//*****************************************************************************
// kousinn
//***************************************************************************** 
void CTitle::Update()
{
	//まだボタン押されてない
	if (m_bButton == false)
	{
		//ゲームパッド繋がってる
		if (m_pGamePad != NULL)
		{
			//何かボタン押された
			if (m_pGamePad->GetAnyButton() == true)
			{
				//ゲームに移行
				CFade::SetFade(CManager::MODE_GAME);
				CSound::Play(CSound::SOUND_LABEL_OK);
				m_bButton = true;
			}
		}
		//キーボード繋がってる
		else if (m_pKeyboard != NULL)
		{
			//何かキー押された
			if (m_pKeyboard->GetAnyKey() == true)
			{
				CFade::SetFade(CManager::MODE_GAME);
				CSound::Play(CSound::SOUND_LABEL_OK);
				m_bButton = true;
			}
		}
	}
	//ボタン押された
	else
	{
		//サイズ変更
		m_size.x += 12.0f;
		m_size.y -= 5.0f;
		m_pUI->SetSize(m_size);
	}

	//透明にする
	if (m_bClear == true)
	{
		//色薄くする
		m_fClear -= 0.05f;
		if (m_fClear <= 0.0f)
		{
			//濃くしていく
			m_fClear = 0.0f;
			m_bClear = false;
		}
	}
	//色濃くする
	else
	{
		//色濃くする
		m_fClear += 0.05f;
		if (m_fClear >= 1.0f)
		{
			//薄くしていく
			m_fClear = 1.0f;
			m_bClear = true;
		}
	}

	//色替え
	m_pUI->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fClear));
}

#if 1
//*****************************************************************************
//描画
//***************************************************************************** 
void CTitle::Draw()
{

}

//*****************************************************************************
//描画
//***************************************************************************** 
CTitle *CTitle::Create()
{
	CTitle *pTitle = NULL;
	pTitle = new CTitle(PRIORITY_EFFECT);		//メモリ確保
								//NULLチェック
	if (pTitle != NULL)
	{
		pTitle->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	return pTitle;
}
#endif
//
//#if 1
////*****************************************************************************
////選択番号取得
////***************************************************************************** 
//int CTitle::GetSerectNum()
//{
//	return m_SerectNam;
//}
//
////*****************************************************************************
////選択番号セット
////***************************************************************************** 
//void CTitle::SetSerectNum(int nNumSerect)
//{
//	m_SerectNam = nNumSerect;
//}
//#endif