//=============================================================================
//
// 撃破処理 [explosion.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "explosion.h"
#include "renderer.h"
#include "player.h"
#include "keyboard.h"

CExplosion::CExplosion(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

CExplosion::~CExplosion(void)
{

}

//初期化
HRESULT CExplosion::Init(D3DXVECTOR3 pos, float fRadius, int nMaxCount)
{
	CScene2D::Init(pos);
	SetSize(D3DXVECTOR2(fRadius, fRadius));
	ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	SetTex(0, 0.125f);
	Set();
	m_nMax = nMaxCount;
	m_nTexPat = 0;
	return S_OK;
}

//終了
void CExplosion::Uninit(void)
{
	CScene2D::Uninit();
}

//更新
void CExplosion::Update(void)
{
	//ポーズ中じゃない
	if (CManager::GetPause() == false)
	{
		//カウント進める
		m_nCount++;
		if (m_nCount >= m_nMax)
		{
			//テクスチャパターン進める
			m_nCount = 0;
			m_nTexPat++;

			//テクスチャパターンが1周したら消す、そうでなかったらテクスチャを設定する
			if (m_nTexPat >= 8)
			{
				SetDeath(true);
			}
			else
			{
				SetTex(m_nTexPat, 0.125f);
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
CExplosion *CExplosion::Create(D3DXVECTOR3 pos, float fRadius, int nMaxCount)
{
	CExplosion *pExplosion;
	pExplosion = new CExplosion(CScene::PRIORITY_CHARA);
	if (pExplosion != NULL)
	{
		pExplosion->Init(pos, fRadius, nMaxCount);
		pExplosion->SetTexture(16);
	}
	return pExplosion;
}