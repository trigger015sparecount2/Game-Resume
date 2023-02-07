//=============================================================================
//
// 背景処理 [bg.h]
// Author : 三上航世
//
//=============================================================================
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"
#include "pauseui.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CBg::m_pTexture = NULL;
bool CBg::m_bScroll = false;

//コンストラクタ
CBg::CBg(PRIORITY Priority) : CScene::CScene(Priority)
{

}

//デストラクタ
CBg::~CBg(void)
{

}

//初期化処理
HRESULT CBg::Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize)
{
	if (m_apScene == NULL)
	{
		m_apScene = CScene2D::Create(fWidth, fHeight, pos, PRIORITY_BG);
		m_apScene->ScrollTexture(0.0f);
		m_apScene->BindTexture(m_pTexture);
	}
	m_fTexY = 0.0f;
	return S_OK;
}

//終了処理
void CBg::Uninit(void)
{
	if (m_apScene != NULL)
	{
		m_apScene->Uninit();
		m_apScene = NULL;
	}
	Release();
}

//更新処理
void CBg::Update(void)
{
	//スクロールさせる場合
	if (m_bScroll == true)
	{
		//ポーズ中じゃなかったら
		if (CPauseUi::GetPause() == false)
		{
			//スクロールさせる
			m_fTexY -= 0.1f;
			//スクロールさせすぎたら元に戻す
			if (m_fTexY < -15.0f)
			{
				m_fTexY = 0.0f;
			}
			//スクロールさせる
			m_apScene->ScrollTexture(m_fTexY);
		}
	}
}

//描画処理
void CBg::Draw(void)
{

}

//テクスチャ読み込み
HRESULT CBg::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/sea.png",
		&m_pTexture);
	return S_OK;
}

//テクスチャ破棄
void CBg::UnLoad(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//作成
CBg *CBg::Create(bool bScroll)
{
	CBg *pBg;
	pBg = new CBg(PRIORITY_BG);
	if (pBg != NULL)
	{
		pBg->Init(SCREEN_WIDTH / 1.4f, SCREEN_HEIGHT / 1.4f , D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 1.0f);
		m_bScroll = bScroll;
	}
	return pBg;
}