//=============================================================================
//
// 火炎弾処理 [fire.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "fire.h"
#include "renderer.h"
#include "enemy.h"
#include "effect.h"
#include "pauseui.h"
#include "prize.h"
#include "ui.h"

//マクロ定義
#define FIRE_SIZE (20.0f)

//静的メンバ変数
LPDIRECT3DTEXTURE9 CFire::m_pTexture = NULL;

CFire::CFire(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

CFire::~CFire(void)
{

}

//初期化
HRESULT CFire::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	CScene2D::Init(FIRE_SIZE, FIRE_SIZE, pos, 1.0f);
	CScene::SetObjType(CScene::OBJTYPE_BULLET);
	CScene::SetPos(pos);
	CScene2D::SetSize(0.16f);
	Set(FIRE_SIZE, FIRE_SIZE, pos);

	m_nEnemy = 0;
	m_pPos = RockEnemy();
	m_move = move;
	m_fRotY = 0.0f;
	m_nEffect = 0;
	m_nLife = 240;
	m_bUninit = false;
	return S_OK;
}

//終了
void CFire::Uninit(void)
{
	m_pPos = NULL;
	CScene2D::Uninit();
}

//更新
void CFire::Update(void)
{
	//ポーズ中じゃない
	if (CPauseUi::GetPause() == false)
	{
		//まだ削除しない
		if (m_bUninit == false)
		{
			D3DXVECTOR3 pos = GetPos();
			float fRotY;
			fRotY = m_fRotY;
			Homing(pos);
			m_nLife--;
			pos.x += sinf(m_fRotY) * m_move.x;
			pos.y -= cosf(m_fRotY) * m_move.y;
			Set(FIRE_SIZE, FIRE_SIZE, pos);
			SetRot((fRotY - m_fRotY) / D3DX_PI);

			//エフェクトカウント進める
			m_nEffect--;
			//エフェクトカウントが進んだら
			if (m_nEffect <= 0)
			{
				//エフェクト作成
				CEffect::Create(pos, CEffect::COLORTYPE_RED);
				//カウントリセット
				m_nEffect = 2;
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
					//敵だったら
					if (objType == OBJTYPE_ENEMY)
					{
						D3DXVECTOR3 posEnemy;
						posEnemy = pScene1->GetPos();
						//当たっていたら
						if (posEnemy.x - ENEMY_SIZE * sinf(0.25f * D3DX_PI) <= pos.x + FIRE_SIZE && pos.x - FIRE_SIZE <= posEnemy.x + ENEMY_SIZE * sinf(0.25f * D3DX_PI) &&
							posEnemy.y - ENEMY_SIZE * cosf(0.25f * D3DX_PI) <= pos.y + FIRE_SIZE && pos.y - FIRE_SIZE <= posEnemy.y + ENEMY_SIZE * cosf(0.25f * D3DX_PI))
						{
							//敵にダメージ
							if (pScene1->GetDamage() == true)
							{
								pScene1->Damage(3);
							}
							m_bUninit = true;
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
						//氷属性だったら
						if (element == ELEMENT_ICE)
						{
							D3DXVECTOR3 posBullet;
							posBullet = pScene2->GetPos();
							//当たっていたら
							if (posBullet.x - ENEMY_SIZE * sinf(0.25f * D3DX_PI) <= pos.x + FIRE_SIZE && pos.x - FIRE_SIZE <= posBullet.x + ENEMY_SIZE * sinf(0.25f * D3DX_PI) &&
								posBullet.y - ENEMY_SIZE * cosf(0.25f * D3DX_PI) <= pos.y + FIRE_SIZE && pos.y - FIRE_SIZE <= posBullet.y + ENEMY_SIZE * cosf(0.25f * D3DX_PI))
							{
								CPrize::Create(pos, D3DXVECTOR3(20.0f, 20.0f, 0.0f), ELEMENT_ICE, 5);
								CUi::SetScore(140);
								pScene2->Uninit();
							}
						}
					}
					pScene2 = NULL;
				}
			}
			//寿命切れ
			if (m_nLife <= 0)
			{
				m_bUninit = true;
			}
		}
		else
		{
			//削除
			Uninit();
		}
	}
}

//描画
void CFire::Draw(void)
{
	CScene2D::Draw();
}

//作成
CFire *CFire::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	CFire *pFire;
	pFire = new CFire(PRIORITY_BULLET);
	if (pFire != NULL)
	{
		pFire->Init(pos, move);
		pFire->BindTexture(m_pTexture);
	}
	return pFire;
}

//テクスチャ読み込み
HRESULT CFire::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/fire.png",
		&m_pTexture);
	return S_OK;
}

//テクスチャ破棄
void CFire::UnLoad(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//敵をロックオンする
D3DXVECTOR3 *CFire::RockEnemy(void)
{
	int nCntEnemy;
	int nEnemy = -1;
	D3DXVECTOR3 pos;
	float fRotMin = 10.0f;
	pos = GetPos();
	for (nCntEnemy = 0; nCntEnemy < MAX_POLYGON; nCntEnemy++)
	{
		CScene *pScene;
		pScene = GetScene(PRIORITY_CHARA, nCntEnemy);
		if (pScene != NULL)
		{
			if (pScene->GetDamage() == true)
			{
				OBJTYPE objType;
				objType = pScene->GetObjType();
				//敵だったら
				if (objType == OBJTYPE_ENEMY)
				{
					D3DXVECTOR3 posEnemy;
					posEnemy = pScene->GetPos();
					float fRotY;
					//敵との角度計算
					fRotY = atan2f(posEnemy.x - pos.x, pos.y - posEnemy.y);
					//正面方向に近かったら
					if (fabsf(fRotMin) > fabsf(fRotY))
					{
						fRotMin = fRotY;
						nEnemy = nCntEnemy;
					}
				}
			}
		}
	}

	//敵が見つからなかったら終了
	if(nEnemy == -1)
	{
		return NULL;
	}

	D3DXVECTOR3 *pPos;

	//敵のポインタを渡す
	pPos = GetPointerPos(nEnemy);

	//何番目の敵か記憶する
	m_nEnemy = nEnemy;
	return pPos;
}

//敵の方向を向く
void CFire::Homing(D3DXVECTOR3 pos)
{
	//敵のposのポインタがある
	if (m_pPos != NULL)
	{
		//敵が存在していなかったら終了
		if (CScene::GetbEnemy(m_nEnemy) == false)
		{
			m_pPos = NULL;
			return;
		}
		//敵との角度計算
		float fRotY;
		fRotY = atan2f(m_pPos->x - pos.x, pos.y - m_pPos->y);

		//角度が狂わないようにする
		if (m_fRotY >= D3DX_PI)
		{
			m_fRotY -= D3DX_PI * 2;
		}
		else if (m_fRotY <= -D3DX_PI)
		{
			m_fRotY += D3DX_PI * 2;
		}

		//敵との角度と今自分が向いてる角度が同じじゃなかったら
		if (m_fRotY != fRotY)
		{

			if (fRotY - m_fRotY < -D3DX_PI)
			{
				m_fRotY += 0.03f;
			}
			else if (fRotY - m_fRotY > D3DX_PI)
			{
				m_fRotY -= 0.03f;
			}
			else if(fRotY - m_fRotY < 0.0f)
			{
				if (fabsf(fRotY - m_fRotY) < 0.03f)
				{
					m_fRotY = fRotY;
				}
				else
				{
					m_fRotY -= 0.03f;
				}
			}
			else if (fRotY - m_fRotY > 0.0f)
			{
				if (fabsf(fRotY - m_fRotY) < 0.03f)
				{
					m_fRotY = fRotY;
				}
				else
				{
					m_fRotY += 0.03f;
				}
			}
		}
	}
}