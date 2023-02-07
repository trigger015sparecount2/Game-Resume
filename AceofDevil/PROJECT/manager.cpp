//=============================================================================
//
// マネージャ [Manager.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"
#include "input.h"
#include "sound.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"

//静的メンバ変数
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputGamePad *CManager::m_pInputGamePad = NULL;
CTitle *CManager::m_pTitle = NULL;
CTutorial *CManager::m_pTutorial = NULL;
CGame *CManager::m_pGame = NULL;
CResult *CManager::m_pResult = NULL;
CManager::MODE CManager::m_mode = CManager::MODE_GAME;

CManager::CManager(void)
{

}

CManager::~CManager(void)
{

}

//初期化
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	m_pRenderer = new CRenderer;
	m_pRenderer->Init(hWnd, bWindow);
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputKeyboard->Init(hInstance, hWnd);
	m_pInputGamePad = new CInputGamePad;
	m_pInputGamePad->Init(hInstance, hWnd);
	CSound::Init(hWnd);
	return S_OK;
}

//終了
void CManager::Uninit(void)
{
	CScene::ReleaseAll();
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}
	if (m_pInputGamePad != NULL)
	{
		m_pInputGamePad->Uninit();
		delete m_pInputGamePad;
		m_pInputGamePad = NULL;
	}
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//更新
void CManager::Update(void)
{
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}
	if (CInputGamePad::GetGamePad() == true)
	{
		m_pInputGamePad->Update();
	}
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}
}

//描画
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//モードの設定
void CManager::SetMode(CManager::MODE mode)
{
	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();
			m_pTitle = NULL;
		}
		break;
	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Uninit();
			m_pTutorial = NULL;
		}
		break;
	case MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			m_pGame = NULL;
		}
		break;
	case MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();
			m_pResult = NULL;
		}
		break;
	default:
		break;
	}
	CScene::ReleaseAll();
	switch (mode)
	{
	case MODE_TITLE:
		m_pTitle = new CTitle;
		m_pTitle->Init(0.0f, 0.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f);
		break;
	case MODE_TUTORIAL:
		m_pTutorial = new CTutorial;
		m_pTutorial->Init(0.0f, 0.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f);
		break;
	case MODE_GAME:
		m_pGame = new CGame;
		m_pGame->Init(0.0f, 0.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f);
		break;
	case MODE_RESULT:
		m_pResult = new CResult;
		m_pResult->Init(0.0f, 0.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f);
		break;
	default:
		break;
	}
	m_mode = mode;
}

//ゲームパッドのポインタ
CInputGamePad *CManager::GetInputGamePad(void)
{
	if (m_pInputGamePad->GetGamePad() == true)
	{
		return m_pInputGamePad;
	}
	return NULL;
}