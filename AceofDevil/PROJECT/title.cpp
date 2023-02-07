//=============================================================================
//
// タイトル [title.h]
// Author : 三上航世
//
//=============================================================================
#include "scene2d.h"
#include "input.h"
#include "sound.h"
#include "title.h"
#include "bg.h"
#include "manager.h"
#include "fade.h"
#include "titleui.h"

//静的メンバ変数
CScene2D *CTitle::m_pScene2D = NULL;
LPDIRECT3DTEXTURE9 CTitle::m_pTexture = NULL;

CTitle::CTitle(PRIORITY Priority) : CScene::CScene(Priority)
{

}

CTitle::~CTitle(void)
{

}

//初期化
HRESULT CTitle::Init(float, float, D3DXVECTOR3, float)
{
	CScene::SetObjType(CScene::OBJTYPE_NONE);
	m_pInputKeyboard = CManager::GetInputKeyboard();
	m_pInputGamePad = CManager::GetInputGamePad();
	CBg::Load();
	CTitleUi::Load();
	CBg::Create(false);
	CTitleUi::Create();
	CSound::Play(CSound::SOUND_LABEL_TITLEBGM);
	return S_OK;
}

//終了
void CTitle::Uninit(void)
{
	CBg::UnLoad();
	CTitleUi::UnLoad();
	if (m_pInputGamePad != NULL)
	{
		m_pInputGamePad = NULL;
	}
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard = NULL;
	}
	Release();
}

//更新
void CTitle::Update(void)
{
	//ゲームパッドかキーボードで何か押されたか
	if (m_pInputGamePad != NULL)
	{
		if (m_pInputGamePad->GetAnyButton() == true)
		{
			CFade::SetFade(CManager::MODE_TUTORIAL);
			CSound::Play(CSound::SOUND_LABEL_YES);
		}
	}
	else if (m_pInputKeyboard != NULL)
	{
		if (m_pInputKeyboard->GetAnyKey() == true)
		{
			CFade::SetFade(CManager::MODE_TUTORIAL);
			CSound::Play(CSound::SOUND_LABEL_YES);
		}
	}
}

//描画
void CTitle::Draw(void)
{

}