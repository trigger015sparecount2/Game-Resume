//=============================================================================
//
// 敵処理 [enemy.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "enemy.h"
#include "bullet.h"
#include "renderer.h"
#include "ui.h"
#include "pauseui.h"
#include "shadow.h"
#include "sound.h"
#include "explosion.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
//int CEnemy::m_nEnemy = 0;

CEnemy::CEnemy(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{
	//m_nEnemy++;
}

CEnemy::~CEnemy(void)
{
	//m_nEnemy--;
}

//初期化
HRESULT CEnemy::Init(D3DXVECTOR3 pos, float fRotY, float fInterval, float fSpeed, D3DXVECTOR2 size, int nShadowTex, int nHP, float fBulletMax, int nMoney, int nScore, float fBulletSpeed, int nBulletType)
{
	CScene2D::Init(pos);
	CScene2D::SetSize(size);
	CScene2D::AddRot(fRotY);
	m_fRot = fRotY;
	CScene2D::Set();
	m_fSpeed = fSpeed;
	m_nHP = nHP;
	m_fBullet = fInterval;
	m_fBulletMax = fBulletMax;
	m_nMoney = nMoney;
	m_nScore = nScore;
	m_bSlow = false;
	m_nBulletType = nBulletType;
	m_fBulletSpeed = fBulletSpeed;
	m_pShadow = CShadow::Create(D3DXVECTOR3(size.x * 0.6f, size.y * 0.6f, 0.0f), D3DXVECTOR3(pos.x + 80.0f, pos.y + 80.0f, 0.0f), nShadowTex);
	m_pShadow->ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
	m_pShadow->AddRot(fRotY);
	m_pShadow->Set();
	return S_OK;
}

//終了
void CEnemy::Uninit(void)
{
	if (m_pShadow != NULL)
	{
		m_pShadow->SetDeath(true);
		m_pShadow = NULL;
	}
	CScene2D::Uninit();
}

//更新
void CEnemy::Update(void)
{
	//ポーズ中じゃない
	if (CManager::GetPause() == false)
	{
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 pos = GetPos();

		//ダメージくらって少しだけ赤くなり、元の色に戻す
		if (m_nDamageTime > 0)
		{
			m_nDamageTime--;
			if (m_nDamageTime <= 0)
			{
				ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}

		//カウント進める
		if (m_fBullet > 0.0f)
		{
			//爆弾に当たっていたらカウントを進めるのを遅くする
			if (m_bSlow == true)
			{
				m_fBullet -= 0.01f;
			}
			else
			{
				m_fBullet -= 1.0f;
			}

			//カウントが進んだら弾を撃ってカウントリセットする
			if (m_fBullet <= 0.0f)
			{
				m_fBullet = m_fBulletMax;
				Shot(pos);
			}
		}

		//移動(爆弾に当たっていたら遅くする)
		if (m_bSlow == true)
		{
			move = SlowMove();
		}
		else
		{
			move = Move();
		}
		pos += move;

		//弾、爆弾の当たり判定
		CollisionBullet(pos);
		m_bSlow = CollisionBomb(pos);

		//影含めて場所の設定
		SetPos(pos);
		m_pShadow->SetPos(D3DXVECTOR3(pos.x + 80.0f, pos.y + 80.0f, 0.0f));
		m_pShadow->Set();
		Set();

		//画面外にいったら消す
		if (pos.x < -GetSize().x * 2.0f || SCREEN_WIDTH + GetSize().x * 2.0f < pos.x ||
			pos.y < -GetSize().y * 2.0f || SCREEN_HEIGHT + GetSize().y * 2.0f < pos.y)
		{
			SetDeath(true);
		}
	}
}

//描画
void CEnemy::Draw(void)
{
	CScene2D::Draw();
}

//作成
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, int nTexType, int nShadowTex, float fRotY, float fInterval, float fSpeed, D3DXVECTOR2 size, int nHP, float fBulletMax, int nMoney, int nScore, float fBulletSpeed, int nBulletType)
{
	CEnemy *pEnemy;
	pEnemy = new CEnemy(PRIORITY_CHARA);
	if (pEnemy != NULL)
	{
		pEnemy->Init(pos, fRotY, fInterval, fSpeed, size, nShadowTex, nHP, fBulletMax, nMoney, nScore, fBulletSpeed, nBulletType);
		pEnemy->SetTexture(nTexType);
	}
	return pEnemy;
}

//移動
D3DXVECTOR3 CEnemy::Move()
{
	D3DXVECTOR3 move;
	move.x = -m_fSpeed * sinf(m_fRot * D3DX_PI);
	move.y = -m_fSpeed * cosf(m_fRot * D3DX_PI);
	return move;
}

//遅い移動
D3DXVECTOR3 CEnemy::SlowMove()
{
	D3DXVECTOR3 move;
	move.x = -m_fSpeed * sinf(m_fRot * D3DX_PI) * 0.01f;
	move.y = -m_fSpeed * cosf(m_fRot * D3DX_PI) * 0.01f;
	return move;
}

//撃つ
void CEnemy::Shot(D3DXVECTOR3 pos)
{
	CScene *pScene;
	CScene *pSceneNext;
	pScene = GetScene(PRIORITY_CHARA);
	while (pScene)
	{
		pSceneNext = pScene->GetNext();
		//プレイヤーだったら
		if (pScene->GetObjType() == OBJECTTYPE_PLAYER)
		{
			D3DXVECTOR3 PlayerPos = pScene->GetPos();
			float fRotY = atan2f(PlayerPos.x - pos.x, pos.y - PlayerPos.y);

			//プレイヤーの方向に向けて撃つ
			fRotY /= D3DX_PI;
			switch (m_nBulletType)
			{
			case 0:
				CBullet::Create(pos, D3DXVECTOR3(0.0f, fRotY, 0.0f), m_fBulletSpeed, true);
				break;
			case 1:
				CBullet::Create(pos, D3DXVECTOR3(0.0f, fRotY + 0.15f, 0.0f), m_fBulletSpeed, true);
				CBullet::Create(pos, D3DXVECTOR3(0.0f, fRotY - 0.15f, 0.0f), m_fBulletSpeed, true);
				break;
			case 2:
				CBullet::Create(pos, D3DXVECTOR3(0.0f, fRotY + 0.2f, 0.0f), m_fBulletSpeed, true);
				CBullet::Create(pos, D3DXVECTOR3(0.0f, fRotY, 0.0f), m_fBulletSpeed, true);
				CBullet::Create(pos, D3DXVECTOR3(0.0f, fRotY - 0.2f, 0.0f), m_fBulletSpeed, true);
				break;
			default:
				break;
			}
			CSound::Play(CSound::SOUND_LABEL_GUN);
			break;
		}
		pScene = pSceneNext;
	}
}

//弾との当たり判定
void CEnemy::CollisionBullet(D3DXVECTOR3 pos)
{
	CScene *pScene;
	CScene *pSceneNext;
	pScene = GetScene(PRIORITY_BULLET);
	while (pScene)
	{
		pSceneNext = pScene->GetNext();
		//弾だったら
		if (pScene->GetObjType() == OBJECTTYPE_BULLET)
		{
			if (pScene->GetEnemy() == false)
			{
				//敵のじゃなかったら
				D3DXVECTOR3 BulletPos = pScene->GetPos();
				D3DXVECTOR3 BulletSize = pScene->GetSize();
				D3DXVECTOR3 EnemySize = GetSize() * 0.65f;
				D3DXVECTOR3 Relpos;
				Relpos = pos - BulletPos;
				float fRotY = pScene->GetRot().y;
				D3DXVECTOR3 TransPos;
				//回転座標
				TransPos.x = cosf(fRotY * D3DX_PI) * Relpos.x + sinf(fRotY * D3DX_PI) * Relpos.y;
				TransPos.y = -sinf(fRotY * D3DX_PI) * Relpos.x + cosf(fRotY * D3DX_PI) * Relpos.y;
				//当たっていたら
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

//爆弾との当たり判定
bool CEnemy::CollisionBomb(D3DXVECTOR3 pos)
{
	CScene *pScene;
	CScene *pSceneNext;
	pScene = GetScene(PRIORITY_EFFECT);
	while (pScene)
	{
		pSceneNext = pScene->GetNext();
		//爆弾だったら
		if (pScene->GetObjType() == OBJECTTYPE_BOMB)
		{
			D3DXVECTOR3 BombPos = pScene->GetPos();
			float fRadius = pScene->GetSize().x * sinf(0.25f * D3DX_PI) - 20.0f;
			float fEnemyRadius = GetSize().x * sinf(0.25f * D3DX_PI);
			float fDistance = sqrtf(powf(BombPos.x - pos.x, 2.0f) + powf(BombPos.y - pos.y, 2.0f));
			//当たっていたら
			if (fDistance <= fRadius + fEnemyRadius)
			{
				return true;
			}
		}
		pScene = pSceneNext;
	}
	return false;
}

//ダメージ処理
void CEnemy::Damage(int nDamage)
{
	//体力減らす
	m_nHP -= nDamage;

	//やられた
	if (m_nHP <= 0)
	{
		CScene *pScene;
		CScene *pSceneNext;
		pScene = GetScene(PRIORITY_CHARA);
		while (pScene)
		{
			pSceneNext = pScene->GetNext();
			//プレイヤーだったら
			if (pScene->GetObjType() == OBJECTTYPE_PLAYER)
			{
				//スコア加算
				pScene->AddScore(m_nScore);
				break;
			}
		}
		//爆発エフェクト作成、金追加、撃墜音流す
		CExplosion::Create(GetPos(), 50.0f, 4);
		CManager::AddMoney(m_nMoney);
		SetDeath(true);
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
			//プレイヤーだったら
			if (pScene->GetObjType() == OBJECTTYPE_PLAYER)
			{
				//スコア加算(少しだけ)
				pScene->AddScore(m_nScore / 50);
				break;
			}
		}
		//少しだけ赤くする、音流す
		m_nDamageTime = 5;
		ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		CSound::Play(CSound::SOUND_LABEL_ENEMYDAMAGE);
	}
}