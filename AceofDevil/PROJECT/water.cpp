//=============================================================================
//
// 水レーザー処理 [water.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "water.h"
#include "renderer.h"
#include "enemy.h"
#include "scene.h"
#include "bullet.h"
#include "pauseui.h"
#include "prize.h"
#include "ui.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CWater::m_pTexture = NULL;

CWater::CWater(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

CWater::~CWater(void)
{

}

HRESULT CWater::Init(D3DXVECTOR3 pos)
{
	CScene2D::Init(WATER_SIZE, WATER_SIZE, pos, 1.0f);
	CScene::SetObjType(CScene::OBJTYPE_BULLET);
	CScene::SetPos(pos);
	CScene2D::SetSize(0.08f);
	Set(0.0f, 0.0f, pos);
	m_pPlayerpos = CScene::GetPlayerPos();
	m_fRotY = D3DX_PI;
	m_fRotDeshY = D3DX_PI;
	m_fSize = 0.0f;
	m_bSize = false;
	m_nListCnt = 0;
	m_bDraw = false;
	int nCntEnemy;
	for (nCntEnemy = 0; nCntEnemy < WATER_MAX_ENEMY; nCntEnemy++)
	{
		m_aEnemies[nCntEnemy] = -1;
	}
	return S_OK;
}

void CWater::Uninit(void)
{
	m_pPlayerpos = NULL;
	CScene2D::Uninit();
}

void CWater::Update(void)
{
	if (CPauseUi::GetPause() == false)
	{
		if (m_bSize == true)
		{
			if (WATER_SIZE > m_fSize)
			{
				m_fSize += 20.0f;
				m_bDraw = true;
			}

		}
		else
		{
			if (m_fSize > 0.0f)
			{
				m_fSize -= 20.0f;
				if (m_fSize <= 20.0f)
				{
					m_bDraw = false;
				}
			}
		}
		if (m_bDraw == true)
		{
			D3DXVECTOR3 pos;
			pos = *m_pPlayerpos;
			if (m_fRotY > D3DX_PI)
			{
				m_fRotY -= D3DX_PI * 2;
			}
			else if (m_fRotY < -D3DX_PI)
			{
				m_fRotY += D3DX_PI * 2;
			}
			if (m_fRotY != m_fRotDeshY)
			{
				float fRotY;
				fRotY = m_fRotY;
				if (m_fRotDeshY - m_fRotY < -D3DX_PI)
				{
					m_fRotY += (m_fRotDeshY - m_fRotY + D3DX_PI * 2) * 0.2f;
				}
				else if (m_fRotDeshY - m_fRotY > D3DX_PI)
				{
					m_fRotY += (m_fRotDeshY - m_fRotY - D3DX_PI * 2) * 0.2f;
				}
				else
				{
					m_fRotY += (m_fRotDeshY - m_fRotY) * 0.2f;
				}
				CScene2D::SetRot(((m_fRotY - fRotY) / D3DX_PI));
			}
			pos.x += sinf(m_fRotY) * m_fSize * 0.92f;
			pos.y += cosf(m_fRotY) * m_fSize * 0.92f;
			Set(m_fSize, m_fSize, pos);

			if (m_nListCnt > 0)
			{
				m_nListCnt--;
				if (m_nListCnt <= 0)
				{
					int nCntEnemy;
					for (nCntEnemy = 0; nCntEnemy < WATER_MAX_ENEMY; nCntEnemy++)
					{
						m_aEnemies[nCntEnemy] = -1;
					}
					m_nListCnt = 10;
				}
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
						D3DXVECTOR3 Relpos;
						Relpos = posEnemy - pos;
						D3DXVECTOR3 TransPos;
						TransPos.x = cosf(D3DX_PI - m_fRotY) * Relpos.x + sinf(D3DX_PI - m_fRotY) * Relpos.y;
						TransPos.y = -sinf(D3DX_PI - m_fRotY) * Relpos.x + cosf(D3DX_PI - m_fRotY) * Relpos.y;
						if (m_fSize * sinf(-0.08f * D3DX_PI) <= TransPos.x + ENEMY_SIZE * sinf(0.25f * D3DX_PI) && TransPos.x + ENEMY_SIZE * sinf(-0.25f * D3DX_PI) <= m_fSize * sinf(0.08f * D3DX_PI) &&
							-m_fSize * 1.3f <= TransPos.y + ENEMY_SIZE * cosf(0.75f * D3DX_PI) && TransPos.y + ENEMY_SIZE * cosf(-0.75f * D3DX_PI) <= m_fSize)
						{
							if (pScene1->GetDamage() == true)
							{
								pScene1->Damage(4);
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
						if (element == ELEMENT_THUNDER)
						{
							D3DXVECTOR3 posEnemy;
							posEnemy = pScene2->GetPos();
							D3DXVECTOR3 Relpos;
							Relpos = posEnemy - pos;
							D3DXVECTOR3 TransPos;
							TransPos.x = cosf(D3DX_PI - m_fRotY) * Relpos.x + sinf(D3DX_PI - m_fRotY) * Relpos.y;
							TransPos.y = -sinf(D3DX_PI - m_fRotY) * Relpos.x + cosf(D3DX_PI - m_fRotY) * Relpos.y;
							if (m_fSize * sinf(-0.08f * D3DX_PI) <= TransPos.x + BULLET_SIZE * sinf(0.25f * D3DX_PI) && TransPos.x + BULLET_SIZE * sinf(-0.25f * D3DX_PI) <= m_fSize * sinf(0.08f * D3DX_PI) &&
								-m_fSize * 1.3f <= TransPos.y + BULLET_SIZE * cosf(0.75f * D3DX_PI) && TransPos.y + BULLET_SIZE * cosf(-0.75f * D3DX_PI) <= m_fSize)
							{
								CPrize::Create(pos, D3DXVECTOR3(20.0f, 20.0f, 0.0f), ELEMENT_THUNDER, 5);
								CUi::SetScore(80);
								pScene2->Uninit();
							}
						}
					}
					pScene2 = NULL;
				}
			}
		}
	}
}

void CWater::Draw(void)
{
	if (m_bDraw == true)
	{
		CScene2D::Draw();
	}
}

CWater *CWater::Create(D3DXVECTOR3 pos)
{
	CWater *pWater;
	pWater = new CWater(PRIORITY_BG);
	if (pWater != NULL)
	{
		pWater->Init(pos);
		pWater->BindTexture(m_pTexture);
	}
	return pWater;
}

HRESULT CWater::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/water.png",
		&m_pTexture);
	return S_OK;
}

void CWater::UnLoad(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

void CWater::Damage(int nDamage)
{

}

bool CWater::GetDamage(void)
{
	return false;
}

void CWater::SetRotDesh(float fRotDeshY)
{
	m_bSize = true;
	m_fRotDeshY = fRotDeshY;
	if (m_fSize <= 0.0f)
	{
		float fRotY;
		fRotY = m_fRotY;
		m_fRotY = fRotDeshY;
		CScene2D::SetRot(((m_fRotY - fRotY) / D3DX_PI));
	}
}

void CWater::SetUninit(void)
{
	m_bSize = false;
}

bool CWater::EnemyList(int nEnemy)
{
	int nCntEnemy;
	for (nCntEnemy = 0; nCntEnemy < WATER_MAX_ENEMY; nCntEnemy++)
	{
		if (m_aEnemies[nCntEnemy] == nEnemy)
		{
			return false;
		}
	}
	return true;
}

void CWater::ListPush(int nEnemy)
{
	m_aEnemies[m_nListCnt] = nEnemy;
	m_nListCnt++;
	if (m_nListCnt >= WATER_MAX_ENEMY)
	{
		m_nListCnt = 0;
	}
}