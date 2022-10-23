//=============================================================================
//
// 弾丸処理 [bullet.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "ice.h"
#include "renderer.h"
#include "enemy.h"
#include "effect.h"
#include "bullet.h"
#include "pauseui.h"
#include "prize.h"
#include "ui.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CIce::m_pTexture = NULL;

CIce::CIce(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

CIce::~CIce(void)
{

}

HRESULT CIce::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRotY)
{
	CScene2D::Init(ICE_SIZE, ICE_SIZE, pos, 1.0f);
	CScene2D::SetSize(0.1f);
	CScene2D::SetRot(fRotY);
	CScene2D::Set(ICE_SIZE, ICE_SIZE, pos);
	CScene::SetObjType(CScene::OBJTYPE_BULLET);
	m_bUninit = false;
	m_move.x = move.x * sinf(fRotY * D3DX_PI);
	m_move.y = move.y * cosf(fRotY * D3DX_PI);
	m_nListCnt = 0;
	m_nEffect = 0;
	int nCntEnemy;
	for (nCntEnemy = 0; nCntEnemy < ICE_MAX_ENEMY; nCntEnemy++)
	{
		m_aEnemies[nCntEnemy] = -1;
	}
	return S_OK;
}

void CIce::Uninit(void)
{
	CScene2D::Uninit();
}

void CIce::Update(void)
{
	if (CPauseUi::GetPause() == false)
	{
		if (m_bUninit == false)
		{
			D3DXVECTOR3 pos = GetPos();
			pos += m_move;
			Set(ICE_SIZE, ICE_SIZE, pos);
			m_nEffect++;
			if (m_nEffect >= 2)
			{
				CEffect::Create(pos, CEffect::COLORTYPE_LIGHTBLUE);
				m_nEffect = 0;
			}
			int nCntScene;
			for (nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
			{
				//敵へのダメージ
				CScene *pScene1;
				pScene1 = GetScene(PRIORITY_CHARA, nCntScene);
				if (pScene1 != NULL)
				{
					OBJTYPE objType;
					objType = pScene1->GetObjType();
					if (objType == OBJTYPE_ENEMY && EnemyList(nCntScene) == true)
					{
						D3DXVECTOR3 posEnemy;
						posEnemy = pScene1->GetPos();
						if (posEnemy.x - ENEMY_SIZE * sinf(0.25f * D3DX_PI) <= pos.x + ICE_SIZE && pos.x - ICE_SIZE <= posEnemy.x + ENEMY_SIZE * sinf(0.25f * D3DX_PI) &&
							posEnemy.y - ENEMY_SIZE * cosf(0.25f * D3DX_PI) <= pos.y + ICE_SIZE && pos.y - ICE_SIZE <= posEnemy.y + ENEMY_SIZE * cosf(0.25f * D3DX_PI))
						{
							if (pScene1->GetDamage() == true)
							{
								pScene1->Damage(2);
								ListPush(nCntScene);
							}
						}
					}
					pScene1 = NULL;
				}

				//弾丸消し
				CScene *pScene2;
				pScene2 = GetScene(PRIORITY_BULLET, nCntScene);
				if (pScene2 != NULL)
				{
					if (pScene2->GetEnemy() == true)
					{
						ELEMENT element;
						element = pScene2->GetElement();
						if (element == ELEMENT_FIRE)
						{
							D3DXVECTOR3 posBullet;
							posBullet = pScene2->GetPos();
							if (posBullet.x - BULLET_SIZE * sinf(0.25f * D3DX_PI) <= pos.x + ICE_SIZE && pos.x - ICE_SIZE <= posBullet.x + BULLET_SIZE * sinf(0.25f * D3DX_PI) &&
								posBullet.y - BULLET_SIZE * cosf(0.25f * D3DX_PI) <= pos.y + ICE_SIZE && pos.y - ICE_SIZE <= posBullet.y + BULLET_SIZE * cosf(0.25f * D3DX_PI))
							{
								CPrize::Create(pos, D3DXVECTOR3(20.0f, 20.0f, 0.0f), ELEMENT_FIRE, 5);
								CUi::SetScore(120);
								pScene2->Uninit();
							}
						}
					}
					pScene2 = NULL;
				}
			}
			if (pos.x <= -ICE_SIZE || SCREEN_WIDTH + ICE_SIZE <= pos.x ||
				pos.y <= -ICE_SIZE || SCREEN_HEIGHT + ICE_SIZE <= pos.y)
			{
				m_bUninit = true;
			}
		}
		else
		{
			Uninit();
		}
	}
}

void CIce::Draw(void)
{
	CScene2D::Draw();
}

CIce *CIce::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRotY)
{
	CIce *pIce;
	pIce = new CIce(PRIORITY_BULLET);
	if (pIce != NULL)
	{
		pIce->Init(pos, move, fRotY);
		pIce->BindTexture(m_pTexture);
	}
	return pIce;
}

HRESULT CIce::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ice.png",
		&m_pTexture);
	return S_OK;
}

void CIce::UnLoad(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

void CIce::Damage(int nDamage)
{

}

bool CIce::GetDamage(void)
{
	return false;
}

bool CIce::EnemyList(int nEnemy)
{
	int nCntEnemy;
	for (nCntEnemy = 0; nCntEnemy < ICE_MAX_ENEMY; nCntEnemy++)
	{
		if (m_aEnemies[nCntEnemy] == nEnemy)
		{
			return false;
		}
	}
	return true;
}

void CIce::ListPush(int nEnemy)
{
	m_aEnemies[m_nListCnt] = nEnemy;
	m_nListCnt++;
	if (m_nListCnt >= ICE_MAX_ENEMY)
	{
		m_nListCnt = 0;
	}
}