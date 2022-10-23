//=============================================================================
//
// ’eŠÛˆ— [bullet.h]
// Author : ŽOãq¢
//
//=============================================================================
#include "manager.h"
#include "thunder.h"
#include "renderer.h"
#include "enemy.h"
#include "effect.h"
#include "thunderband.h"
#include "thundereffect.h"
#include "bullet.h"
#include "pauseui.h"
#include "sound.h"
#include "prize.h"
#include "ui.h"

//Ã“Iƒƒ“ƒo•Ï”
LPDIRECT3DTEXTURE9 CThunder::m_pTexture = NULL;

CThunder::CThunder(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

CThunder::~CThunder(void)
{

}

HRESULT CThunder::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	CScene2D::Init(THUNDER_SIZE, THUNDER_SIZE, pos, 1.0f);
	CScene::SetObjType(CScene::OBJTYPE_BULLET);
	m_pThunderEffect = CThundereffect::Create(pos);
	m_move = move;
	m_nEffectTime = 2;
	return S_OK;
}

void CThunder::Uninit(void)
{
	CScene2D::Uninit();
}

void CThunder::Update(void)
{
	if (CPauseUi::GetPause() == false)
	{
		if (m_bUninit == false)
		{
			D3DXVECTOR3 pos = GetPos();
			pos += m_move;
			SetRot(0.7f);
			Set(THUNDER_SIZE, THUNDER_SIZE, pos);
			m_pThunderEffect->Set(THUNDEREFFECT_SIZE, THUNDEREFFECT_SIZE, pos);
			m_nEffectTime--;
			if (m_nEffectTime <= 0)
			{
				CEffect::Create(pos, CEffect::COLORTYPE_YELLOW);
				m_nEffectTime = 2;
			}
			int nCntScene;
			for (nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
			{
				//“G‚Ö‚Ìƒ_ƒ[ƒW
				CScene *pScene1;
				pScene1 = GetScene(PRIORITY_CHARA, nCntScene);
				if (pScene1 != NULL)
				{
					OBJTYPE objType;
					objType = pScene1->GetObjType();
					if (objType == OBJTYPE_ENEMY)
					{
						D3DXVECTOR3 posEnemy;
						posEnemy = pScene1->GetPos();
						if (posEnemy.x - ENEMY_SIZE * sinf(0.25f * D3DX_PI) <= pos.x + THUNDER_SIZE * sinf(0.25f * D3DX_PI) && pos.x - THUNDER_SIZE * sinf(0.25f * D3DX_PI) <= posEnemy.x + ENEMY_SIZE * sinf(0.25f * D3DX_PI) &&
							posEnemy.y - ENEMY_SIZE * cosf(0.25f * D3DX_PI) <= pos.y + THUNDER_SIZE * cosf(0.25f * D3DX_PI) && pos.y - THUNDER_SIZE * cosf(0.25f * D3DX_PI) <= posEnemy.y + ENEMY_SIZE * cosf(0.25f * D3DX_PI))
						{
							if (pScene1->GetDamage() == true)
							{
								pScene1->Damage(7);
								BandCreate(nCntScene, posEnemy);
							}
							m_bUninit = true;
						}
					}
					pScene1 = NULL;
				}

				//’eŠÛÁ‚µ
				CScene *pScene2;
				pScene2 = GetScene(PRIORITY_BULLET, nCntScene);
				if (pScene2 != NULL)
				{
					if (pScene2->GetEnemy() == true)
					{
						ELEMENT element;
						element = pScene2->GetElement();
						if (element == ELEMENT_WATER)
						{
							D3DXVECTOR3 posBullet;
							posBullet = pScene2->GetPos();
							if (posBullet.x - BULLET_SIZE * sinf(0.25f * D3DX_PI) <= pos.x + THUNDER_SIZE * sinf(0.25f * D3DX_PI) && pos.x - THUNDER_SIZE * sinf(0.25f * D3DX_PI) <= posBullet.x + BULLET_SIZE * sinf(0.25f * D3DX_PI) &&
								posBullet.y - BULLET_SIZE * cosf(0.25f * D3DX_PI) <= pos.y + THUNDER_SIZE * cosf(0.25f * D3DX_PI) && pos.y - THUNDER_SIZE * cosf(0.25f * D3DX_PI) <= posBullet.y + BULLET_SIZE * cosf(0.25f * D3DX_PI))
							{
								CPrize::Create(pos, D3DXVECTOR3(20.0f, 20.0f, 0.0f), ELEMENT_WATER, 5);
								CUi::SetScore(200);
								pScene2->Uninit();
							}
						}
					}
					pScene2 = NULL;
				}
			}
			if (pos.x <= -THUNDER_SIZE || SCREEN_WIDTH + THUNDER_SIZE <= pos.x ||
				pos.y <= -THUNDER_SIZE || SCREEN_HEIGHT + THUNDER_SIZE <= pos.y)
			{
				m_bUninit = true;
			}
		}
		else
		{
			m_pThunderEffect->Uninit();
			m_pThunderEffect = NULL;
			Uninit();
		}
	}
}

void CThunder::Draw(void)
{
	CScene2D::Draw();
}

CThunder *CThunder::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	CThunder *pThunder;
	pThunder = new CThunder(PRIORITY_BULLET);
	if (pThunder != NULL)
	{
		pThunder->Init(pos, move);
		pThunder->BindTexture(m_pTexture);
		pThunder->ChangeColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}
	return pThunder;
}

HRESULT CThunder::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//ƒeƒNƒXƒ`ƒƒ‚Ì“Ç‚Ýž‚Ý
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/thunderbullet.png",
		&m_pTexture);
	return S_OK;
}

void CThunder::UnLoad(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

void CThunder::Damage(int nDamage)
{

}

bool CThunder::GetDamage(void)
{
	return false;
}

void CThunder::BandCreate(int nEnemy, D3DXVECTOR3 pos)
{
	bool bSound = false;
	int nCntScene;
	for (nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
	{
		//“G‚Ö‚Ì“dŒ‚
		CScene *pScene1;
		pScene1 = GetScene(PRIORITY_CHARA, nCntScene);
		if (pScene1 != NULL && nCntScene != nEnemy)
		{
			OBJTYPE objType;
			objType = pScene1->GetObjType();
			if (objType == OBJTYPE_ENEMY)
			{
				D3DXVECTOR3 posEnemy;
				posEnemy = pScene1->GetPos();
				float fDistance;
				float fLengthX;
				float fLengthY;
				fLengthX = pos.x - posEnemy.x;
				fLengthY = pos.y - posEnemy.y;
				fDistance = sqrtf(powf(fLengthX, 2.0f) + powf(fLengthY, 2.0f));
				if (fDistance <= BAND_LENGTH)
				{
					D3DXVECTOR3 posBand;
					posBand = (pos + posEnemy) / 2;
					float fRotY;
					fRotY = atan2f(fLengthX, fLengthY);
					fRotY = fRotY / D3DX_PI;
					CThunderBand::Create(posBand, fRotY, fDistance / 1.5f);
					pScene1->Damage(5);
					bSound = true;
				}
			}
			pScene1 = NULL;
		}

		//’eŠÛÁ‚µ
		CScene *pScene2;
		pScene2 = GetScene(PRIORITY_BULLET, nCntScene);
		if (pScene2 != NULL)
		{
			ELEMENT element;
			element = pScene2->GetElement();
			if (element == ELEMENT_WATER)
			{
				D3DXVECTOR3 posBullet;
				posBullet = pScene2->GetPos();
				float fDistance;
				float fLengthX;
				float fLengthY;
				fLengthX = pos.x - posBullet.x;
				fLengthY = pos.y - posBullet.y;
				fDistance = sqrtf(powf(fLengthX, 2.0f) + powf(fLengthY, 2.0f));
				if (fDistance <= BAND_LENGTH)
				{
					D3DXVECTOR3 posBand;
					posBand = (pos + posBullet) / 2;
					float fRotY;
					fRotY = atan2f(fLengthX, fLengthY);
					fRotY = fRotY / D3DX_PI;
					CThunderBand::Create(posBand, fRotY, fDistance / 1.5f);
					CPrize::Create(pos, D3DXVECTOR3(20.0f, 20.0f, 0.0f), ELEMENT_WATER, 5);
					CUi::SetScore(100);
					pScene2->Uninit();
					bSound = true;
				}
			}
			pScene2 = NULL;
		}
	}
	if (bSound == true)
	{
		CSound::Play(CSound::SOUND_LABEL_LIGHTNING);
	}
}