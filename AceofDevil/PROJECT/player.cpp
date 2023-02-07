//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "renderer.h"
#include "sound.h"
#include "fade.h"
#include "fire.h"
#include "ice.h"
#include "thunder.h"
#include "water.h"
#include "wind.h"
#include "ui.h"
#include "pauseui.h"
#include "shadow.h"
#include "explosion.h"
#include "tutorialui.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
int CPlayer::m_aResource[MAX_TYPE] = {};
bool CPlayer::m_bDeath = false;
bool CPlayer::m_bTuto = false;

CPlayer::CPlayer(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

CPlayer::~CPlayer(void)
{

}

//初期化
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	CScene2D::Init(POLYGON_WIDTH, POLYGON_HEIGHT, pos, 1.0f);
	CScene2D::SetObjType(CScene::OBJTYPE_PLAYER);
	m_Maxmove = D3DXVECTOR3(6.0f, 6.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nBulletTime = 6;
	m_nBullet = 0;
	m_pInputKeyboard = CManager::GetInputKeyboard();
	m_pInputGamePad = CManager::GetInputGamePad();
	m_pWater = CWater::Create(pos);
	m_pWind = CWind::Create(pos);
	m_pShadowScene = CShadow::Create(D3DXVECTOR3(pos.x + 60.0f, pos.y + 60.0f, 0.0f), POLYGON_WIDTH / 2, POLYGON_HEIGHT / 2, CShadow::SHADOW_F_15S_MTD);
	m_nType = 0;
	m_nHP = 5;
	m_nMagicTime = 3;
	m_nSlide = 0;
	m_bSlide = true;
	CUi::SetLife(m_nHP);
	m_bUse = false;
	m_fScale = 1.0f;
	m_bDeath = false;
	int nCntMagic;
	for (nCntMagic = 0; nCntMagic < MAX_TYPE; nCntMagic++)
	{
		m_aResource[nCntMagic] = 500;
	}
	SetGauge();
	return S_OK;
}

//終了
void CPlayer::Uninit(void)
{
	m_pInputGamePad = NULL;
	m_pInputKeyboard = NULL;
	if (m_pShadowScene != NULL)
	{
		m_pShadowScene = NULL;
	}
	m_pWater = NULL;
	m_pWind = NULL;
	CScene2D::Uninit();
}

//更新
void CPlayer::Update(void)
{
	//ポーズ中じゃない
	if (CPauseUi::GetPause() == false)
	{
		bool bUse = m_bUse;
		D3DXVECTOR3 pos = GetPos();

		//魔力回復のカウントを進める
		m_nMagicTime--;
		if (m_nMagicTime <= 0)
		{
			//魔力回復
			int nCntMagic;
			for (nCntMagic = 0; nCntMagic < MAX_TYPE; nCntMagic++)
			{
				if (m_aResource[nCntMagic] < 1000)
				{
					m_aResource[nCntMagic]++;
					if (m_aResource[nCntMagic] > 1000)
					{
						m_aResource[nCntMagic] = 1000;
					}
				}
			}
			m_nMagicTime = 3;
		}

		//弾発射のインターバルのカウントを進める
		if (m_nBullet > 0)
		{
			m_nBullet--;
		}

		//ダメージをくらった処理
		if (m_nDamageTime > 0)
		{
			m_nDamageTime--;
			if (m_nDamageTime <= 0)
			{
				ChangeColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}

		//ダメージをくらった処理
		if (m_nDTime > 0)
		{
			m_nDTime--;
			if (m_nDTime <= 0 && m_nHP > 0)
			{
				//半透明にする
				ChangeColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			}
		}

		//操作
		if (m_pInputKeyboard != NULL || m_pInputGamePad != NULL)
		{
			//ゲームパッド
			if (m_pInputGamePad != NULL)
			{
				//移動
				if (m_pInputGamePad->LeftStickX() != 0.0f || m_pInputGamePad->LeftStickY() != 0.0f)
				{
					Move(atan2f(m_pInputGamePad->LeftStickX(), m_pInputGamePad->LeftStickY()));
				}
				else
				{
					Stop();
				}

				//属性変更
				if (m_pInputGamePad->GetButton(CInputGamePad::DIP_R2) == true)
				{
					m_nType++;
					if (m_nType >= MAX_TYPE)
					{
						m_nType = 0;
					}
					m_nBulletTime = SetBulletTime(m_nType);
					m_bUse = false;
					m_nBullet = 0;
					CUi::ChangeMagic(false);
					CSound::Play(CSound::SOUND_LABEL_MAGICCHANGE);
				}
				else if (m_pInputGamePad->GetButton(CInputGamePad::DIP_L2) == true)
				{
					m_nType--;
					if (m_nType < 0)
					{
						m_nType = MAX_TYPE - 1;
					}
					m_nBulletTime = SetBulletTime(m_nType);
					m_bUse = true;
					m_nBullet = 0;
					CUi::ChangeMagic(true);
					CSound::Play(CSound::SOUND_LABEL_MAGICCHANGE);
				}

				//水を選んでる
				if (m_nType == 3)
				{
					//魔力がある
					if (m_aResource[3] >= 3)
					{
						//右スティックを傾けた方向に水流を噴射する
						if (m_pInputGamePad->RightStickX() != 0.0f || m_pInputGamePad->RightStickY() != 0.0f)
						{
							m_pWater->SetRotDesh(atan2f(m_pInputGamePad->RightStickX(), m_pInputGamePad->RightStickY()));
							m_bUse = true;
							m_aResource[3] -= 3;
						}
						else
						{
							//水を消す
							m_pWater->SetUninit();
							m_bUse = false;
							CSound::Stop(CSound::SOUND_LABEL_WATER);
						}
					}
					else
					{
						//水を消す
						m_pWater->SetUninit();
						m_bUse = false;
						CSound::Stop(CSound::SOUND_LABEL_WATER);
					}
				}
				else
				{
					//水を消す
					m_pWater->SetUninit();
					CSound::Stop(CSound::SOUND_LABEL_WATER);
				}

				//風以外の場合、風を消す
				if (m_nType != 4)
				{
					m_pWind->SetSize(false);
					CSound::Stop(CSound::SOUND_LABEL_WIND);
				}

				//撃つ
				if (m_pInputGamePad->GetPress(CInputGamePad::DIP_R1) == true)
				{
					if (m_nBullet <= 0)
					{
						Shot(pos);
					}
				}
				else if(m_nType == 4)
				{
					//風消す
					m_pWind->SetSize(false);
					m_bUse = false;
					CSound::Stop(CSound::SOUND_LABEL_WIND);
				}
			}

			//キーボード
			else if (m_pInputKeyboard != NULL)
			{
				//移動
				if (m_pInputKeyboard->GetPress(DIK_A) == true)
				{
					if (m_pInputKeyboard->GetPress(DIK_W) == true)
					{
						Move(-0.75f * D3DX_PI);
					}
					else if (m_pInputKeyboard->GetPress(DIK_S) == true)
					{
						Move(-0.25f * D3DX_PI);
					}
					else
					{
						Move(-0.5f * D3DX_PI);
					}
				}
				else if (m_pInputKeyboard->GetPress(DIK_D) == true)
				{
					if (m_pInputKeyboard->GetPress(DIK_W) == true)
					{
						Move(0.75f * D3DX_PI);
					}
					else if (m_pInputKeyboard->GetPress(DIK_S) == true)
					{
						Move(0.25f * D3DX_PI);
					}
					else
					{
						Move(0.5f * D3DX_PI);
					}
				}
				else if (m_pInputKeyboard->GetPress(DIK_W) == true)
				{
					Move(1.0f * D3DX_PI);
				}
				else if (m_pInputKeyboard->GetPress(DIK_S) == true)
				{
					Move(0.0f * D3DX_PI);
				}
				else
				{
					Stop();
				}

				//属性変更
				if (m_pInputKeyboard->GetKey(DIK_E) == true)
				{
					m_nType++;
					if (m_nType >= MAX_TYPE)
					{
						m_nType = 0;
					}
					CUi::ChangeMagic(false);
					m_nBulletTime = SetBulletTime(m_nType);
					m_nBullet = 0;
					m_bUse = false;
					CSound::Play(CSound::SOUND_LABEL_MAGICCHANGE);
				}
				else if (m_pInputKeyboard->GetKey(DIK_Q) == true)
				{
					m_nType--;
					if (m_nType < 0)
					{
						m_nType = MAX_TYPE - 1;
					}
					CUi::ChangeMagic(true);
					m_nBulletTime = SetBulletTime(m_nType);
					m_nBullet = 0;
					m_bUse = false;
					CSound::Play(CSound::SOUND_LABEL_MAGICCHANGE);
				}

				//水を選んでいたら
				if (m_nType == 3)
				{
					//魔力がある
					if (m_aResource[3] >= 3)
					{
						//矢印で操作、その方向に水流を噴射する
						if (m_pInputKeyboard->GetPress(DIK_LEFT) == true)
						{
							if (m_pInputKeyboard->GetPress(DIK_UP) == true)
							{
								m_pWater->SetRotDesh(-D3DX_PI * 0.75f);
							}
							else if (m_pInputKeyboard->GetPress(DIK_DOWN) == true)
							{
								m_pWater->SetRotDesh(-D3DX_PI * 0.25f);
							}
							else
							{
								m_pWater->SetRotDesh(-D3DX_PI * 0.5f);
							}
							m_aResource[3] -= 3;
							m_bUse = true;
						}
						else if (m_pInputKeyboard->GetPress(DIK_RIGHT) == true)
						{
							if (m_pInputKeyboard->GetPress(DIK_UP) == true)
							{
								m_pWater->SetRotDesh(D3DX_PI * 0.75f);
							}
							else if (m_pInputKeyboard->GetPress(DIK_DOWN) == true)
							{
								m_pWater->SetRotDesh(D3DX_PI * 0.25f);
							}
							else
							{
								m_pWater->SetRotDesh(D3DX_PI * 0.5f);
							}
							m_aResource[3] -= 3;
							m_bUse = true;
						}
						else if (m_pInputKeyboard->GetPress(DIK_UP) == true)
						{
							m_pWater->SetRotDesh(D3DX_PI * 1.0f);
							m_aResource[3] -= 3;
							m_bUse = true;
						}
						else if (m_pInputKeyboard->GetPress(DIK_DOWN) == true)
						{
							m_pWater->SetRotDesh(D3DX_PI * 0.0f);
							m_aResource[3] -= 3;
							m_bUse = true;
						}
						else
						{
							//水を消す
							m_pWater->SetUninit();
							m_bUse = false;
							CSound::Stop(CSound::SOUND_LABEL_WATER);
						}
					}
					else
					{
						//水を消す
						m_pWater->SetUninit();
						m_bUse = false;
						CSound::Stop(CSound::SOUND_LABEL_WATER);
					}
				}
				else
				{
					//水を消す
					m_pWater->SetUninit();
					CSound::Stop(CSound::SOUND_LABEL_WATER);
				}

				//風以外の場合、風が消える
				if (m_nType != 4)
				{
					m_pWind->SetSize(false);
					CSound::Stop(CSound::SOUND_LABEL_WIND);
				}

				//撃つ
				if (m_pInputKeyboard->GetPress(DIK_RSHIFT) == true)
				{
					if (m_nBullet <= 0)
					{
						Shot(pos);
					}
				}
				else if (m_nType == 4)
				{
					//風消す
					m_pWind->SetSize(false);
					m_bUse = false;
					CSound::Stop(CSound::SOUND_LABEL_WIND);
				}
			}
			if (bUse == false && m_bUse == true)
			{
				if (m_nType == 3)
				{
					//水の音流す
					CSound::Play(CSound::SOUND_LABEL_WATER);
				}
				else if (m_nType == 4)
				{
					//風の音流す
					CSound::Play(CSound::SOUND_LABEL_WIND);
				}
			}
		}

		//生きてる
		if (m_nHP > 0)
		{
			pos += m_move;
			if (pos.x < POLYGON_WIDTH * sinf(0.25f * D3DX_PI))
			{
				pos.x = POLYGON_WIDTH * sinf(0.25f * D3DX_PI);
			}
			else if (pos.x > SCREEN_WIDTH - POLYGON_WIDTH * sinf(0.25f * D3DX_PI))
			{
				pos.x = SCREEN_WIDTH - POLYGON_WIDTH * sinf(0.25f * D3DX_PI);
			}
			if (pos.y < POLYGON_HEIGHT * cosf(0.25f * D3DX_PI))
			{
				pos.y = POLYGON_HEIGHT * cosf(0.25f * D3DX_PI);
			}
			else if (pos.y > SCREEN_HEIGHT - POLYGON_HEIGHT * cosf(0.25f * D3DX_PI))
			{
				pos.y = SCREEN_HEIGHT - POLYGON_HEIGHT * cosf(0.25f * D3DX_PI);
			}
			//魔力ゲージをセットする
			SetGauge();

			//UIと自機が被ったらUIを薄くする
			if (m_bTuto == true)
			{
				CTutorialUi::TutoClear(pos, POLYGON_WIDTH * sinf(0.25f * D3DX_PI), POLYGON_HEIGHT * cosf(0.25f * D3DX_PI));
			}
			CUi::KillClear(pos, POLYGON_WIDTH * sinf(0.25f * D3DX_PI), POLYGON_HEIGHT * cosf(0.25f * D3DX_PI));
			CUi::ScoreClear(pos, POLYGON_WIDTH * sinf(0.25f * D3DX_PI), POLYGON_HEIGHT * cosf(0.25f * D3DX_PI));
			CUi::DMGClear(pos, POLYGON_WIDTH * sinf(0.25f * D3DX_PI), POLYGON_HEIGHT * cosf(0.25f * D3DX_PI));
			CUi::MagicClear(pos, POLYGON_WIDTH * sinf(0.25f * D3DX_PI), POLYGON_HEIGHT * cosf(0.25f * D3DX_PI));
		}
		else
		{
			//徐々に小さくする
			m_fScale -= 0.001f;

			//カウント増加
			m_nSlide++;

			//カウントが一定以上になったら
			if (m_nSlide >= 3)
			{
				//左右に振動させる
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

			//爆発エフェクトカウントが進んだら
			if (m_nDTime <= 0)
			{
				//爆発エフェクトをランダムな場所に作成
				float fWidth;
				float fHeight;
				fWidth = (rand() % (int)POLYGON_WIDTH * 2) - POLYGON_WIDTH;
				fHeight = (rand() % (int)POLYGON_HEIGHT * 2) - POLYGON_HEIGHT;
				D3DXVECTOR3 Expos;
				Expos.x = pos.x + fWidth;
				Expos.y = pos.y + fHeight;
				CExplosion::Create(Expos);
				m_nDTime = 10;
			}

			//カウント進んだら
			if (m_nDamageTime <= 0)
			{
				//下に行く
				pos.y += 2.0f;
				//消えたら、リザルトに移行
				if (pos.y > SCREEN_HEIGHT)
				{
					CFade::SetFade(CManager::MODE_RESULT);
				}
			}
		}
		//場所や大きさ、影などをセット
		Set(POLYGON_WIDTH * m_fScale, POLYGON_HEIGHT * m_fScale, pos);
		m_pShadowScene->Set(POLYGON_WIDTH / 2.0f * m_fScale, POLYGON_HEIGHT / 2.0f * m_fScale, D3DXVECTOR3(pos.x + 60.0f * m_fScale, pos.y + 60.0f * m_fScale, 0.0f));
	}
}

//描画
void CPlayer::Draw(void)
{
	CScene2D::Draw();
}

//作成
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer *pPlayer;
	pPlayer = new CPlayer(PRIORITY_CHARA);
	if (pPlayer != NULL)
	{
		pPlayer->Init(pos);
		pPlayer->BindTexture(m_pTexture);
	}
	return pPlayer;
}

//移動
void CPlayer::Move(float fRot)
{
	m_move.x += (m_Maxmove.x * sinf(fRot) - m_move.x) * 0.1f;
	m_move.y += (m_Maxmove.y * cosf(fRot) - m_move.y) * 0.1f;
}

//止まる
void CPlayer::Stop(void)
{
	m_move.x -= m_move.x * 0.08f;
	m_move.y -= m_move.y * 0.08f;
}

//テクスチャ読み込み
HRESULT CPlayer::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/F-15S_MTD.png",
		&m_pTexture);
	return S_OK;
}

//テクスチャ破棄
void CPlayer::UnLoad(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//ダメージ処理
void CPlayer::Damage(int)
{
	//チュートリアルでなければ、体力を減らす
	if (m_bTuto == false)
	{
		m_nHP--;
		CUi::SetLife(m_nHP);
		CUi::Damage();
	}
	CSound::Play(CSound::SOUND_LABEL_PLAYERDAMAGE);

	//まだ生きてる
	if (m_nHP >= 1)
	{
		//カウントの設定、赤くする、ピンチだったらピンチのSE流す
		m_nDamageTime = 300;
		m_nDTime = 5;
		ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		if (m_nHP == 1)
		{
			CSound::Play(CSound::SOUND_LABEL_PINCH);
		}
	}
	else
	{
		//操作不能にする
		m_pInputGamePad = NULL;
		m_pInputKeyboard = NULL;
		CSound::Play(CSound::SOUND_LABEL_SHATDOWNBGM);
		m_nDamageTime = 60;
		m_bDeath = true;
	}
}

//ダメージ受けるか
bool CPlayer::GetDamage(void)
{
	if (m_nDamageTime <= 0 && m_nHP > 0)
	{
		return true;
	}
	return false;
}

//種類によって次に弾を撃つまでのインターバルが変わる
int CPlayer::SetBulletTime(int nType)
{
	switch (nType)
	{
	case 0:
		return 6;
	case 1:
		return 20;
	case 2:
		return 50;
	case 4:
		return 0;
	default:
		break;
	}
	return 0;
}

//それぞれUIのゲージを変える
void CPlayer::SetGauge(void)
{
	CUi::SetFireGauge(m_aResource[0]);
	CUi::SetIceGauge(m_aResource[1]);
	CUi::SetThunderGauge(m_aResource[2]);
	CUi::SetWaterGauge(m_aResource[3]);
	CUi::SetWindGauge(m_aResource[4]);
}

//撃つ
void CPlayer::Shot(D3DXVECTOR3 pos)
{
	if (m_nBullet <= 0)
	{
		//魔力があれば、撃つ
		switch (m_nType)
		{
		case 0:
			//火
			if (m_aResource[0] >= 15)
			{
				CFire::Create(pos, D3DXVECTOR3(5.0f, 5.0f, 0.0f));
				CSound::Play(CSound::SOUND_LABEL_FIRE);
				m_aResource[0] -= 15;
			}
			break;
		case 1:
			//氷
			if (m_aResource[1] >= 30)
			{
				CIce::Create(pos, D3DXVECTOR3(-6.0f, -6.0f, 0.0f), -0.1f);
				CIce::Create(pos, D3DXVECTOR3(-6.0f, -6.0f, 0.0f), 0.0f);
				CIce::Create(pos, D3DXVECTOR3(-6.0f, -6.0f, 0.0f), 0.1f);
				CSound::Play(CSound::SOUND_LABEL_ICE);
				m_aResource[1] -= 30;
			}
			break;
		case 2:
			//雷
			if (m_aResource[2] >= 90)
			{
				CThunder::Create(pos, D3DXVECTOR3(0.0f, -8.0f, 0.0f));
				CSound::Play(CSound::SOUND_LABEL_THUNDER);
				m_aResource[2] -= 90;
			}
			break;
		case 4:
			//風
			if (m_aResource[4] >= 4)
			{
				m_pWind->SetSize(true);
				m_bUse = true;
				m_aResource[4] -= 4;
			}
			else
			{
				m_pWind->SetSize(false);
				m_bUse = false;
				CSound::Stop(CSound::SOUND_LABEL_WIND);
			}
			break;
		}
		m_nBullet = m_nBulletTime;
	}
}

//プライズを拾う、魔力が回復する
void CPlayer::GetPrize(int nType, int nHeal)
{
	m_aResource[nType] += nHeal;
	if (m_aResource[nType] > 1000)
	{
		m_aResource[nType] = 1000;
	}
}