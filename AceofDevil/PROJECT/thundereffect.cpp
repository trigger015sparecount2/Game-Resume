//=============================================================================
//
// 雷弾のエフェクト処理 [thundereffect.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "thundereffect.h"
#include "renderer.h"
#include "player.h"
#include "pauseui.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CThundereffect::m_pTexture = NULL;

CThundereffect::CThundereffect(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

CThundereffect::~CThundereffect(void)
{

}

//初期化
HRESULT CThundereffect::Init(D3DXVECTOR3 pos)
{
	CScene2D::Init(THUNDEREFFECT_SIZE, THUNDEREFFECT_SIZE, pos, 1.0f);
	CScene::SetObjType(CScene::OBJTYPE_NONE);
	return S_OK;
}

//終了
void CThundereffect::Uninit(void)
{
	CScene2D::Uninit();
}

//更新
void CThundereffect::Update(void)
{
	if (CPauseUi::GetPause() == false)
	{
		SetRot(0.9f);
	}
}

//描画
void CThundereffect::Draw(void)
{
	CScene2D::Draw();
}

//作成
CThundereffect *CThundereffect::Create(D3DXVECTOR3 pos)
{
	CThundereffect *pThundereffect;
	pThundereffect = new CThundereffect(PRIORITY_BG);
	if (pThundereffect != NULL)
	{
		pThundereffect->Init(pos);
		pThundereffect->BindTexture(m_pTexture);
		pThundereffect->ChangeColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f));
	}
	return pThundereffect;
}

//テクスチャ読み込み
HRESULT CThundereffect::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/thundereffect.png",
		&m_pTexture);
	return S_OK;
}

//テクスチャ破棄
void CThundereffect::UnLoad(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}