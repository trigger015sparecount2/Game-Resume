//=============================================================================
//
// 爆発処理 [explosion.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "explosion.h"
#include "renderer.h"
#include "pauseui.h"

//マクロ定義
#define EXPLOSION_SIZE (50.0f)

//静的メンバ変数
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL;

//コンストラクタ
CExplosion::CExplosion(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

//デストラクタ
CExplosion::~CExplosion(void)
{

}

//初期化
HRESULT CExplosion::Init(D3DXVECTOR3 pos)
{
	CScene2D::Init(EXPLOSION_SIZE, EXPLOSION_SIZE, pos, 0.125f);
	CScene2D::SetObjType(CScene::OBJTYPE_EXPLOSION);
	m_nPatternAnim = 0;
	m_nCounterAnim = 0;
	return S_OK;
}

//終了処理
void CExplosion::Uninit(void)
{
	CScene2D::Uninit();
}

//更新
void CExplosion::Update(void)
{
	//ポーズ中でなかったら
	if (CPauseUi::GetPause() == false)
	{
		//アニメーションカウント増加
		m_nCounterAnim++;
		if (m_nCounterAnim >= 4)
		{
			//カウントをリセットしつつアニメーションを進める
			m_nCounterAnim = 0;
			m_nPatternAnim++;
			SetTexture(m_nPatternAnim, 8);

			//アニメーションが1周したら消す
			if (m_nPatternAnim >= 8)
			{
				Uninit();
			}
		}
	}
}

//描画
void CExplosion::Draw(void)
{
	CScene2D::Draw();
}

//作成
CExplosion *CExplosion::Create(D3DXVECTOR3 pos)
{
	CExplosion *pExplosion;
	pExplosion = new CExplosion(PRIORITY_EFFECT);
	if (pExplosion != NULL)
	{
		pExplosion->Init(pos);
		pExplosion->BindTexture(m_pTexture);
	}
	return pExplosion;
}

//テクスチャ読み込み
HRESULT CExplosion::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/explosion000.png",
		&m_pTexture);
	return S_OK;
}

//テクスチャ破棄
void CExplosion::UnLoad(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}