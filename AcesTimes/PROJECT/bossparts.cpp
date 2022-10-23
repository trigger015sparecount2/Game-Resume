//=============================================================================
//
//ボスパーツ処理 [bossparts.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "bossparts.h"
#include "bullet.h"
#include "renderer.h"
#include "ui.h"
#include "pauseui.h"
#include "shadow.h"
#include "sound.h"
#include "explosion.h"
#include "fade.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

CBossParts::CBossParts(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

CBossParts::~CBossParts(void)
{

}

HRESULT CBossParts::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nFireTex, int nHP, int nMoney, int nScore)
{
	CScene2D::Init(pos);
	CScene2D::SetSize(size);
	CScene2D::Set();
	m_nHP = nHP;
	m_nMoney = nMoney;
	m_nScore = nScore;
	m_pFire = CShadow::Create(D3DXVECTOR3(50.0f, 80.0f, 0.0f), pos, nFireTex);
	m_pFire->ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pFire->Set();
	m_nCntFire = 10;
	m_nCntDeath = 0;
	m_bFire = false;
	return S_OK;
}

void CBossParts::Uninit(void)
{
	if (m_pFire != NULL)
	{
		m_pFire->SetDeath(true);
		m_pFire = NULL;
	}
	CScene2D::Uninit();
}

void CBossParts::Update(void)
{
	if (CManager::GetPause() == false)
	{
		D3DXVECTOR3 pos = GetPos();
		if (m_nDamageTime > 0)
		{
			m_nDamageTime--;
			if (m_nDamageTime <= 0)
			{
				ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
		if (m_nHP > 0)
		{
			CollisionBullet(pos);
			if (m_nCntFire > 0)
			{
				m_nCntFire--;
				if (m_nCntFire <= 0)
				{
					m_nCntFire = 10;
					if (m_bFire == true)
					{
						m_bFire = false;
						m_pFire->SetSize(D3DXVECTOR2(55.0f, 88.0f));
					}
					else
					{
						m_bFire = true;
						m_pFire->SetSize(D3DXVECTOR2(50.0f, 80.0f));
					}
				}
			}
			if (m_bFire == true)
			{
				m_pFire->SetPos(D3DXVECTOR3(pos.x, pos.y + 92.0f, 0.0f));
			}
			else
			{
				m_pFire->SetPos(D3DXVECTOR3(pos.x, pos.y + 98.0f, 0.0f));
			}
			m_pFire->Set();
		}
		else
		{
			m_nCntDeath++;
			D3DXVECTOR2 size = GetSize();
			if (m_nCntDeath % 8 == 0)
			{
				m_nCntDeath = 0;
				D3DXVECTOR3 ExPos = (D3DXVECTOR3(pos.x + size.x * (((float)(rand() % 100)) - 50.0f) * 0.016f, pos.y + size.y * (((float)(rand() % 100)) - 50.0f) * 0.016f, 0.0f));
				CExplosion::Create(ExPos, 50.0f, 4);
			}
		}
		Set();
	}
}

void CBossParts::Draw(void)
{
	CScene2D::Draw();
}

CBossParts *CBossParts::Create(D3DXVECTOR3 pos, int nTexType, int nFireTex, D3DXVECTOR2 size, int nHP, int nMoney, int nScore)
{
	CBossParts *pBossParts;
	pBossParts = new CBossParts(PRIORITY_CHARA);
	if (pBossParts != NULL)
	{
		pBossParts->Init(pos, size, nFireTex, nHP, nMoney, nScore);
		pBossParts->SetTexture(nTexType);
	}
	return pBossParts;
}

void CBossParts::CollisionBullet(D3DXVECTOR3 pos)
{
	CScene *pScene;
	CScene *pSceneNext;
	pScene = GetScene(PRIORITY_BULLET);
	while (pScene)
	{
		pSceneNext = pScene->GetNext();
		if (pScene->GetObjType() == OBJECTTYPE_BULLET)
		{
			if (pScene->GetEnemy() == false)
			{
				D3DXVECTOR3 BulletPos = pScene->GetPos();
				D3DXVECTOR3 BulletSize = pScene->GetSize();
				D3DXVECTOR3 EnemySize = GetSize() * 0.65f;
				D3DXVECTOR3 Relpos;
				Relpos = pos - BulletPos;
				float fRotY = pScene->GetRot().y;
				D3DXVECTOR3 TransPos;
				TransPos.x = cosf(fRotY * D3DX_PI) * Relpos.x + sinf(fRotY * D3DX_PI) * Relpos.y;
				TransPos.y = -sinf(fRotY * D3DX_PI) * Relpos.x + cosf(fRotY * D3DX_PI) * Relpos.y;
				if (BulletSize.x * sinf(-0.12f * D3DX_PI) <= TransPos.x + EnemySize.x * sinf(0.25f * D3DX_PI) && TransPos.x - EnemySize.x * sinf(0.25f * D3DX_PI) <= BulletSize.x * sinf(0.12f * D3DX_PI) &&
					BulletSize.y * cosf(0.88f * D3DX_PI) <= TransPos.y + EnemySize.y * cosf(0.25f * D3DX_PI) && TransPos.y - EnemySize.x * cosf(0.25f * D3DX_PI) <= BulletSize.y * cosf(0.12f * D3DX_PI))
				{
					Damage(1);
					pScene->SetDeath(true);
				}
			}
		}
		pScene = pSceneNext;
	}
}

void CBossParts::Damage(int nDamage)
{
	m_nHP -= nDamage;
	if (m_nHP <= 0)
	{
		CScene *pScene;
		CScene *pSceneNext;
		pScene = GetScene(PRIORITY_CHARA);
		while (pScene)
		{
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJECTTYPE_PLAYER)
			{
				pScene->AddScore(m_nScore);
				break;
			}
		}
		CExplosion::Create(GetPos(), 50.0f, 4);
		CManager::AddMoney(m_nMoney);
		m_bDeath = true;
		CSound::Play(CSound::SOUND_LABEL_ENEMYSHATDOWN);
		m_pFire->ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		//m_pFire->SetDeath(true);
		//m_pFire = NULL;
	}
	else
	{
		CScene *pScene;
		CScene *pSceneNext;
		pScene = GetScene(PRIORITY_CHARA);
		while (pScene)
		{
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJECTTYPE_PLAYER)
			{
				pScene->AddScore(m_nScore / 50);
				break;
			}
		}
		m_nDamageTime = 5;
		ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		CSound::Play(CSound::SOUND_LABEL_ENEMYDAMAGE);
	}
}