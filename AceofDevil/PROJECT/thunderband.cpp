//=============================================================================
//
// 雷の帯処理 [thunderband.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "thunderband.h"
#include "renderer.h"
#include "enemy.h"
#include "effect.h"
#include "pauseui.h"

//マクロ定義
#define BAND_WIDTH (40.0f)

//静的メンバ変数
LPDIRECT3DTEXTURE9 CThunderBand::m_pTexture = NULL;

CThunderBand::CThunderBand(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

CThunderBand::~CThunderBand(void)
{

}

//初期化
HRESULT CThunderBand::Init(D3DXVECTOR3 pos, float fRotY, float fLength)
{
	CScene2D::Init(fLength, 100.0f, pos, 1.0f);
	CScene::SetObjType(CScene::OBJTYPE_BULLET);
	SetSize(0.1f);
	SetRot(fRotY);
	Set(fLength, 100.0f, pos);
	m_nLife = 0;
	m_nTex = 0;
	SetTexture(m_nTex, 2);
	return S_OK;
}

//終了
void CThunderBand::Uninit(void)
{
	CScene2D::Uninit();
}

//更新
void CThunderBand::Update(void)
{
	//ポーズ中じゃなかったら
	if (CPauseUi::GetPause() == false)
	{
		//寿命カウント進める
		m_nLife++;
		if (m_nLife % 2 == 0)
		{
			//テクスチャカウント進める
			m_nTex++;
			if (m_nTex >= 2)
			{
				m_nTex = 0;
			}
			SetTexture(m_nTex, 2);
		}
		//寿命切れ、消す
		if (m_nLife >= 10)
		{
			Uninit();
		}
	}
}

//描画
void CThunderBand::Draw(void)
{
	CScene2D::Draw();
}

//作成
CThunderBand *CThunderBand::Create(D3DXVECTOR3 pos, float fRotY, float fLength)
{
	CThunderBand *pThunderBand;
	pThunderBand = new CThunderBand(PRIORITY_CHARA);
	if (pThunderBand != NULL)
	{
		pThunderBand->Init(pos, fRotY, fLength);
		pThunderBand->BindTexture(m_pTexture);
	}
	return pThunderBand;
}

//テクスチャの読み込み
HRESULT CThunderBand::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/lightning.png",
		&m_pTexture);
	return S_OK;
}

//テクスチャの破棄
void CThunderBand::UnLoad(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}