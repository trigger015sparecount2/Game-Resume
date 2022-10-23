//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "player.h"
#include "input.h"
#include "keyboard.h"
#include "gamepad.h"
#include "bullet.h"
#include "renderer.h"
#include "sound.h"
#include "fade.h"
#include "ui.h"
#include "pauseui.h"
#include "shadow.h"
#include "bomb.h"
#include "warp.h"
#include "select.h"
#include "textui.h"
#include "afterimage.h"
#include "explosion.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

CPlayer::CPlayer(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nHP, float fSpeed, int nMaxSlow, int nMaxReturn, SHOTTYPE ShotType, int nShadowTex, int nHudTex)
{
	CScene2D::Init(pos);
	CScene2D::SetSize(size);
	CScene2D::Set();
	m_pGauge[0] = CUI::Create(D3DXVECTOR3(40.0f, 650.0f, 0.0f), D3DXVECTOR2(20.0f, 70.0f), 25, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pGauge[1] = CUI::Create(D3DXVECTOR3(224.0f, 650.0f, 0.0f), D3DXVECTOR2(240.0f, 70.0f), 26, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pGauge[2] = CUI::Create(D3DXVECTOR3(408.0f, 650.0f, 0.0f), D3DXVECTOR2(20.0f, 70.0f), 25, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pGauge[3] = CUI::Create(D3DXVECTOR3(280.0f, 630.0f, 0.0f), D3DXVECTOR2(140.0f, 22.0f), -1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pGauge[4] = CUI::Create(D3DXVECTOR3(183.0f, 630.0f, 0.0f), D3DXVECTOR2(138.0f, 28.0f), 32, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pGauge[4]->SetHGauge(D3DXVECTOR2(193.0f, 28.0f));
	m_pGauge[5] = CUI::Create(D3DXVECTOR3(280.0f, 670.0f, 0.0f), D3DXVECTOR2(140.0f, 22.0f), -1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pGauge[6] = CUI::Create(D3DXVECTOR3(183.0f, 670.0f, 0.0f), D3DXVECTOR2(138.0f, 28.0f), 32, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pGauge[6]->SetHGauge(D3DXVECTOR2(193.0f, 28.0f));

	m_pScore[0] = CUI::Create(D3DXVECTOR3(29.0f, 40.0f, 0.0f), D3DXVECTOR2(10.0f, 40.0f), 25, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pScore[1] = CUI::Create(D3DXVECTOR3(195.0f, 40.0f, 0.0f), D3DXVECTOR2(224.0f, 40.0f), 26, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pScore[2] = CUI::Create(D3DXVECTOR3(361.0f, 40.0f, 0.0f), D3DXVECTOR2(10.0f, 40.0f), 25, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	
	m_pHUD[0] = CUI::Create(D3DXVECTOR3(1180.0f, 524.0f, 0.0f), D3DXVECTOR2(110.0f, 35.0f), 9, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pHUD[1] = CUI::Create(D3DXVECTOR3(1180.0f, 605.0f, 0.0f), D3DXVECTOR2(110.0f, 80.0f), 10, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pHUD[2] = CUI::Create(D3DXVECTOR3(1180.0f, 686.0f, 0.0f), D3DXVECTOR2(110.0f, 35.0f), 9, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pHUD[3] = CUI::Create(D3DXVECTOR3(1180.0f, 585.0f, 0.0f), D3DXVECTOR2(80.0f, 80.0f), nHudTex, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	
	
	m_fClear[0] = 1.0f;
	m_fClear[1] = 1.0f;
	m_fClear[2] = 1.0f;

	m_nHudTex = nHudTex;
	m_Status.pos = pos;
	m_Status.nHP = nHP;
	m_nMaxHP = nHP;
	m_ShotType = ShotType;
	m_pShadow = CShadow::Create(D3DXVECTOR3(size.x * 0.6f, size.y * 0.6f, 0.0f), D3DXVECTOR3(pos.x + 80.0f, pos.y + 80.0f, 0.0f), nShadowTex);
	m_pShadow->ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
	m_pKeyboard = CManager::GetKeyboard();
	m_pGamePad = CManager::GetGamepad();
	m_fMove = fSpeed;
	m_nCntShot = 0;
	m_nMaxReturn = nMaxReturn;
	m_nReturn = nMaxReturn;
	m_nMaxSlow = nMaxSlow;
	m_nSlow = nMaxSlow;
	m_bWarp = false;
	m_nPauseSelect = 0;
	m_nScore = 0;
	m_nCntDeath = 0;
	m_bDeath = false;
	m_bWarpNow = false;
	m_bPinch = false;
	m_bUIClear = true;
	m_nCntPinch = 0;
	m_nCntWarp = 0;
	m_fHUDColor = 0.0f;
	m_nCntWarpPoint = 0;
	m_pWarp = CWarp::Create(D3DXVECTOR2(30.0f, 30.0f));
	int nColor[4] = { 255,0,255,0 };
	char aText[128];
	wsprintf(&aText[0], "SCORE:%08d", 0);
	RECT rect = { 37, 14, 900, 950 };
	m_pTextUI[0] = CTextUI::Create(50, nColor, CTextUI::TEXT_WORDBREAK, &aText[0], rect);
	wsprintf(&aText[0], "SLOW:\nRETURN:");
	rect = { 0, 611, 180, 700 };
	m_pTextUI[1] = CTextUI::Create(40, nColor, CTextUI::TEXT_RIGHT, &aText[0], rect);
	wsprintf(&aText[0], "DMG:%02d%%", 0);
	rect = { 1123, 644, 1280, 700 };
	m_pTextUI[2] = CTextUI::Create(37, nColor, CTextUI::TEXT_WORDBREAK, &aText[0], rect);
	if (m_pGamePad->GetGamePad() == true)
	{
		wsprintf(&aText[0], "移動:左スティック\nショット:RB\nスロウボム:X\nリターンポイント設置:RT\nリターン:LT\nポーズ:START\n");
	}
	else
	{
		wsprintf(&aText[0], "移動:WASD\nショット:J\nスロウボム:SHIFT\nリターンポイント設置:E\nリターン:Q\nポーズ:ESCキー\n");
	}
	rect = { 0, 10, 1270, 700 };
	CTextUI::Create(30, nColor, CTextUI::TEXT_RIGHT, &aText[0], rect);
	CPauseUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), -1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.4f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CPauseUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f - 170.0f, 0.0f), D3DXVECTOR2(200.0f, 50.0f), 9, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CPauseUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f + 170.0f, 0.0f), D3DXVECTOR2(200.0f, 50.0f), 9, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	CPauseUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR2(200.0f, 195.0f), 10, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pPauseUI[0] = CPauseUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f - 110.0f, 0.0f), D3DXVECTOR2(120.0f, 50.0f), 11, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pPauseUI[1] = CPauseUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR2(150.0f, 50.0f), 12, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pPauseUI[2] = CPauseUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f + 110.0f, 0.0f), D3DXVECTOR2(120.0f, 50.0f), 13, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	return S_OK;
}

void CPlayer::Uninit(void)
{
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard = NULL;
	}
	if (m_pGamePad != NULL)
	{
		m_pGamePad = NULL;
	}
	if (m_pWarp != NULL)
	{
		m_pWarp = NULL;
	}
	if (m_pShadow != NULL)
	{
		m_pShadow->SetDeath(true);
		m_pShadow = NULL;
	}
	int nCnt;
	for (nCnt = 0; nCnt < 7; nCnt++)
	{
		if (m_pGauge[nCnt] != NULL)
		{
			m_pGauge[nCnt] = NULL;
		}
	}
	for (nCnt = 0; nCnt < 4; nCnt++)
	{
		if (m_pHUD[nCnt] != NULL)
		{
			m_pHUD[nCnt] = NULL;
		}
	}
	for (nCnt = 0; nCnt < 3; nCnt++)
	{
		if (m_pScore[nCnt] != NULL)
		{
			m_pScore[nCnt] = NULL;
		}
	}
	for (nCnt = 0; nCnt < 3; nCnt++)
	{
		if (m_pPauseUI[nCnt] != NULL)
		{
			m_pPauseUI[nCnt] = NULL;
		}
	}
	for (nCnt = 0; nCnt < 3; nCnt++)
	{
		if (m_pTextUI[nCnt] != NULL)
		{
			m_pTextUI[nCnt] = NULL;
		}
	}
	CManager::SetScore(m_nScore);
	CScene2D::Uninit();
}

void CPlayer::Update(void)
{
	if (m_bDeath == false) //生存時
	{
		if (CManager::GetPause() == false)
		{
			if (m_nCntDamage > 0)
			{
				m_nCntDamage--;
				if (m_nCntDamage == 290) //ダメージくらってから10フレーム後
				{
					int nCnt;
					int nColor[4] = { 255,0,255,0 };
					for (nCnt = 0; nCnt < 3; nCnt++)//テキスト
					{
						m_pTextUI[nCnt]->SetColor(nColor);
					}
					for (nCnt = 0; nCnt < 3; nCnt++)//左上スコア
					{
						m_pScore[nCnt]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[1]));
					}
					for (nCnt = 0; nCnt < 7; nCnt++)//左下ゲージ
					{
						if (nCnt != 3 && nCnt != 5)
						{
							if (nCnt == 4 || nCnt == 6)
							{
								m_pGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 1.0f, m_fClear[0]));
							}
							else
							{
								m_pGauge[nCnt]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[0]));
							}
						}
					}
					for (nCnt = 0; nCnt < 4; nCnt++)//右下体力
					{
						if (nCnt == 3)
						{
							if (ceilf(m_nMaxHP / 2) <= m_Status.nHP)
							{
								//HP半分より上
								m_fHUDColor += 1.0f / (m_nMaxHP / 2.0f);
								m_pHUD[nCnt]->ColorChange(D3DXCOLOR(m_fHUDColor, 1.0f, 0.0f, m_fClear[2]));
							}
							else
							{
								//HP半分より下
								m_fHUDColor -= 1.0f / (m_nMaxHP / 2.0f);
								m_pHUD[nCnt]->ColorChange(D3DXCOLOR(1.0f, m_fHUDColor, 0.0f, m_fClear[2]));
							}
						}
						else
						{
							m_pHUD[nCnt]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[2]));
						}
					}
					ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f)); //自機を半透明にする
				}
				else if (m_nCntDamage == 60) //自機の色を戻す
				{
					ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
			if (m_nCntShot > 0)
			{
				m_nCntShot--;
			}
			if (m_bPinch == true)
			{
				m_nCntPinch++;
				if (m_nCntPinch == 5)
				{
					m_nCntPinch = 0;
					if (m_bUIClear == true)
					{
						m_bUIClear = false;
						m_pHUD[3]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f)); //右下の自機のシルエットを透明にする
					}
					else
					{
						m_bUIClear = true;
						m_pHUD[3]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2])); //右下の自機のシルエットを実体化させる
					}
				}
			}
			if (m_nCntWarpPoint > 0)
			{
				m_nCntWarpPoint--;
				if (m_nCntWarpPoint <= 0)
				{
					m_pWarp->SetDraw(false);
					m_bWarp = false;
				}
			}
			if (m_nSlow < m_nMaxSlow)
			{
				m_nSlow++;
				D3DXVECTOR2 size = m_pGauge[4]->GetSize();
				size.x += 193.0f / ((float)m_nMaxSlow);
				m_pGauge[4]->SetHGauge(size); //スロウゲージをセットする
				//m_pGauge[4]->Set();
			}
			if (m_nReturn < m_nMaxReturn)
			{
				m_nReturn++;
				D3DXVECTOR2 size = m_pGauge[6]->GetSize();
				size.x += 193.0f / ((float)m_nMaxReturn);
				m_pGauge[6]->SetHGauge(size); //リターンゲージをセットする
				//m_pGauge[6]->Set();
			}
			D3DXVECTOR3 pos = GetPos();
			if (m_bWarpNow == false)
			{
				if (m_pGamePad != NULL)
				{
					if (fabsf(m_pGamePad->LeftStickX()) > 0.3f || fabsf(m_pGamePad->LeftStickY()) > 0.3f)
					{
						Move(atan2f(m_pGamePad->LeftStickX(), m_pGamePad->LeftStickY()));
					}
					else
					{
						Stop();
					}
					if (m_pGamePad->GetPress(CGamePad::DIP_R1) == true)
					{
						if (m_nCntShot <= 0)
						{
							Shot(pos);
							m_nCntShot = 8;
						}
					}
					if (m_pGamePad->GetButton(CGamePad::DIP_X) == true)
					{
						if (m_nSlow >= m_nMaxSlow)
						{
							CBomb::Create(pos, 360.0f, 480);
							m_nSlow = 0;
							CSound::Play(CSound::SOUND_LABEL_BOMB);
							m_pGauge[4]->SetSize(D3DXVECTOR2(0.0f, 28.0f));
						}
					}
					if (m_pGamePad->GetButton(CGamePad::DIP_R2) == true)
					{
						TimeKeep(pos);
					}
					if (m_pGamePad->GetButton(CGamePad::DIP_L2) == true)
					{
						if (m_bWarp == true && m_nReturn >= m_nMaxReturn)
						{
							TimeReturn(pos);
						}
					}
					if (m_pGamePad->GetButton(CGamePad::DIP_START) == true)
					{
						CManager::SetPause(true);
						CSound::Play(CSound::SOUND_LABEL_PAUSE);
					}
				}
				else if (m_pKeyboard != NULL)
				{
					if (m_pKeyboard->GetPress(DIK_W) == true)
					{
						if (m_pKeyboard->GetPress(DIK_A) == true)
						{
							Move(D3DX_PI * -0.75f);
						}
						else if (m_pKeyboard->GetPress(DIK_D) == true)
						{
							Move(D3DX_PI * 0.75f);
						}
						else
						{
							Move(D3DX_PI);
						}
					}
					else if (m_pKeyboard->GetPress(DIK_S) == true)
					{
						if (m_pKeyboard->GetPress(DIK_A) == true)
						{
							Move(D3DX_PI * -0.25f);
						}
						else if (m_pKeyboard->GetPress(DIK_D) == true)
						{
							Move(D3DX_PI * 0.25f);
						}
						else
						{
							Move(0.0f);
						}
					}
					else if (m_pKeyboard->GetPress(DIK_A) == true)
					{
						Move(D3DX_PI * -0.5f);
					}
					else if (m_pKeyboard->GetPress(DIK_D) == true)
					{
						Move(D3DX_PI * 0.5f);
					}
					else
					{
						Stop();
					}
					if (m_pKeyboard->GetPress(DIK_Q) == true)
					{
						if (m_bWarp == true && m_nReturn >= m_nMaxReturn)
						{
							TimeReturn(pos);
						}
					}
					else if (m_pKeyboard->GetPress(DIK_E) == true)
					{
						TimeKeep(pos);
					}
					if (m_pKeyboard->GetPress(DIK_J) == true)
					{
						if (m_nCntShot <= 0)
						{
							Shot(pos);
							m_nCntShot = 8;
						}
					}
					if (m_pKeyboard->GetKey(DIK_LSHIFT) == true || m_pKeyboard->GetKey(DIK_RSHIFT) == true)
					{
						if (m_nSlow >= m_nMaxSlow)
						{
							CBomb::Create(pos, 360.0f, 480);
							m_nSlow = 0;
							CSound::Play(CSound::SOUND_LABEL_BOMB);
							m_pGauge[4]->SetSize(D3DXVECTOR2(0.0f, 28.0f));
						}
					}
					if (m_pKeyboard->GetKey(DIK_ESCAPE) == true)
					{
						CManager::SetPause(true);
						CSound::Play(CSound::SOUND_LABEL_PAUSE);
					}
				}
			}
			else
			{
				m_nCntWarp--;
				pos = Return(pos);
				if (m_nCntWarp <= 0)
				{
					m_bWarpNow = false;
				}
			}
			pos += m_move;
			CollisionUI(pos);
			CollisionBullet(pos);
			if (pos.x < GetSize().x * sinf(-GetSizeRot() * D3DX_PI))
			{
				pos.x = GetSize().x * sinf(-GetSizeRot() * D3DX_PI);
			}
			else if (SCREEN_WIDTH + GetSize().x * sinf(GetSizeRot() * D3DX_PI) < pos.x)
			{
				pos.x = SCREEN_WIDTH + GetSize().x * sinf(GetSizeRot() * D3DX_PI);
			}
			if (pos.y < GetSize().y * cosf((1.0f - GetSizeRot()) * D3DX_PI))
			{
				pos.y = GetSize().y * cosf((1.0f - GetSizeRot()) * D3DX_PI);
			}
			else if (SCREEN_HEIGHT + GetSize().y * cosf(GetSizeRot() * D3DX_PI) < pos.y)
			{
				pos.y = SCREEN_HEIGHT + GetSize().y * cosf(GetSizeRot() * D3DX_PI);
			}
			m_pShadow->SetPos(D3DXVECTOR3(pos.x + 80.0f, pos.y + 80.0f, 0.0f));
			m_pShadow->Set();
			SetPos(pos);
			Set();
		}
		else
		{
			if (m_pGamePad != NULL)
			{
				if (m_pGamePad->GetButton(CGamePad::DIP_START) == true)
				{
					CManager::SetPause(false);
				}
				if (m_pGamePad->TriggerCrossKey() == 0.0f)
				{
					PauseChange(-1);
				}
				if (m_pGamePad->TriggerCrossKey() == 18000.0f)
				{
					PauseChange(1);
				}
				if (m_pGamePad->GetButton(CGamePad::DIP_B) == true)
				{
					if (CFade::GetFade() == CFade::FADE_NONE)
					{
						PauseSelect();
					}
				}
			}
			else if (m_pKeyboard != NULL)
			{
				if (m_pKeyboard->GetKey(DIK_ESCAPE) == true)
				{
					CManager::SetPause(false);
				}
				if (m_pKeyboard->GetKey(DIK_W) == true)
				{
					PauseChange(-1);
				}
				if (m_pKeyboard->GetKey(DIK_S) == true)
				{
					PauseChange(1);
				}
				if (m_pKeyboard->GetKey(DIK_J) == true)
				{
					if (CFade::GetFade() == CFade::FADE_NONE)
					{
						PauseSelect();
					}
				}
			}
		}
	}
	else //死亡時
	{
		m_nCntDeath++;
		D3DXVECTOR2 size = GetSize();
		D3DXVECTOR3 pos = GetPos();
		if (m_nCntDeath % 2 == 0)
		{
			pos.x -= 3.0f;
		}
		else
		{
			pos.x += 3.0f;
		}
		if (m_nCntDeath % 5 == 0)
		{
			D3DXVECTOR3 ExPos = (D3DXVECTOR3(pos.x + size.x * (((float)(rand() % 100)) - 50.0f) * 0.016f, pos.y + size.y * (((float)(rand() % 100)) - 50.0f) * 0.016f, 0.0f));
			CExplosion::Create(ExPos, 50.0f, 4);
		}
		if (m_nCntDeath % 30 == 0)
		{
			CSound::Play(CSound::SOUND_LABEL_ENEMYSHATDOWN);
		}
		size.x -= 0.05f;
		size.y -= 0.05f;
		if (m_nCntDeath == 40)
		{
			int nCnt;
			for (nCnt = 0; nCnt < 7; nCnt++)
			{
				if (nCnt > 2)
				{
					m_pGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
				}
			}
			for (nCnt = 0; nCnt < 4; nCnt++)
			{
				if (nCnt > 2)
				{
					m_pHUD[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
				}
			}
		}
		else if (m_nCntDeath >= 80)
		{
			pos.y += 2.0f;
			if (m_nCntDeath > 240 || pos.y > SCREEN_HEIGHT)
			{
				CFade::SetFade(CManager::MODE_RESULT);
			}
			else if (m_nCntDeath == 80)
			{
				int nCnt;
				for (nCnt = 0; nCnt < 3; nCnt++)
				{
					m_pScore[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
				}
				for (nCnt = 0; nCnt < 7; nCnt++)
				{
					m_pGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
				}
				for (nCnt = 0; nCnt < 4; nCnt++)
				{
					m_pHUD[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
				}
			}
		}
		m_pShadow->SetPos(D3DXVECTOR3(pos.x + 80.0f + 80.0f * (float)m_nCntDeath * -0.002f, pos.y + 80.0f + 80.0f * (float)m_nCntDeath * -0.002f, 0.0f));
		m_pShadow->Set();
		SetPos(pos);
		SetSize(size);
		Set();
	}
}

void CPlayer::Draw(void)
{
	CScene2D::Draw();
}

CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nHP, float fSpeed, int nMaxSlow, int nMaxReturn, int nTexType, SHOTTYPE ShotType, int nShadowTex, int nHudTex)
{
	CPlayer *pPlayer;
	pPlayer = new CPlayer(PRIORITY_CHARA);
	if (pPlayer != NULL)
	{
		pPlayer->Init(pos, size, nHP, fSpeed, nMaxSlow, nMaxReturn, ShotType, nShadowTex, nHudTex);
		pPlayer->SetTexture(nTexType);
	}
	return pPlayer;
}

void CPlayer::Move(float fRot)
{
	m_move.x += (m_fMove * sinf(fRot) - m_move.x) * 0.1f;
	m_move.y += (m_fMove * cosf(fRot) - m_move.y) * 0.1f;
}

void CPlayer::Stop(void)
{
	m_move.x -= m_move.x * 0.2f;
	m_move.y -= m_move.y * 0.2f;
}

void CPlayer::Damage(int nDamage)
{
	if (m_nCntDamage <= 0)
	{
		m_Status.nHP -= nDamage;
		if (m_Status.nHP <= 0)
		{
			int nCnt;
			int nColor[4] = { 0,255,0,0 };
			for (nCnt = 0; nCnt < 3; nCnt++)
			{
				m_pTextUI[nCnt]->SetColor(nColor);
			}
			for (nCnt = 0; nCnt < 3; nCnt++)
			{
				m_pScore[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
			for (nCnt = 0; nCnt < 7; nCnt++)
			{
				if (nCnt != 3 && nCnt != 5)
				{
					m_pGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				}
			}
			for (nCnt = 0; nCnt < 4; nCnt++)
			{
				m_pHUD[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
			m_bDeath = true;
			CSound::Play(CSound::SOUND_LABEL_ENEMYSHATDOWN);
		}
		else
		{
			if (m_Status.nHP == 1)
			{
				m_bPinch = true;
				CSound::Play(CSound::SOUND_LABEL_PINCH);
			}
			CSound::Play(CSound::SOUND_LABEL_AIRCRAFTDAMAGE);
			m_nCntDamage = 300;
			ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			char aText[32];
			wsprintf(&aText[0], "DMG:%02d%%", (100 / m_nMaxHP) * (m_nMaxHP - m_Status.nHP));
			m_pTextUI[2]->SetText(&aText[0]);
			int nCnt;
			int nColor[4] = { 255,255,0,0 };
			for (nCnt = 0; nCnt < 3; nCnt++)
			{
				m_pTextUI[nCnt]->SetColor(nColor);
			}
			for (nCnt = 0; nCnt < 3; nCnt++)
			{
				m_pScore[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
			for (nCnt = 0; nCnt < 7; nCnt++)
			{
				if (nCnt != 3 && nCnt != 5)
				{
					m_pGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				}
			}
			for (nCnt = 0; nCnt < 4; nCnt++)
			{
				m_pHUD[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}
	}
}

void CPlayer::CollisionBullet(D3DXVECTOR3 pos)
{
	CScene *pScene;
	CScene *pSceneNext;
	pScene = GetScene(PRIORITY_BULLET);
	while (pScene)
	{
		pSceneNext = pScene->GetNext();
		if (pScene->GetObjType() == OBJECTTYPE_BULLET)
		{
			if (pScene->GetEnemy() == true)
			{
				D3DXVECTOR3 BulletPos = pScene->GetPos();
				D3DXVECTOR3 BulletSize = pScene->GetSize();
				D3DXVECTOR3 PlayerSize = D3DXVECTOR3(3.0f, 3.0f, 0.0f);
				D3DXVECTOR3 Relpos;
				Relpos = pos - BulletPos;
				//float m_fBulletSizeRot = pScene->GetSizeRot();
				float fRotY = pScene->GetRot().y;
				D3DXVECTOR3 TransPos;
				TransPos.x = cosf(fRotY * D3DX_PI) * Relpos.x + sinf(fRotY * D3DX_PI) * Relpos.y;
				TransPos.y = -sinf(fRotY * D3DX_PI) * Relpos.x + cosf(fRotY * D3DX_PI) * Relpos.y;
				//float f = BulletSize.x * sinf(m_fBulletSizeRot * D3DX_PI);
				if (BulletSize.x * sinf(-0.12f * D3DX_PI) <= TransPos.x + PlayerSize.x && TransPos.x - PlayerSize.x <= BulletSize.x * sinf(0.12f * D3DX_PI) &&
					BulletSize.y * cosf(0.88f * D3DX_PI) <= TransPos.y + PlayerSize.y && TransPos.y - PlayerSize.x <= BulletSize.y * cosf(0.12f * D3DX_PI))
				{
					Damage(1);
					pScene->SetDeath(true);
				}
			}
		}
		pScene = pSceneNext;
	}
}

void CPlayer::PauseChange(int nAdd)
{
	m_pPauseUI[m_nPauseSelect]->ColorChange(D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f));
	m_nPauseSelect += nAdd;
	if (m_nPauseSelect < 0)
	{
		m_nPauseSelect = 2;
	}
	else if (m_nPauseSelect > 2)
	{
		m_nPauseSelect = 0;
	}
	m_pPauseUI[m_nPauseSelect]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	CSound::Play(CSound::SOUND_LABEL_CHOICE);
}

void CPlayer::PauseSelect()
{
	switch (m_nPauseSelect)
	{
	case 0:
		CManager::SetPause(false);
		break;
	case 1:
		CFade::SetFade(CManager::MODE_GAME);
		break;
	case 2:
		CFade::SetFade(CManager::MODE_TITLE);
		break;
	default:
		break;
	}
	CSound::Play(CSound::SOUND_LABEL_YES);
}

void CPlayer::TimeKeep(D3DXVECTOR3 pos)
{
	if (m_nReturn >= m_nMaxReturn)
	{
		m_KeepStatus.pos = pos;
		m_KeepStatus.nHP = m_Status.nHP;
		m_pWarp->SetDraw(true);
		m_pWarp->SetPos(pos);
		m_pWarp->Set();
		m_bWarp = true;
		CSound::Play(CSound::SOUND_LABEL_RETURNSET);
		m_nCntWarpPoint = 600;
	}
}

void CPlayer::TimeReturn(D3DXVECTOR3 pos)
{
	m_pGauge[6]->SetSize(D3DXVECTOR2(0.0f, 28.0f));
	m_Status.nHP = m_KeepStatus.nHP;
	m_nCntWarpPoint = 0;
	if (m_Status.nHP == 1)
	{
		m_bPinch = true;
	}
	else
	{
		m_bPinch = false;
	}
	int nCnt;
	int nColor[4] = { 255,0,255,0 };
	for (nCnt = 0; nCnt < 3; nCnt++)
	{
		m_pTextUI[nCnt]->SetColor(nColor);
	}
	for (nCnt = 0; nCnt < 3; nCnt++)
	{
		m_pScore[nCnt]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[1]));
	}
	for (nCnt = 0; nCnt < 7; nCnt++)
	{
		if (nCnt != 3 && nCnt != 5)
		{
			if (nCnt == 4 || nCnt == 6)
			{
				m_pGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 1.0f, m_fClear[0]));
			}
			else
			{
				m_pGauge[nCnt]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[0]));
			}
		}
	}
	for (nCnt = 0; nCnt < 4; nCnt++)
	{
		if (nCnt == 3)
		{
			if (m_nMaxHP / 2 < m_Status.nHP)
			{
				//HP半分より上
				m_fHUDColor = 1.0f / (m_nMaxHP / 2) * (m_nMaxHP - m_Status.nHP);
				m_pHUD[nCnt]->ColorChange(D3DXCOLOR(m_fHUDColor, 1.0f, 0.0f, m_fClear[2]));
			}
			else
			{
				//HP半分より下
				m_fHUDColor = 1.0f - 1.0f / (m_nMaxHP / 2) * (ceilf(m_nMaxHP / 2.0f) - m_Status.nHP);
				m_pHUD[nCnt]->ColorChange(D3DXCOLOR(1.0f, m_fHUDColor, 0.0f, m_fClear[2]));
			}
		}
		else
		{
			m_pHUD[nCnt]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[2]));
		}
	}
	char aText[32];
	wsprintf(&aText[0], "DMG:%02d%%", (100 / m_nMaxHP) * (m_nMaxHP - m_Status.nHP));
	m_pTextUI[2]->SetText(&aText[0]);
	m_nCntDamage = 30;
	ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_bWarpNow = true;
	m_nCntWarp = 7;
	m_nReturn = 0;
	m_pWarp->SetDraw(false);
	m_bWarp = false;
	m_WarpMove = (m_KeepStatus.pos - pos) / m_nCntWarp;
	CSound::Play(CSound::SOUND_LABEL_RETURN);
}

D3DXVECTOR3 CPlayer::Return(D3DXVECTOR3 pos)
{
	pos += m_WarpMove;
	CAfterImage::Create(pos, GetSize() * 0.8f, D3DXCOLOR(1.0f, 0.8f, 1.0f, 1.0f), m_nHudTex);
	return pos;
}

void CPlayer::Shot(D3DXVECTOR3 pos)
{
	switch (m_ShotType)
	{
	case SHOTTYPE_FRONT:
		CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 12.0f, false);
		break;
	case SHOTTYPE_FRONTWIDE:
		CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.12f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, -0.12f, 0.0f), 12.0f, false);
		break;
	case SHOTTYPE_FRONTSIDE:
		CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, -0.5f, 0.0f), 12.0f, false);
		break;
	case SHOTTYPE_FRONTWIDESIDE:
		CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.12f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, -0.12f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, -0.5f, 0.0f), 12.0f, false);
		break;
	case SHOTTYPE_FRONTWIDEBACKSIDE:
		CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.12f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, -0.12f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.88f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, -0.88f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, 1.0f, 0.0f), 12.0f, false);
		break;
	case SHOTTYPE_ALL:
		CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.12f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, -0.12f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, -0.5f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.88f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, -0.88f, 0.0f), 12.0f, false);
		CBullet::Create(pos, D3DXVECTOR3(0.0f, 1.0f, 0.0f), 12.0f, false);
		break;
	default:
		break;
	}
	CSound::Play(CSound::SOUND_LABEL_GUN);
}

void CPlayer::CollisionUI(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 PlayerSize = GetSize() * sinf(D3DX_PI * 0.25f);
	D3DXVECTOR3 UIPos;
	D3DXVECTOR2 UISize;
	int nCntUI;
	int nCnt;
	int nColor[4] = { 0,0,0,0 };
	for (nCntUI = 0; nCntUI < 3; nCntUI++)
	{
		switch (nCntUI)
		{
		case 0:
			UIPos = m_pGauge[1]->GetPos();
			UISize = m_pGauge[1]->GetSize() * sinf(D3DX_PI * 0.25f);
			UISize.x += m_pGauge[0]->GetSize().x * sinf(D3DX_PI * 0.25f) * 2.0f;
			if (pos.x - PlayerSize.x <= UIPos.x + UISize.x && UIPos.x - UISize.x <= pos.x + PlayerSize.x &&
				pos.y - PlayerSize.y <= UIPos.y + UISize.y && UIPos.y - UISize.y <= pos.y + PlayerSize.y)
			{
				if (m_fClear[0] > 0.3f)
				{
					m_fClear[0] -= 0.1f;
					if (m_nCntDamage >= 290)
					{
						for (nCnt = 0; nCnt < 7; nCnt++)
						{
							if (nCnt != 3 && nCnt != 5)
							{
								if (nCnt == 4 || nCnt == 6)
								{
									m_pGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 1.0f, m_fClear[0]));
								}
								else
								{
									m_pGauge[nCnt]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[0]));
								}
							}
							else
							{
								m_pGauge[nCnt]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fClear[0]));
							}
						}
						nColor[0] = 255 * m_fClear[0];
						nColor[1] = 255;
						nColor[2] = 0;
						m_pTextUI[1]->SetColor(nColor);
					}
					else
					{
						for (nCnt = 0; nCnt < 7; nCnt++)
						{
							if (nCnt != 3 && nCnt != 5)
							{
								if (nCnt == 4 || nCnt == 6)
								{
									m_pGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 1.0f, m_fClear[0]));
								}
								else
								{
									m_pGauge[nCnt]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[0]));
								}
							}
							else
							{
								m_pGauge[nCnt]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fClear[0]));
							}
						}
						nColor[0] = 255 * m_fClear[0];
						nColor[1] = 0;
						nColor[2] = 255;
						m_pTextUI[1]->SetColor(nColor);
					}
				}
			}
			else
			{
				if (m_fClear[0] < 1.0f)
				{
					m_fClear[0] += 0.1f;
					if (m_nCntDamage >= 290)
					{
						for (nCnt = 0; nCnt < 7; nCnt++)
						{
							if (nCnt != 3 && nCnt != 5)
							{
								if (nCnt == 4 || nCnt == 6)
								{
									m_pGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 1.0f, m_fClear[0]));
								}
								else
								{
									m_pGauge[nCnt]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[0]));
								}
							}
							else
							{
								m_pGauge[nCnt]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fClear[0]));
							}
						}
						nColor[0] = 255 * m_fClear[0];
						nColor[1] = 255;
						nColor[2] = 0;
						m_pTextUI[1]->SetColor(nColor);
					}
					else
					{
						for (nCnt = 0; nCnt < 7; nCnt++)
						{
							if (nCnt != 3 && nCnt != 5)
							{
								if (nCnt == 4 || nCnt == 6)
								{
									m_pGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 1.0f, m_fClear[0]));
								}
								else
								{
									m_pGauge[nCnt]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[0]));
								}
							}
							else
							{
								m_pGauge[nCnt]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fClear[0]));
							}
						}
						nColor[0] = 255 * m_fClear[0];
						nColor[1] = 0;
						nColor[2] = 255;
						m_pTextUI[1]->SetColor(nColor);
					}
				}
			}
			break;
		case 1:
			UIPos = m_pScore[1]->GetPos();
			UISize = m_pScore[1]->GetSize() * sinf(D3DX_PI * 0.25f);
			UISize.x += m_pScore[0]->GetSize().x * sinf(D3DX_PI * 0.25f) * 2.0f;
			if (pos.x - PlayerSize.x <= UIPos.x + UISize.x && UIPos.x - UISize.x <= pos.x + PlayerSize.x &&
				pos.y - PlayerSize.y <= UIPos.y + UISize.y && UIPos.y - UISize.y <= pos.y + PlayerSize.y)
			{
				if (m_fClear[1] > 0.3f)
				{
					m_fClear[1] -= 0.1f;
					if (m_nCntDamage >= 290)
					{
						for (nCnt = 0; nCnt < 3; nCnt++)
						{
							m_pScore[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[1]));
						}
						nColor[0] = 255 * m_fClear[1];
						nColor[1] = 255;
						nColor[2] = 0;
						m_pTextUI[0]->SetColor(nColor);
					}
					else
					{
						for (nCnt = 0; nCnt < 3; nCnt++)
						{
							m_pScore[nCnt]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[1]));
						}
						nColor[0] = 255 * m_fClear[1];
						nColor[1] = 0;
						nColor[2] = 255;
						m_pTextUI[0]->SetColor(nColor);
					}
				}
			}
			else
			{
				if (m_fClear[1] < 1.0f)
				{
					m_fClear[1] += 0.1f;
					if (m_nCntDamage >= 290)
					{
						for (nCnt = 0; nCnt < 3; nCnt++)
						{
							m_pScore[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[1]));
						}
						nColor[0] = 255 * m_fClear[1];
						nColor[1] = 255;
						nColor[2] = 0;
						m_pTextUI[0]->SetColor(nColor);
					}
					else
					{
						for (nCnt = 0; nCnt < 3; nCnt++)
						{
							m_pScore[nCnt]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[1]));
						}
						nColor[0] = 255 * m_fClear[1];
						nColor[1] = 0;
						nColor[2] = 255;
						m_pTextUI[0]->SetColor(nColor);
					}
				}
			}
			break;
		case 2:
			UIPos = m_pHUD[1]->GetPos();
			UISize = m_pHUD[1]->GetSize() * sinf(D3DX_PI * 0.25f);
			UISize.y += m_pHUD[0]->GetSize().y * sinf(D3DX_PI * 0.25f) * 2.0f;
			if (pos.x - PlayerSize.x <= UIPos.x + UISize.x && UIPos.x - UISize.x <= pos.x + PlayerSize.x &&
				pos.y - PlayerSize.y <= UIPos.y + UISize.y && UIPos.y - UISize.y <= pos.y + PlayerSize.y)
			{
				if (m_fClear[2] > 0.3f)
				{
					m_fClear[2] -= 0.1f;
					if (m_nCntDamage >= 290)
					{
						for (nCnt = 0; nCnt < 4; nCnt++)
						{
							m_pHUD[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2]));
						}
						nColor[0] = 255 * m_fClear[2];
						nColor[1] = 0;
						nColor[2] = 255;
						m_pTextUI[2]->SetColor(nColor);
					}
					else
					{
						for (nCnt = 0; nCnt < 4; nCnt++)
						{
							if (nCnt == 3)
							{
								if (m_nMaxHP / 2 <= m_Status.nHP)
								{
									//HP半分より上
									m_pHUD[nCnt]->ColorChange(D3DXCOLOR(m_fHUDColor, 1.0f, 0.0f, m_fClear[2]));
								}
								else
								{
									//HP半分より下
									m_pHUD[nCnt]->ColorChange(D3DXCOLOR(1.0f, m_fHUDColor, 0.0f, m_fClear[2]));
								}
							}
							else
							{
								m_pHUD[nCnt]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[2]));
							}
						}
						nColor[0] = 255 * m_fClear[2];
						nColor[1] = 0;
						nColor[2] = 255;
						m_pTextUI[2]->SetColor(nColor);
					}
				}
			}
			else
			{
				if (m_fClear[2] < 1.0f)
				{
					m_fClear[2] += 0.1f;
					if (m_nCntDamage >= 290)
					{
						for (nCnt = 0; nCnt < 4; nCnt++)
						{
							m_pHUD[nCnt]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2]));
						}
						nColor[0] = 255 * m_fClear[2];
						nColor[1] = 0;
						nColor[2] = 255;
						m_pTextUI[2]->SetColor(nColor);
					}
					else
					{
						for (nCnt = 0; nCnt < 4; nCnt++)
						{
							if (nCnt == 3)
							{
								if (m_nMaxHP / 2 <= m_Status.nHP)
								{
									//HP半分より上
									m_pHUD[nCnt]->ColorChange(D3DXCOLOR(m_fHUDColor, 1.0f, 0.0f, m_fClear[2]));
								}
								else
								{
									//HP半分より下
									m_pHUD[nCnt]->ColorChange(D3DXCOLOR(1.0f, m_fHUDColor, 0.0f, m_fClear[2]));
								}
							}
							else
							{
								m_pHUD[nCnt]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[2]));
							}
						}
						nColor[0] = 255 * m_fClear[2];
						nColor[1] = 0;
						nColor[2] = 255;
						m_pTextUI[2]->SetColor(nColor);
					}
				}
			}
			break;
		default:
			break;
		}
	}
}

void CPlayer::AddScore(int nScore)
{
	m_nScore += nScore;
	char aText[32];
	wsprintf(&aText[0], "SCORE:%08d", m_nScore);
	m_pTextUI[0]->SetText(aText);
}