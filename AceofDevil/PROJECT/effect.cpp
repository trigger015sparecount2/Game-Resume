//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "effect.h"
#include "renderer.h"
#include "pauseui.h"

//マクロ定義
#define EFFECT_SIZE (20.0f)

//静的メンバ変数
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;

//コンストラクタ
CEffect::CEffect(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

//デストラクタ
CEffect::~CEffect(void)
{

}

//初期化処理
HRESULT CEffect::Init(D3DXVECTOR3 pos, COLORTYPE colType)
{
	CScene2D::Init(EFFECT_SIZE, EFFECT_SIZE, pos, 1.0f);
	CScene2D::SetObjType(CScene::OBJTYPE_EXPLOSION);

	//色替え
	switch (colType)
	{
	case COLORTYPE_WHITE:
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
		break;
	case COLORTYPE_RED:
		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f);
		break;
	case COLORTYPE_LIGHTBLUE:
		m_col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.6f);
		break;
	case COLORTYPE_YELLOW:
		m_col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.6f);
		break;
	case COLORTYPE_BLUE:
		m_col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.6f);
		break;
	case COLORTYPE_GREEN:
		m_col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.6f);
		break;
	}
	m_colType = colType;
	CScene2D::ChangeColor(m_col);
	CScene::SetPos(pos);
	m_fSize = EFFECT_SIZE;
	return S_OK;
}

//終了処理
void CEffect::Uninit(void)
{
	CScene2D::Uninit();
}

//更新処理
void CEffect::Update(void)
{
	if (CPauseUi::GetPause() == false)
	{
		D3DXVECTOR3 pos;
		pos = GetPos();

		//小さくする
		m_fSize -= 2.0f;
		Set(m_fSize, m_fSize, pos);
		
		//色を薄くしていく
		m_col.a -= 0.05f;
		CScene2D::ChangeColor(m_col);

		//かなり小さくなるまたはかなり薄くなる
		if (m_fSize <= 0.0f || m_col.a <= 0.0f)
		{
			Uninit();
		}
	}
}

//描画処理
void CEffect::Draw(void)
{
	CScene2D::Draw();
}

//作成
CEffect *CEffect::Create(D3DXVECTOR3 pos, COLORTYPE colType)
{
	CEffect *pEffect;
	pEffect = new CEffect(PRIORITY_EFFECT);
	if (pEffect != NULL)
	{
		pEffect->Init(pos, colType);
		pEffect->BindTexture(m_pTexture);
	}
	return pEffect;
}

//テクスチャ読み込み
HRESULT CEffect::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/particle.png",
		&m_pTexture);
	return S_OK;
}

//テクスチャ破棄
void CEffect::UnLoad(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}