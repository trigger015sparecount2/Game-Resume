//=============================================================================
//
// ボス処理 [boss.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "boss.h"
#include "bullet.h"
#include "renderer.h"
#include "explosion.h"
#include "ui.h"
#include "pauseui.h"
#include "shadow.h"
#include "fade.h"
#include "sound.h"
#include "prize.h"

//マクロ定義
#define BOSS_SIZE (80.0f)

//静的メンバ変数
LPDIRECT3DTEXTURE9 CBoss::m_pTexture = NULL;

//コンストラクタ
CBoss::CBoss(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

//デストラクタ
CBoss::~CBoss(void)
{

}

//初期化処理
HRESULT CBoss::Init(void)
{
	CScene2D::Init(BOSS_SIZE, BOSS_SIZE, D3DXVECTOR3(SCREEN_WIDTH / 2, -BOSS_SIZE, 0.0f), 1.0f);
	CScene2D::SetObjType(CScene::OBJTYPE_ENEMY);
	m_pShadowScene = CShadow::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 60.0f, -BOSS_SIZE + 60.0f, 0.0f), BOSS_SIZE / 2, BOSS_SIZE / 2, CShadow::SHADOW_RAFALE);
	m_move = D3DXVECTOR3(2.5f, 1.8f, 0.0f);
	m_nBullet = 0;
	m_nBulletInter = 160;
	m_nHP = 500;
	m_nScore = 1000;
	m_fScale = 1.0f;
	m_nDTime = 0;
	m_nType = 3;
	m_nSlide = 0;
	m_nCycle = 0;
	m_nBTime = 0;
	m_nCntBullet = 0;
	m_bSlide = true;
	m_bDrop = true;
	m_bUninit = false;
	m_bLeft = true;
	return S_OK;
}

//終了処理
void CBoss::Uninit(void)
{
	CScene2D::Uninit();
}

//更新処理
void CBoss::Update(void)
{
	if (CPauseUi::GetPause() == false)
	{
		if (m_bUninit == false)
		{
			D3DXVECTOR3 pos;
			pos = GetPos();
			//最初の登場時、上から現れる
			if (m_bDrop == true)
			{
				pos.y += 2.0f;
				if (pos.y > 100.0f)
				{
					//ある程度下に来るとやめる
					m_bDrop = false;
				}
			}
			else
			{
				//体力1以上
				if (m_nHP > 0)
				{
					//弾発射カウントプラスしていく
					m_nBullet++;
					//弾発射するまでカウントしたら
					if (m_nBulletInter <= m_nBullet)
					{
						//弾発射
						Shot(pos);
					}

					//被弾時処理
					if (m_nDamageTime > 0)
					{
						m_nDamageTime--;
						if (m_nDamageTime <= 0)
						{
							ChangeColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						}
					}

					//移動処理
					Move(pos);
					pos += m_move;
				}
				else
				{
					//徐々に画像を小さく、左右に小刻みに動かす
					m_fScale -= 0.001f;
					m_nSlide++;
					if (m_nSlide <= 0)
					{
						if (m_bSlide == true)
						{
							pos.x += 3.0f;
							m_bSlide = false;
						}
						else
						{
							pos.x -= 3.0f;
							m_bSlide = true;
						}
						m_nSlide = 0;
					}

					//爆発エフェクトのカウント増加
					m_nDTime++;
					//カウントが一定以上
					if (m_nDTime >= 10)
					{
						//ランダムに範囲を設定
						float fWidth;
						float fHeight;
						fWidth = (rand() % (int)BOSS_SIZE * 2) - BOSS_SIZE;
						fHeight = (rand() % (int)BOSS_SIZE * 2) - BOSS_SIZE;

						//場所を設定
						D3DXVECTOR3 Expos;
						Expos.x = pos.x + fWidth;
						Expos.y = pos.y + fHeight;
						
						//爆発エフェクト作成
						CExplosion::Create(Expos);
						m_nDTime = 0;
					}

					if (m_nDamageTime <= 0)
					{
						//徐々に下に移動
						pos.y += 2.0f;
						//画面から消えたら
						if (pos.y > SCREEN_HEIGHT)
						{
							//フェードする
							CFade::SetFade(CManager::MODE_RESULT);
						}
					}
					else
					{
						m_nDamageTime--;
					}
				}
			}
			//ボスの場所、大きさ、影の設定
			Set(BOSS_SIZE * m_fScale, BOSS_SIZE * m_fScale, pos);
			m_pShadowScene->Set(BOSS_SIZE / 2.0f * m_fScale, BOSS_SIZE / 2.0f * m_fScale, D3DXVECTOR3(pos.x + 60.0f * m_fScale, pos.y + 60.0f * m_fScale, 0.0f));
		}
		else
		{
			m_pShadowScene->Uninit();
			m_pShadowScene = NULL;
			Uninit();
		}
	}
}

//描画処理
void CBoss::Draw(void)
{
	CScene2D::Draw();
}

//作成
CBoss *CBoss::Create(void)
{
	CBoss *pBoss;
	pBoss = new CBoss(PRIORITY_CHARA);
	if (pBoss != NULL)
	{
		pBoss->Init();
		pBoss->BindTexture(m_pTexture);
	}
	return pBoss;
}

//テクスチャ読み込み
HRESULT CBoss::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/RAFALE.png",
		&m_pTexture);
	return S_OK;
}

//テクスチャ破棄
void CBoss::UnLoad(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//ダメージ処理
void CBoss::Damage(int nDamage)
{
	m_nHP -= nDamage;
	//撃墜時処理
	if (m_nHP <= 0)
	{
		//UI関連
		CUi::SetKill();
		CUi::SetScore(m_nScore * 100);
		CSound::Play(CSound::SOUND_LABEL_SHATDOWNBGM);
		m_nDamageTime = 60;
		ChangeColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	//まだ生きてる
	else
	{
		m_nDamageTime = 5;
		CUi::SetScore(m_nScore * nDamage);
		//赤くする
		ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		CSound::Play(CSound::SOUND_LABEL_ENEMYDAMAGE);
	}
}

//ダメージを受けるか
bool CBoss::GetDamage(void)
{
	if (m_nHP <= 0)
	{
		return false;
	}
	return true;
}

//攻撃処理
void CBoss::Shot(D3DXVECTOR3 pos)
{
	//最初撃つ前
	if (m_nCycle <= 0)
	{
		//ランダムで属性を設定
		m_nType = rand() % 4;
		switch (m_nType)
		{
		case 0: //火
			m_nCycle = 2;
			m_nBTime = 3;
			break;
		case 1: //氷
			m_nCycle = 3;
			m_nBTime = 15;
			break;
		case 2: //雷
			m_nCycle = 1;
			m_nBTime = 0;
			break;
		case 3: //水
			m_nCycle = 3;
			m_nBTime = 50;
			break;
		default:
			break;
		}
	}

	if (m_nBTime <= 0)
	{
		switch (m_nType)
		{
		case 0: //火
			//左右に振らせる
			if (m_bLeft == true)
			{
				CBullet::Create(pos, D3DXVECTOR3(3.0f * sinf(2.0f + m_nCntBullet * (1.0f / 7.0f)), -3.0f * cosf(2.0f + m_nCntBullet * (1.0f / 7.0f)), 0.0f), true, CScene::ELEMENT_FIRE);
			}
			else
			{
				CBullet::Create(pos, D3DXVECTOR3(3.0f * sinf(-2.0f - m_nCntBullet * (1.0f / 7.0f)), -3.0f * cosf(-2.0f - m_nCntBullet * (1.0f / 7.0f)), 0.0f), true, CScene::ELEMENT_FIRE);
			}

			//撃った弾のカウント増加
			m_nCntBullet++;
			//次弾撃つまでのカウントリセット
			m_nBTime = 3;

			//十発撃つ度に
			if (m_nCntBullet > 10)
			{
				if (m_bLeft == true)
				{
					m_bLeft = false;
				}
				else
				{
					m_bLeft = true;
					//サイクル減少
					m_nCycle--;
				}
				m_nCntBullet = 0;
			}
			break;
		case 1: //氷
			//プレイヤーの角度計算
			m_fRotY = SearchPlayer(pos);
			//弾を散弾で打ち出す
			CBullet::Create(pos, D3DXVECTOR3(3.0f * sinf(m_fRotY + 0.2f * D3DX_PI), -3.0f * cosf(m_fRotY + 0.2f * D3DX_PI), 0.0f), true, CScene::ELEMENT_ICE);
			CBullet::Create(pos, D3DXVECTOR3(3.0f * sinf(m_fRotY + 0.1f * D3DX_PI), -3.0f * cosf(m_fRotY + 0.1f * D3DX_PI), 0.0f), true, CScene::ELEMENT_ICE);
			CBullet::Create(pos, D3DXVECTOR3(3.0f * sinf(m_fRotY)				  , -3.0f * cosf(m_fRotY), 0.0f)				 , true, CScene::ELEMENT_ICE);
			CBullet::Create(pos, D3DXVECTOR3(3.0f * sinf(m_fRotY - 0.1f * D3DX_PI), -3.0f * cosf(m_fRotY - 0.1f * D3DX_PI), 0.0f), true, CScene::ELEMENT_ICE);
			CBullet::Create(pos, D3DXVECTOR3(3.0f * sinf(m_fRotY - 0.2f * D3DX_PI), -3.0f * cosf(m_fRotY - 0.2f * D3DX_PI), 0.0f), true, CScene::ELEMENT_ICE);
			//サイクル減少
			m_nCycle--;
			m_nBTime = 15;
			break;
		case 2: //雷
			//自身を中心に円形に打ち出す
			int nCntBullet;
			for (nCntBullet = 0; nCntBullet < 40; nCntBullet++)
			{
				CBullet::Create(pos, D3DXVECTOR3(2.5f * sinf(m_fRotY + nCntBullet * (1.0f / 20.0f) * D3DX_PI), -2.5f * cosf(m_fRotY + nCntBullet * (1.0f / 20.0f) * D3DX_PI), 0.0f), true, CScene::ELEMENT_THUNDER);
			}
			m_nCycle--;
			break;
		case 3: //水
			//プレイヤーの角度計算
			m_fRotY = SearchPlayer(pos);
			if (m_nCycle == 1)
			{
				//速い
				CBullet::Create(pos, D3DXVECTOR3(10.0f * sinf(m_fRotY), -10.0f * cosf(m_fRotY), 0.0f), true, CScene::ELEMENT_WATER);
			}
			else
			{
				//通常
				CBullet::Create(pos, D3DXVECTOR3(6.0f * sinf(m_fRotY), -6.0f * cosf(m_fRotY), 0.0f), true, CScene::ELEMENT_WATER);
			}
			//サイクル減少
			m_nCycle--;
			if (m_nCycle == 1)
			{
				//少し溜める
				m_nBTime = 100;
			}
			else
			{
				//通常
				m_nBTime = 50;
			}
			break;
		default:
			break;
		}
		//サイクルが0以下になると
		if (m_nCycle <= 0)
		{
			//弾発射カウントを0にする
			m_nBullet = 0;
		}
		CSound::Play(CSound::SOUND_LABEL_GUN);
	}
	else
	{
		m_nBTime--;
	}
}

//移動処理
void CBoss::Move(D3DXVECTOR3 pos)
{
	//左側に接触
	if (pos.x < BOSS_SIZE * sinf(0.25f * D3DX_PI))
	{
		//右に移動
		m_move.x = 2.5f;
	}
	//右側に接触
	else if (SCREEN_WIDTH - BOSS_SIZE * sinf(0.25f * D3DX_PI) < pos.x)
	{
		//左に移動
		m_move.x = -2.5f;
	}
	//上に接触
	if (pos.y < BOSS_SIZE * cosf(0.25f * D3DX_PI))
	{
		//下に移動
		m_move.y = 1.8f;
	}
	//下側に来たら
	else if (200.0f < pos.y)
	{
		//上に移動
		m_move.y = -1.8f;
	}
}

//プレイヤーの角度
float CBoss::SearchPlayer(D3DXVECTOR3 pos)
{
	float fRotY;
	int nCntScene;
	for (nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
	{
		CScene *pScene;
		pScene = GetScene(PRIORITY_CHARA, nCntScene);
		if (pScene != NULL)
		{
			OBJTYPE objType;
			objType = pScene->GetObjType();
			//プレイヤーだったら
			if (objType == OBJTYPE_PLAYER)
			{
				D3DXVECTOR3 posPlayer;
				posPlayer = pScene->GetPos();
				fRotY = atan2f(posPlayer.x - pos.x, pos.y - posPlayer.y);
				return fRotY;
			}
		}
	}
	return 0.0f;
}