//=============================================================================
//
//ボス処理 [boss.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "boss.h"
#include "bullet.h"
#include "renderer.h"
#include "ui.h"
#include "pauseui.h"
#include "shadow.h"
#include "sound.h"
#include "explosion.h"
#include "fade.h"
#include "bossparts.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

CBoss::CBoss(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{
	
}

CBoss::~CBoss(void)
{
	
}

HRESULT CBoss::Init(D3DXVECTOR3 pos, float fInterval, D3DXVECTOR2 size, int nShadowTex, int nHP, float fBulletMax, int nMoney, int nScore, bool bParts, int nBGMType)
{
	CScene2D::Init(pos);
	CScene2D::SetSize(size);
	CScene2D::Set();
	m_nHP = nHP;
	m_fBulletMax = fBulletMax;
	if (bParts == false)
	{
		m_fBullet[0] = fInterval;
	}
	else
	{
		int nCnt;
		for (nCnt = 0; nCnt < 7; nCnt++)
		{
			m_fBullet[nCnt] = fInterval + (float)(rand() % 60);
		}
	}
	m_nMoney = nMoney;
	m_nScore = nScore;
	m_bSlow = false;
	if (nBGMType == 1)
	{
		m_pShadow = CShadow::Create(D3DXVECTOR3(size.x * 0.6f, size.y * 0.6f, 0.0f), D3DXVECTOR3(pos.x + 300.0f, pos.y + 300.0f, 0.0f), nShadowTex);
	}
	else
	{
		m_pShadow = CShadow::Create(D3DXVECTOR3(size.x * 0.6f, size.y * 0.6f, 0.0f), D3DXVECTOR3(pos.x + 80.0f, pos.y + 80.0f, 0.0f), nShadowTex);
	}
	m_pShadow->ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f));
	m_pShadow->Set();
	m_bParts = bParts;
	m_bMove[0] = false;
	m_bMove[1] = false;
	m_nType = nBGMType;
	m_nBullet = 2;
	m_bDeath = false;
	if (m_bParts == true)
	{
		int nCntParts;
		for (nCntParts = 0; nCntParts < 8; nCntParts++)
		{
			m_pBossParts[nCntParts] = CBossParts::Create(D3DXVECTOR3((pos.x - 350.0f) + nCntParts * 100.0f, pos.y + 100.0f, 0.0f), 53, 54, D3DXVECTOR2(50.0f, 50.0f), 40, 1000, 3000);
		}
	}
	m_nCntStart = 300;
	m_nCntDeath = 0;
	switch (nBGMType)
	{
	case 0:
		CSound::Stop(CSound::SOUND_LABEL_STAGE1BGM);
		CSound::Play(CSound::SOUND_LABEL_BOSS1BGM);
		break;
	case 1:
		CSound::Stop(CSound::SOUND_LABEL_STAGE2BGM);
		CSound::Play(CSound::SOUND_LABEL_BOSS2BGM);
		break;
	case 2:
		CSound::Stop(CSound::SOUND_LABEL_STAGE3BGM);
		CSound::Play(CSound::SOUND_LABEL_BOSS3BGM);
		break;
	default:
		break;
	}
	return S_OK;
}

void CBoss::Uninit(void)
{
	if (m_pShadow != NULL)
	{
		m_pShadow->SetDeath(true);
		m_pShadow = NULL;
	}
	CScene2D::Uninit();
}

void CBoss::Update(void)
{
	if (CManager::GetPause() == false)
	{
		if (m_bDeath == false)
		{
			D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 pos = GetPos();
			if (m_nDamageTime > 0)
			{
				m_nDamageTime--;
				if (m_nDamageTime <= 0)
				{
					ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
			if (m_bParts == true)
			{
				int nCnt;
				for (nCnt = 0; nCnt < 7; nCnt++)
				{
					if (m_fBullet[nCnt] > 0.0f)
					{
						if (m_bSlow == true)
						{
							m_fBullet[nCnt] -= 0.01f;
						}
						else
						{
							m_fBullet[nCnt] -= 1.0f;
						}
						if (m_fBullet[nCnt] <= 0.0f)
						{
							m_fBullet[nCnt] = m_fBulletMax + (float)(rand() % 60);
							PartsShot(pos, nCnt);
						}
					}
				}
			}
			else
			{
				if (m_fBullet[0] > 0.0f)
				{
					if (m_bSlow == true)
					{
						m_fBullet[0] -= 0.01f;
					}
					else
					{
						m_fBullet[0] -= 1.0f;
					}
					if (m_fBullet[0] <= 0.0f)
					{
						if (m_nBullet > 0)
						{
							m_nBullet--;
							m_fBullet[0] = 15.0f;
						}
						else
						{
							m_nBullet = 2;
							m_fBullet[0] = m_fBulletMax;
						}
						Shot(pos);
					}
				}
			}
			if (m_nCntStart > 0)
			{
				m_nCntStart--;
				if (pos.y < 200.0f)
				{
					move = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				}
			}
			else if (m_nType != 1)
			{
				if (m_bSlow == true)
				{
					move = SlowMove();
				}
				else
				{
					move = Move();
				}
			}
			else if(m_nType == 1 && pos.y < 100.0f)
			{
				move = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			}
			pos += move;
			if (pos.x < GetSize().x)
			{
				m_bMove[0] = true;
			}
			else if (SCREEN_WIDTH - GetSize().x < pos.x)
			{
				m_bMove[0] = false;
			}
			if (pos.y < GetSize().y)
			{
				m_bMove[1] = true;
			}
			else if (300.0f < pos.y)
			{
				m_bMove[1] = false;
			}
			if (m_bParts == false)
			{
				CollisionBullet(pos);
			}
			else
			{
				bool bDestroy = true;
				int nCnt;
				for (nCnt = 0; nCnt < 8; nCnt++)
				{
					if (m_pBossParts[nCnt]->GetDestroy() == false)
					{
						bDestroy = false;
						break;
					}
				}
				if (bDestroy == true)
				{
					m_bDeath = true;
				}
			}
			m_bSlow = CollisionBomb(pos);
			SetPos(pos);
			if (m_nType != 1)
			{
				m_pShadow->SetPos(D3DXVECTOR3(pos.x + 80.0f, pos.y + 80.0f, 0.0f));
			}
			else
			{
				m_pShadow->SetPos(D3DXVECTOR3(pos.x + 300.0f, pos.y + 300.0f, 0.0f));
			}
			m_pShadow->Set();
			if (m_bParts == true)
			{
				int nCntParts;
				for (nCntParts = 0; nCntParts < 8; nCntParts++)
				{
					m_pBossParts[nCntParts]->SetPos(D3DXVECTOR3((pos.x - 410.0f) + nCntParts * 120.0f, pos.y + 80.0f, 0.0f));
					m_pBossParts[nCntParts]->Set();
				}
			}
			Set();
		}
		else
		{
			m_nCntDeath++;
			D3DXVECTOR2 size = GetSize();
			D3DXVECTOR3 pos = GetPos();
			if (m_nCntDeath % 5 == 0)
			{
				D3DXVECTOR3 ExPos = (D3DXVECTOR3(pos.x + size.x * (((float)(rand() % 100)) - 50.0f) * 0.016f, pos.y + size.y * (((float)(rand() % 100)) - 50.0f) * 0.016f, 0.0f));
				CExplosion::Create(ExPos, 50.0f, 4);
			}
			if (m_nCntDeath % 2 == 0)
			{
				pos.x -= 3.0f;
			}
			else
			{
				pos.x += 3.0f;
			}
			if (m_nCntDeath % 30 == 0)
			{
				CSound::Play(CSound::SOUND_LABEL_ENEMYSHATDOWN);
			}
			size.x -= 0.04f;
			size.y -= 0.05f;
			if (m_nCntDeath >= 80)
			{
				pos.y += 2.0f;
				if (m_nCntDeath > 300)
				{
					CFade::SetFade(CManager::MODE_RESULT);
				}
			}
			if (m_bParts == true)
			{
				m_pShadow->SetPos(D3DXVECTOR3(pos.x + 300.0f + 300.0f * (float)m_nCntDeath * -0.002f, pos.y + 300.0f + 300.0f * (float)m_nCntDeath * -0.002f, 0.0f));
			}
			else
			{
				m_pShadow->SetPos(D3DXVECTOR3(pos.x + 80.0f + 80.0f * (float)m_nCntDeath * -0.002f, pos.y + 80.0f + 80.0f * (float)m_nCntDeath * -0.002f, 0.0f));
			}
			m_pShadow->Set();
			SetPos(pos);
			SetSize(size);
			if (m_bParts == true)
			{
				int nCntParts;
				for (nCntParts = 0; nCntParts < 8; nCntParts++)
				{
					m_pBossParts[nCntParts]->SetPos(D3DXVECTOR3((pos.x - 410.0f) + nCntParts * 120.0f, pos.y + 80.0f + 80.0f * (float)m_nCntDeath * -0.0003f, 0.0f));
					D3DXVECTOR2 PartsSize = m_pBossParts[nCntParts]->GetSize();
					PartsSize.x -= 0.03f;
					PartsSize.y -= 0.03f;
					m_pBossParts[nCntParts]->SetSize(PartsSize);
					m_pBossParts[nCntParts]->Set();
				}
			}
			Set();
		}
	}
}

void CBoss::Draw(void)
{
	CScene2D::Draw();
}

CBoss *CBoss::Create(D3DXVECTOR3 pos, int nTexType, int nShadowTex, float fInterval, D3DXVECTOR2 size, int nHP, float fBulletMax, int nMoney, int nScore, bool bParts, int nBGMType)
{
	CBoss *pEnemy;
	pEnemy = new CBoss(PRIORITY_CHARA);
	if (pEnemy != NULL)
	{
		pEnemy->Init(pos, fInterval, size, nShadowTex, nHP, fBulletMax, nMoney, nScore, bParts, nBGMType);
		pEnemy->SetTexture(nTexType);
	}
	return pEnemy;
}

D3DXVECTOR3 CBoss::Move()
{
	D3DXVECTOR3 move;
	if (m_bMove[0] == true)
	{
		move.x = 3.0f;
	}
	else
	{
		move.x = -3.0f;
	}
	if (m_bMove[1] == true)
	{
		move.y = 2.0f;
	}
	else
	{
		move.y = -2.0f;
	}
	return move;
}

D3DXVECTOR3 CBoss::SlowMove()
{
	D3DXVECTOR3 move;
	if (m_bMove[0] == true)
	{
		move.x = 0.03f;
	}
	else
	{
		move.x = -0.03f;
	}
	if (m_bMove[1] == true)
	{
		move.y = 0.02f;
	}
	else
	{
		move.y = -0.02f;
	}
	return move;
}

void CBoss::Shot(D3DXVECTOR3 pos)
{
	CScene *pScene;
	CScene *pSceneNext;
	pScene = GetScene(PRIORITY_CHARA);
	while (pScene)
	{
		pSceneNext = pScene->GetNext();
		if (pScene->GetObjType() == OBJECTTYPE_PLAYER)
		{
			D3DXVECTOR3 PlayerPos = pScene->GetPos();
			float fRotY = atan2f(PlayerPos.x - pos.x, pos.y - PlayerPos.y);
			fRotY /= D3DX_PI;
			CBullet::Create(pos, D3DXVECTOR3(0.0f, fRotY - 0.1f, 0.0f), 6.0f, true);
			CBullet::Create(pos, D3DXVECTOR3(0.0f, fRotY, 0.0f), 6.0f, true);
			CBullet::Create(pos, D3DXVECTOR3(0.0f, fRotY + 0.1f, 0.0f), 6.0f, true);
			if (m_nType == 2)
			{
				CBullet::Create(pos, D3DXVECTOR3(0.0f, fRotY - 0.2f, 0.0f), 6.0f, true);
				CBullet::Create(pos, D3DXVECTOR3(0.0f, fRotY + 0.2f, 0.0f), 6.0f, true);
			}
			CSound::Play(CSound::SOUND_LABEL_GUN);
			break;
		}
		pScene = pSceneNext;
	}
}

void CBoss::PartsShot(D3DXVECTOR3 pos, int nCntBullet)
{
	CScene *pScene;
	CScene *pSceneNext;
	pScene = GetScene(PRIORITY_CHARA);
	while (pScene)
	{
		pSceneNext = pScene->GetNext();
		if (pScene->GetObjType() == OBJECTTYPE_PLAYER)
		{
			D3DXVECTOR3 PlayerPos = pScene->GetPos();
			float fRotY = atan2f(PlayerPos.x - (pos.x - 350.0f + 120.0f * nCntBullet), (pos.y + 80.0f) - PlayerPos.y);
			fRotY /= D3DX_PI;
			if (fabsf(fRotY) > 0.92f || fabsf(fRotY) < 0.5f)
			{
				CBullet::Create(D3DXVECTOR3(pos.x - 350.0f + 120.0f * nCntBullet, pos.y + 80.0f, 0.0f), D3DXVECTOR3(0.0f, fRotY, 0.0f), 6.0f, true);
			}
			else
			{
				CBullet::Create(D3DXVECTOR3(pos.x - 350.0f + 120.0f * nCntBullet, pos.y + 80.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 6.0f, true);
			}
			CSound::Play(CSound::SOUND_LABEL_GUN);
			break;
		}
		pScene = pSceneNext;
	}
}

void CBoss::CollisionBullet(D3DXVECTOR3 pos)
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

bool CBoss::CollisionBomb(D3DXVECTOR3 pos)
{
	CScene *pScene;
	CScene *pSceneNext;
	pScene = GetScene(PRIORITY_EFFECT);
	while (pScene)
	{
		pSceneNext = pScene->GetNext();
		if (pScene->GetObjType() == OBJECTTYPE_BOMB)
		{
			D3DXVECTOR3 BombPos = pScene->GetPos();
			float fRadius = pScene->GetSize().x * sinf(0.25f * D3DX_PI);
			float fEnemyRadius = GetSize().x * sinf(0.25f * D3DX_PI);
			float fDistance = sqrtf(powf(BombPos.x - pos.x, 2.0f) + powf(BombPos.y - pos.y, 2.0f));
			if (fDistance <= fRadius + fEnemyRadius)
			{
				return true;
			}
		}
		pScene = pSceneNext;
	}
	return false;
}

void CBoss::Damage(int nDamage)
{
	m_nHP -= nDamage;
	if (m_nHP == 0)
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
		ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		CSound::Play(CSound::SOUND_LABEL_ENEMYSHATDOWN);
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