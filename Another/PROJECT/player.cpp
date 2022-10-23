//---------------------------
//Author:三上航世
//モデル(player.cpp)
//---------------------------
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "mouse.h"
#include "keyboard.h"
#include "model.h"
#include "camera.h"
#include "fade.h"
#include "shadow.h"
#include "gamepad.h"
#include "game.h"
#include "sound.h"
#include "orbit.h"
#include "particle.h"
#include "rockon.h"
#include "ui.h"
#include "pauseui.h"
#include "enemy.h"

//=============================================================================
//静的
//=============================================================================

//=============================================================================
//マクロ
//=============================================================================
#define RAND_COLOR (int(rand()%255) + 30)	//色ランダム
#define RAND_MOVE ((int(rand() % 3) + 1 )- (int (rand()%3) + 1))	//移動値ランダム
#define RAND_MOVE_2 ((int(rand() % 3) + 1 ))	//移動値ランダム

CPlayer::CPlayer(PRIORITY Priority) : CScene3D::CScene3D(Priority)
{

}

CPlayer::~CPlayer()
{

}

//初期化処理
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *aFileName)
{
	Load(aFileName);
	SetPos(pos);
	SetRot(rot);
	m_nMaxHP = m_nHP;
	m_rotDesh = rot;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_RestartPos = pos;
	m_PosOld = pos;
	m_pKeyboard = CManager::GetKeyboard();
	m_pGamePad = CManager::GetGamepad();
	m_pMouse = CManager::GetMouse();
	m_pCamera = CManager::GetRenderer()->GetCamera();
	m_pRockon = CRockon::Create();
	m_pCamera->ResetRot();
	m_pCamera->SetPlayer(this);
	m_bJump = true;
	m_bLandObject = false;
	m_bHit = false;
	m_bCounter = false;
	m_nComboTime = 0;
	m_nNonOpe = 0;
	m_motionTypeOld = MOTIONTYPE_NEUTRAL;
	m_nRunStop = 0;
	m_nCntDamage = 0;
	m_nCntCol = 0;
	m_bRockon = false;
	m_nCntNoDmg = 0;
	m_nCntRun = 0;
	m_nPauseSelect = 0;
	m_nCntDeath = 0;
	m_nCntRoundChange = -1;
	m_bGameClear = false;
	m_bEndSelect = false;
	m_bPinch = false;
	D3DXVECTOR3 size;
	size.x = GetRadius();
	size.y = 0.0f;
	size.z = GetRadius();
	m_pShadow = CShadow::Create(size, D3DXVECTOR3(pos.x, pos.y + 0.1f, pos.z));
	MotionChange(MOTIONTYPE_RETURN);
	m_bLanding = true;
	int nCntOrbit;
	for (nCntOrbit = 0; nCntOrbit < 4; nCntOrbit++)
	{
		m_pOrbit[nCntOrbit] = COrbit::Create();
	}
	if (m_pGamePad != NULL)
	{
		m_pCounterUI = CUI::Create(D3DXVECTOR3(640.0f, 600.0f, 0.0f), 300.0f, 80.0f, 6, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));	//カウンターゲームパッド
	}
	else
	{
		m_pCounterUI = CUI::Create(D3DXVECTOR3(640.0f, 600.0f, 0.0f), 300.0f, 80.0f, 5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));	//カウンターキーボード
	}
	m_pHPUI = CUI::Create(D3DXVECTOR3(1037.5f, 620.0f, 0.0), 298.0f, 58.0f, 2, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	//HPゲージ
	m_pEnemyHPUI[0] = CUI::Create(D3DXVECTOR3(1230.0f, 60.0f, 0.0), 16.0f, 40.0f, 1, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
	m_pEnemyHPUI[1] = CUI::Create(D3DXVECTOR3(1097.0f, 60.0f, 0.0), 250.0f, 40.0f, -1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	m_pEnemyHPUI[2] = CUI::Create(D3DXVECTOR3(1097.0f, 60.0f, 0.0), 248.0f, 38.0f, 7, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_pPauseUI[0] = CPauseUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100.0f, 0.0f), 240.0f, 60.0f, 9, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	m_pPauseUI[1] = CPauseUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 190.0f, 60.0f, 10, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f));
	m_pPauseUI[2] = CPauseUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 100.0f, 0.0f), 150.0f, 60.0f, 11, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f));
	m_pRoundUI[0] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 50.0f, 0.0f), 200.0f, 60.0f, 14, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pRoundUI[1] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 30.0f, 100.0f, 0.0f), 50.0f, 65.0f, 12, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	m_pRoundUI[2] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 110.0f, 0.0f), 20.0f, 50.0f, 13, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pRoundUI[3] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 20.0f, 120.0f, 0.0f), 30.0f, 40.0f, 12, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	m_pRoundUI[1]->SetTex(1, 0.1f);
	m_pRoundUI[3]->SetTex(MAX_ROUND, 0.1f);
	return S_OK;
}

//終了処理
void CPlayer::Uninit()
{
	if (m_pCamera != NULL)
	{
		m_pCamera = NULL;
	}
	if (m_pGamePad != NULL)
	{
		m_pGamePad = NULL;
	}
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard = NULL;
	}
	int nCnt;
	for (nCnt = 0; nCnt < 32; nCnt++)
	{
		if (m_pModel[nCnt] != NULL)
		{
			m_pModel[nCnt]->Uninit();
			m_pModel[nCnt] = NULL;
		}
	}
	if (m_pMouse != NULL)
	{
		m_pMouse = NULL;
	}
	if (m_pShadow != NULL)
	{
		m_pShadow->SetDeath(true);
		m_pShadow = NULL;
	}
	for (nCnt = 0; nCnt < 4; nCnt++)
	{
		if (m_pOrbit[nCnt] != NULL)
		{
			m_pOrbit[nCnt] = NULL;
		}
	}
	for (nCnt = 0; nCnt < 16; nCnt++)
	{
		if (m_pEnemy[nCnt] != NULL)
		{
			m_pEnemy[nCnt] = NULL;
		}
	}
	if (m_pCounterUI != NULL)
	{
		m_pCounterUI = NULL;
	}
	if (m_pHPUI != NULL)
	{
		m_pHPUI = NULL;
	}
	for (nCnt = 0; nCnt < 3; nCnt++)
	{
		if (m_pEnemyHPUI[nCnt] != NULL)
		{
			m_pEnemyHPUI[nCnt] = NULL;
		}
	}
	for (nCnt = 0; nCnt < 3; nCnt++)
	{
		if (m_pPauseUI[nCnt] != NULL)
		{
			m_pPauseUI[nCnt] = NULL;
		}
	}
	for (nCnt = 0; nCnt < 4; nCnt++)
	{
		if (m_pRoundUI[nCnt] != NULL)
		{
			m_pRoundUI[nCnt] = NULL;
		}
	}
	for (nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pEndUI[nCnt] != NULL)
		{
			m_pEndUI[nCnt] = NULL;
		}
	}
	CScene3D::Uninit();
}

//更新処理
void CPlayer::Update()
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	m_PosOld = pos;
	bool bRun = false;
	bool bCamera = false;
	if (m_bLandObject == true && m_bJump == false)
	{
		m_bJump = true;
	}
	if (m_bJump == false && m_bLanding == true)
	{
		m_bLanding = false;
	}
	if (CManager::GetGameClear() == true && m_bGameClear == false)
	{
		GameClear();
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	if (m_bRockon == true)
	{
		if (m_pRockon->GetRockEnemy() == NULL)
		{
			m_bRockon = false;
		}
	}
	//この方法でしかPause実装思い出せませんでした、すまん
	bool Pause = CManager::GetPause();

	if (m_motionType != MOTIONTYPE_DEATH && m_motionType != MOTIONTYPE_CLEAR)
	{
		if (Pause == false && CManager::GetCountdown() == false && CManager::GetEnd() == false)
		{
			if (m_nComboTime > 0)
			{
				m_nComboTime--;
			}
			if (m_nNonOpe > 0)
			{
				m_nNonOpe--;
			}
			if (m_nCntNoDmg > 0)
			{
				m_nCntNoDmg--;
			}
			if (m_bPinch == true)
			{
				m_nCntPinch--;
				if (m_nCntPinch <= 0)
				{
					m_pHPUI->ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_nCntPinch = 20;
				}
				else if (m_nCntPinch % 10 == 0)
				{
					m_pHPUI->ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}
			}
			if (m_motionType == MOTIONTYPE_RUN && m_nCntRun > 0)
			{
				m_nCntRun--;
				if (m_nCntRun <= 0)
				{
					m_nCntRun = 12;
					CSound::Play(CSound::SOUND_LABEL_FOOTSTEP);
				}
			}
			if (m_nCntRoundChange > 0)
			{
				m_nCntRoundChange--;
				if (m_nCntRoundChange % 10 == 0)
				{
					m_pRoundUI[1]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
				}
				else if (m_nCntRoundChange % 5 == 0)
				{
					m_pRoundUI[1]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
				}
			}
			if (m_pGamePad != NULL)
			{
				if (m_nNonOpe <= 0)
				{
					if (fabsf(m_pGamePad->LeftStickX()) >= 0.3f || fabsf(m_pGamePad->LeftStickY()) >= 0.3f)
					{
						bRun = Move(atan2f(m_pGamePad->LeftStickX(), -m_pGamePad->LeftStickY()) / D3DX_PI);
					}
					else
					{
						bRun = Stop();
					}
					if (bRun == true && m_bJump == true && m_motionType != MOTIONTYPE_RUN)
					{
						MotionChange(MOTIONTYPE_RUN);
						m_nCombo = 0;
						m_nCntRun = 6;
					}
					if (bRun == false && m_motionType == MOTIONTYPE_RUN)
					{
						m_nRunStop++;
						if (m_nRunStop > 10)
						{
							MotionChange(MOTIONTYPE_RETURN);
							m_nRunStop = 0;
							m_nCntRun = -1;
						}
					}
					if (m_pGamePad->GetButton(CGamePad::DIP_A) == true)
					{
						Jump();
					}
				}
				else
				{
					bRun = Stop();
				}
				if (m_nNonOpe <= 0 || m_bHit == true)
				{
					if (m_nComboTime <= 0)
					{
						if (m_pGamePad->GetButton(CGamePad::DIP_B) == true)
						{
							if (m_bRockon == true)
							{
								MotionChange(RockAttack(pos));
							}
							else
							{
								MotionChange(NoRockAttack());
							}
						}
					}
					if (m_pGamePad->GetButton(CGamePad::DIP_X) == true)
					{
						m_nCombo = 0;
						if (sqrtf(m_pGamePad->LeftStickX()) >= 0.3f || sqrtf(m_pGamePad->LeftStickY()) >= 0.3f)
						{
							float fRotY = atan2f(m_pGamePad->LeftStickX(), -m_pGamePad->LeftStickY());
							float fCameraRotY = m_pCamera->GetRotY();
							m_rotDesh.y = fCameraRotY + fRotY + D3DX_PI;
							if (m_bJump == false)
							{
								MotionChange(MOTIONTYPE_AIRDODGE);
								CSound::Play(CSound::SOUND_LABEL_DODGE);
							}
							else
							{
								MotionChange(MOTIONTYPE_GROUNDDODGE);
								CSound::Play(CSound::SOUND_LABEL_DODGE);
							}
						}
						else
						{
							MotionChange(MOTIONTYPE_GUARD);
						}
					}
				}
				if (m_pGamePad->GetButton(CGamePad::DIP_R1) == true)
				{
					if (m_bRockon == false)
					{
						Rockon();
					}
					else
					{
						UnRockon();
					}
				}
				if (sqrtf(m_pGamePad->RightStickX()) >= 0.3f)
				{
					m_pCamera->AddRotY(m_pGamePad->RightStickX() * 0.03f);
					bCamera = true;
				}
				if (sqrtf(m_pGamePad->RightStickY()) >= 0.3f)
				{
					m_pCamera->AddRotX(m_pGamePad->RightStickY() * -0.03f);
					bCamera = true;
				}
				m_pCamera->SetAdd(bCamera);
				if (m_pGamePad->GetButton(CGamePad::DIP_START) == true)
				{
					CManager::SetPause(true);
					CSound::Play(CSound::SOUND_LABEL_PAUSE);
				}
			}
			else if (m_pKeyboard != NULL)
			{
				if (m_nNonOpe <= 0)
				{
					if (m_pKeyboard->GetPress(DIK_A) == true)
					{
						if (m_pKeyboard->GetPress(DIK_S) == true)
						{
							bRun = Move(-0.75f);
						}
						else if (m_pKeyboard->GetPress(DIK_W) == true)
						{
							bRun = Move(-0.25f);
						}
						else
						{
							bRun = Move(-0.5f);
						}
					}
					else if (m_pKeyboard->GetPress(DIK_D) == true)
					{
						if (m_pKeyboard->GetPress(DIK_S) == true)
						{
							bRun = Move(0.75f);
						}
						else if (m_pKeyboard->GetPress(DIK_W) == true)
						{
							bRun = Move(0.25f);
						}
						else
						{
							bRun = Move(0.5f);
						}
					}
					else if (m_pKeyboard->GetPress(DIK_W) == true)
					{
						bRun = Move(0.0f);
					}
					else if (m_pKeyboard->GetPress(DIK_S) == true)
					{
						bRun = Move(1.0f);
					}
					else
					{
						bRun = Stop();
					}
					if (bRun == true && m_bJump == true && m_motionType != MOTIONTYPE_RUN)
					{
						MotionChange(MOTIONTYPE_RUN);
						m_nCombo = 0;
						m_nCntRun = 6;
					}
					if (bRun == false && m_motionType == MOTIONTYPE_RUN)
					{
						m_nRunStop++;
						if (m_nRunStop > 10)
						{
							MotionChange(MOTIONTYPE_RETURN);
							m_nRunStop = 0;
							m_nCntRun = -1;
						}
					}
					if (m_pKeyboard->GetKey(DIK_SPACE) == true && m_bJump == true)
					{
						Jump();
					}
				}
				else
				{
					Stop();
				}
				if (m_nNonOpe <= 0 || m_bHit == true)
				{
					if (m_nComboTime <= 0)
					{
						if (m_pKeyboard->GetKey(DIK_J) == true)
						{
							if (m_bRockon == true)
							{
								MotionChange(RockAttack(pos));
							}
							else
							{
								MotionChange(NoRockAttack());
							}
						}
					}
					if (m_pKeyboard->GetKey(DIK_I) == true)
					{
						m_nCombo = 0;
						if (m_pKeyboard->GetPress(DIK_W) == true || m_pKeyboard->GetPress(DIK_S) == true ||
							m_pKeyboard->GetPress(DIK_A) == true || m_pKeyboard->GetPress(DIK_D) == true)
						{
							float fRotY = 1.0f;
							if (m_pKeyboard->GetPress(DIK_A) == true)
							{
								if (m_pKeyboard->GetPress(DIK_S) == true)
								{
									fRotY = -0.75f;
								}
								else if (m_pKeyboard->GetPress(DIK_W) == true)
								{
									fRotY = -0.25f;
								}
								else
								{
									fRotY = -0.5f;
								}
							}
							else if (m_pKeyboard->GetPress(DIK_D) == true)
							{
								if (m_pKeyboard->GetPress(DIK_S) == true)
								{
									fRotY = 0.75f;
								}
								else if (m_pKeyboard->GetPress(DIK_W) == true)
								{
									fRotY = 0.25f;
								}
								else
								{
									fRotY = 0.5f;
								}
							}
							else if (m_pKeyboard->GetPress(DIK_W) == true)
							{
								fRotY = 0.0f;
							}
							else if (m_pKeyboard->GetPress(DIK_S) == true)
							{
								fRotY = 1.0f;
							}
							float fCameraRotY = m_pCamera->GetRotY();
							m_rotDesh.y = fCameraRotY + D3DX_PI * fRotY + D3DX_PI;
							if (m_rotDesh.y >= D3DX_PI)
							{
								m_rotDesh.y -= D3DX_PI * 2;
							}
							if (m_rotDesh.y < -D3DX_PI)
							{
								m_rotDesh.y += D3DX_PI * 2;
							}
							rot.y = m_rotDesh.y;
							if (m_bJump == false)
							{
								MotionChange(MOTIONTYPE_AIRDODGE);
								CSound::Play(CSound::SOUND_LABEL_DODGE);
							}
							else
							{
								MotionChange(MOTIONTYPE_GROUNDDODGE);
								CSound::Play(CSound::SOUND_LABEL_DODGE);
							}
						}
						else
						{
							MotionChange(MOTIONTYPE_GUARD);
						}	
					}
				}
				if (m_pKeyboard->GetKey(DIK_R) == true)
				{
					if (m_bRockon == false)
					{
						Rockon();
					}
					else
					{
						UnRockon();
					}
				}
				if (m_pKeyboard->GetKey(DIK_ESCAPE) == true)
				{
					CManager::SetPause(true);
					CSound::Play(CSound::SOUND_LABEL_PAUSE);
				}
				if (m_pMouse != NULL)
				{
					if (m_pMouse->MouseX() != 0.0f || m_pMouse->MouseY() != 0.0f)
					{
						m_pCamera->AddRotY(m_pMouse->MouseX() * 0.001f);
						m_pCamera->AddRotX(m_pMouse->MouseY() * -0.002f);
					}
					else
					{
						m_pCamera->SetAdd(false);
					}
				}
			}
		}
		else if(Pause == true && CManager::GetCountdown() == false)
		{
			if (m_pGamePad != NULL)
			{
				if (m_pGamePad->GetButton(CGamePad::DIP_START) == true)
				{
					CManager::SetPause(false);
					CSound::Play(CSound::SOUND_LABEL_CANCEL);
				}
				if (m_pGamePad->TriggerCrossKey() == 0.0f)
				{
					PauseChange(-1);
				}
				else if (m_pGamePad->TriggerCrossKey() == 18000.0f)
				{
					PauseChange(1);
				}
				if (m_pGamePad->GetButton(CGamePad::DIP_B) == true)
				{
					PauseSelect();
				}
			}
			else if (m_pKeyboard != NULL)
			{
				if (m_pKeyboard->GetKey(DIK_ESCAPE) == true)
				{
					CManager::SetPause(false);
					CSound::Play(CSound::SOUND_LABEL_CANCEL);
				}
				if (m_pKeyboard->GetKey(DIK_W) == true || m_pKeyboard->GetKey(DIK_UP) == true)
				{
					PauseChange(-1);
				}
				else if (m_pKeyboard->GetKey(DIK_S) == true || m_pKeyboard->GetKey(DIK_DOWN) == true)
				{
					PauseChange(1);
				}
				if (m_pKeyboard->GetKey(DIK_RETURN) == true)
				{
					PauseSelect();
				}
			}
		}
		if (m_bRockon == true)
		{
			SearchRock(pos);
		}
		else
		{
			if (m_MotionInfo.aKeyInfo[m_nKey].fMove == 0.0f)
			{
				MotionStopMove();
			}
			else if (m_MotionInfo.bHoming == false)
			{
				MotionMove(m_rotDesh.y, m_MotionInfo.aKeyInfo[m_nKey].fMove);
			}
			else
			{
				MotionMove(m_rotDesh.y, m_MotionInfo.aKeyInfo[m_nKey].fMove * 0.5f);
			}
		}
		if (Pause == false && CManager::GetCountdown() == false && CManager::GetEnd() == false)
		{
			pos += m_move + m_MotionMove;
			CollisionEnemy(&pos);
			CollisionObject(&pos, m_PosOld);
			CollisionPlane(&pos, m_PosOld);
			PlayerDamage(pos, m_fHeight, m_fRadius);
			if (m_bJump == false && m_MotionInfo.bGravity == true)
			{
				if (m_move.y > -25.0f)
				{
					m_move.y -= m_fGravity;
				}
				else
				{
					m_move.y = -25.0f;
				}
			}
			if (m_bJump == false)
			{
				UnderSearch(pos);
			}
			if (m_bJump == false && m_motionType == MOTIONTYPE_NEUTRAL || m_bJump == false && m_motionType == MOTIONTYPE_RUN)
			{
				MotionChange(MOTIONTYPE_AIRRETURN);
			}
			if (m_MotionInfo.bGravity == false && m_move.y != 0.0f)
			{
				m_move.y = 0.0f;
			}
			m_pShadow->Move(pos.y);
			if (m_bRockon == true)
			{
				RockCameraMove(pos);
			}
		}
	}
	else
	{
		if (m_bEndSelect == true)
		{
			if (m_pGamePad != NULL)
			{
				if (m_pGamePad->TriggerCrossKey() == 0.0f)
				{
					EndChange(-1);
				}
				else if (m_pGamePad->TriggerCrossKey() == 18000.0f)
				{
					EndChange(1);
				}
				if (m_pGamePad->GetButton(CGamePad::DIP_B) == true)
				{
					EndSelect();
				}
			}
			else if (m_pKeyboard != NULL)
			{
				if (m_pKeyboard->GetKey(DIK_W) == true || m_pKeyboard->GetKey(DIK_UP) == true)
				{
					EndChange(-1);
				}
				else if (m_pKeyboard->GetKey(DIK_S) == true || m_pKeyboard->GetKey(DIK_DOWN) == true)
				{
					EndChange(1);
				}
				if (m_pKeyboard->GetKey(DIK_RETURN) == true)
				{
					EndSelect();
				}
			}
		}
		else if (m_nCntDeath > 150)
		{
			m_bEndSelect = true;
			CManager::SetEnd(true);
			CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, -1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f));
			if (m_nHP <= 0)
			{
				CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 120.0f, 0.0f), 270.0f, 80.0f, 20, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 120.0f, 0.0f), 280.0f, 80.0f, 21, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			m_pEndUI[0] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 500.0f, 0.0f), 220.0f, 80.0f, 10, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_pEndUI[1] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 600.0f, 0.0f), 170.0f, 80.0f, 11, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
			m_nPauseSelect = 0;
			CSound::Play(CSound::SOUND_LABEL_SELECT);
		}
		else
		{
			m_nCntDeath++;
		}
	}
	if (rot.y >= D3DX_PI)
	{
		rot.y -= D3DX_PI * 2;
	}
	if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2;
	}
	if (rot.y != m_rotDesh.y)
	{
		if (m_rotDesh.y - rot.y < -D3DX_PI)
		{
			rot.y += (m_rotDesh.y - rot.y + D3DX_PI * 2) * 0.14f;
		}
		else if (m_rotDesh.y - rot.y > D3DX_PI)
		{
			rot.y += (m_rotDesh.y - rot.y - D3DX_PI * 2) * 0.14f;
		}
		else
		{
			rot.y += (m_rotDesh.y - rot.y) * 0.14f;
		}
	}
	if (Pause == false && CManager::GetEnd() == false)
	{
		Motion();
		SetRot(rot);
		if (pos.y < -1000.0f)
		{
			pos = m_RestartPos;
		}
		SetPos(pos);
		if (CGame::GetRoundChange() == true)
		{
			m_pRoundUI[1]->SetTex(CGame::GetRound(), 0.1f);
			m_nCntRoundChange = 60;
		}
		////チート操作
		//if (m_pKeyboard != NULL)
		//{
		//	//リスポーン地点に戻る
		//	if (m_pKeyboard->GetKey(DIK_RETURN) == true)
		//	{
		//		m_MotionMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//		SetPos(m_RestartPos);
		//		m_pCamera->ResetRot();
		//	}
		//	//リスポーン地点の設定
		//	if (m_pKeyboard->GetKey(DIK_LSHIFT) == true)
		//	{
		//		m_RestartPos = pos;
		//	}
		//}
	}
}

//描画処理
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;   //デバイスのポインタ
	D3DXMATRIX mtxPlayer, mtxRot, mtxTrans;
	pDevice = CManager::GetRenderer()->GetDevice();		 //デバイスを取得する
	
	//プレイヤーのワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxPlayer);
	//プレイヤーの向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);
	D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxRot);
	//プレイヤーの位置を反映
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxTrans);
	//プレイヤーのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxPlayer);
	SetMatrix(mtxPlayer);
	int nCntModel;
	for (nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		if (m_pModel[nCntModel] != NULL)
		{
			m_pModel[nCntModel]->Draw();
		}
	}
	//CollisionPos();
	Orbit();
}

void CPlayer::Load(const char *aFileName)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する
	D3DXVECTOR3 rot;
	D3DXVECTOR3 pos;
	int nParent;
	CModel *paModel[32];

	int nCntParts = 0;
	int nCntMotion = 0;
	int nCntKey = 0;
	int nCntCol = 0;
	int nCntObt = 0;
	int nCntDmg = 0;
	char aFile[256];
	int nLoop = 0;
	int nHorming = 0;
	int nUpdn = 0;
	int nGravity = 0;

	bool bChara = false;
	bool bParts = false;
	bool bMotion = false;
	bool bOrbit = false;
	bool bOrbitSet = false;
	bool bKey = false;
	bool bKeySet = false;
	bool bColSet = false;
	bool bCol = false;
	bool bDmgSet = false;
	bool bDmg = false;
	int nIndex;

	FILE *pFile;
	pFile = fopen(aFileName, "r");
	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]); //fscanfを繰り返してファイルを読み取っていく
			if (strcmp(&aFile[0], "#") == 0) //モデルファイル読み込み
			{
				fscanf(pFile, "%s", &aFile[0]);
			}
			if (strcmp(&aFile[0], "MODEL_FILENAME") == 0) //モデルファイル読み込み
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%s", &aFile[0]);
				paModel[nCntParts] = CModel::Create(&aFile[0]);
				nCntParts++;
			}
			if (bChara == true) //CHARACTERSET時
			{
				if (strcmp(&aFile[0], "NUM_PARTS") == 0) //パーツ数
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_nNumModel);
				}
				if (strcmp(&aFile[0], "MOVE") == 0) //移動速度
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &m_fMove);
				}
				if (strcmp(&aFile[0], "JUMP") == 0) //ジャンプ力
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &m_fJump);
				}
				if (strcmp(&aFile[0], "GRAVITY") == 0) //重力
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &m_fGravity);
				}
				if (strcmp(&aFile[0], "RADIUS") == 0) //半径
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &m_fRadius);
				}
				if (strcmp(&aFile[0], "HEIGHT") == 0) //身長
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &m_fHeight);
				}
				if (strcmp(&aFile[0], "HP") == 0) //体力
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_nHP);
				}
				if (strcmp(&aFile[0], "PARTSSET") == 0)
				{
					bParts = true;
				}
				if (strcmp(&aFile[0], "END_PARTSSET") == 0)
				{
					bParts = false;
					nCntParts++;
				}
				if (bParts == true) //PARTSSET時
				{
					//if (strcmp(&aFile[0], "MOVEPARTS") == 0) //これが動くとg_playerのposも動く（いらないかも）
					//{
					//	g_player.nMoveParts = nCntParts;
					//}
					if (strcmp(&aFile[0], "INDEX") == 0) //モデルファイル読み込みの時の、何番目のモデルを呼び出すか
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &nIndex);
						m_pModel[nCntParts] = paModel[nIndex];
					}
					if (strcmp(&aFile[0], "PARENT") == 0) //親モデル
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &nParent);
						if (nParent != -1)
						{
							m_pModel[nCntParts]->SetParent(m_pModel[nParent]);
						}
					}
					if (strcmp(&aFile[0], "POS") == 0) //各パーツの場所
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
						m_pModel[nCntParts]->SetOriPos(pos);
					}
					if (strcmp(&aFile[0], "ROT") == 0) //各パーツの角度
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
						m_pModel[nCntParts]->SetOriRot(rot);
					}
				}
			}
			if (bMotion == true && nCntMotion < MOTIONTYPE_RETURN) //MOTIONSET時
			{
				if (strcmp(&aFile[0], "LOOP") == 0) //そのモーションがループするか
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nLoop);
					if (nLoop == 0)
					{
						m_aMotionInfo[nCntMotion].bLoop = false;
					}
					else
					{
						m_aMotionInfo[nCntMotion].bLoop = true;
					}
				}
				if (strcmp(&aFile[0], "HORMING") == 0) //移動量が減るか
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nHorming);
					if (nHorming == 0)
					{
						m_aMotionInfo[nCntMotion].bHoming = false;
					}
					else
					{
						m_aMotionInfo[nCntMotion].bHoming = true;
					}
				}
				if (strcmp(&aFile[0], "GRAVITY") == 0) //重力が働くか
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nGravity);
					if (nGravity == 0)
					{
						m_aMotionInfo[nCntMotion].bGravity = false;
					}
					else
					{
						m_aMotionInfo[nCntMotion].bGravity = true;
					}
				}
				if (strcmp(&aFile[0], "UPDN") == 0) //上下にどう動くか
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nUpdn);
					switch (nUpdn)
					{
					case 0:
						m_aMotionInfo[nCntMotion].MotionUpdn = CPlayer::MOTIONUPDN_NONE;
						break;
					case 1:
						m_aMotionInfo[nCntMotion].MotionUpdn = CPlayer::MOTIONUPDN_UNDER;
						break;
					case 2:
						m_aMotionInfo[nCntMotion].MotionUpdn = CPlayer::MOTIONUPDN_UPPER;
						break;
					case 3:
						m_aMotionInfo[nCntMotion].MotionUpdn = CPlayer::MOTIONUPDN_UPDN;
						break;
					default:
						break;
					}
				}
				if (strcmp(&aFile[0], "NUM_KEY") == 0) //いくつキーがあるか
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].nNumKey);
				}
				if (strcmp(&aFile[0], "NON_OPE") == 0) //操作不能時間
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].nNonOpe);
				}
				if (strcmp(&aFile[0], "COMBO_TIME") == 0) //コンボができるまでの時間
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].nComboTime);
				}
				if (strcmp(&aFile[0], "NO_DAMAGE") == 0) //無敵時間
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].nNoDmg);
				}
				if (strcmp(&aFile[0], "NO_ATTACK") == 0) //攻撃しないモーション
				{
					m_aMotionInfo[nCntMotion].Damage[0].nStartTime = 1;
					m_aMotionInfo[nCntMotion].Damage[0].nFinishTime = 0;
				}
				if (strcmp(&aFile[0], "KEYSET") == 0)
				{
					bKeySet = true;
					m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].fMove = 0.0f;
					m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].fHeight = 0.0f;
					nCntParts = 0;
				}
				if (strcmp(&aFile[0], "END_KEYSET") == 0)
				{
					bKeySet = false;
					nCntKey++;
				}
				if (strcmp(&aFile[0], "ORBITSET") == 0)
				{
					bOrbitSet = true;
					nCntObt = 0;
				}
				if (strcmp(&aFile[0], "END_ORBITSET") == 0)
				{
					bOrbitSet = false;
					m_aMotionInfo[nCntMotion].nNumOrbit = nCntObt;
				}
				if (bColSet == true) //COLLISIONSET時
				{
					if (strcmp(&aFile[0], "COLLISION") == 0)
					{
						bCol = true;
					}
					if (strcmp(&aFile[0], "END_COLLISION") == 0)
					{
						bCol = false;
						nCntCol++;
					}
					if (bCol == true)
					{
						if (strcmp(&aFile[0], "COL") == 0) //当たり判定（親のインデックス、場所、大きさ）
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%d %f %f %f %f", &m_aMotionInfo[nCntMotion].Collision[nCntCol].nIdxModelParent, &m_aMotionInfo[nCntMotion].Collision[nCntCol].pos.x,
								&m_aMotionInfo[nCntMotion].Collision[nCntCol].pos.y, &m_aMotionInfo[nCntMotion].Collision[nCntCol].pos.z, &m_aMotionInfo[nCntMotion].Collision[nCntCol].fRadius);
						}
					}
				}
				if (bOrbitSet == true)
				{
					if (strcmp(&aFile[0], "ORBIT") == 0)
					{
						bOrbit = true;
					}
					if (strcmp(&aFile[0], "END_ORBIT") == 0)
					{
						bOrbit = false;
						nCntObt++;
					}
					if (bOrbit == true)
					{
						if (strcmp(&aFile[0], "OBT") == 0) //軌跡（親のインデックス、場所、表示時間、発生時間）
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%d %f %f %f %f %d %d", &m_aMotionInfo[nCntMotion].Orbit[nCntObt].nIdxParent, &m_aMotionInfo[nCntMotion].Orbit[nCntObt].pos.x,
								&m_aMotionInfo[nCntMotion].Orbit[nCntObt].pos.y, &m_aMotionInfo[nCntMotion].Orbit[nCntObt].pos.z,
								&m_aMotionInfo[nCntMotion].Orbit[nCntObt].fDis, &m_aMotionInfo[nCntMotion].Orbit[nCntObt].nStartTime, &m_aMotionInfo[nCntMotion].Orbit[nCntObt].nFinishTime);
						}
						if (strcmp(&aFile[0], "COL1") == 0) //色先端
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%f %f %f %f", &m_aMotionInfo[nCntMotion].Orbit[nCntObt].col1.r, &m_aMotionInfo[nCntMotion].Orbit[nCntObt].col1.g,
								&m_aMotionInfo[nCntMotion].Orbit[nCntObt].col1.b, &m_aMotionInfo[nCntMotion].Orbit[nCntObt].col1.a);
						}
						if (strcmp(&aFile[0], "COL2") == 0) //色根本
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%f %f %f %f", &m_aMotionInfo[nCntMotion].Orbit[nCntObt].col2.r, &m_aMotionInfo[nCntMotion].Orbit[nCntObt].col2.g,
								&m_aMotionInfo[nCntMotion].Orbit[nCntObt].col2.b, &m_aMotionInfo[nCntMotion].Orbit[nCntObt].col2.a);
						}
					}
				}
				if (bDmgSet == true)
				{
					if (strcmp(&aFile[0], "DAMAGE") == 0)
					{
						bDmg = true;
					}
					if (strcmp(&aFile[0], "END_DAMAGE") == 0)
					{
						bDmg = false;
						nCntDmg++;
					}
					if (bDmg == true)
					{
						if (strcmp(&aFile[0], "DMG") == 0) //ダメージ（水平吹っ飛ばし力、垂直吹っ飛ばし力、ダメージ量、ヒット間隔）
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%f %f %d %d", &m_aMotionInfo[nCntMotion].Damage[nCntDmg].fHClash, &m_aMotionInfo[nCntMotion].Damage[nCntDmg].fVClash,
								&m_aMotionInfo[nCntMotion].Damage[nCntDmg].nDamage, &m_aMotionInfo[nCntMotion].Damage[nCntDmg].nDamageTime);
						}
						if (strcmp(&aFile[0], "DMG_TIME") == 0) //ダメージの時間(始まり、終わり)
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%d %d", &m_aMotionInfo[nCntMotion].Damage[nCntDmg].nStartTime, &m_aMotionInfo[nCntMotion].Damage[nCntDmg].nFinishTime);
						}
					}
				}
				if (bKeySet == true) //KEYSET時
				{
					if (strcmp(&aFile[0], "FRAME") == 0) //フレーム数
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
					}
					if (strcmp(&aFile[0], "MOVE") == 0) //移動量(横)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].fMove);
					}
					if (strcmp(&aFile[0], "HEIGHT") == 0) //移動量(縦)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].fHeight);
					}
					if (strcmp(&aFile[0], "KEY") == 0)
					{
						bKey = true;
					}
					if (strcmp(&aFile[0], "END_KEY") == 0)
					{
						bKey = false;
						nCntParts++;
					}
					if (bKey == true) //KEY時
					{
						if (strcmp(&aFile[0], "POS") == 0) //場所
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%f %f %f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].pos.x,
								&m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].pos.y, &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].pos.z);
						}
						if (strcmp(&aFile[0], "ROT") == 0) //角度
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%f %f %f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].rot.x,
								&m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].rot.y, &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].rot.z);
						}
					}
				}
			}
			if (strcmp(&aFile[0], "CHARACTERSET") == 0)
			{
				bChara = true;
				nCntParts = 0;
			}
			if (strcmp(&aFile[0], "END_CHARACTERSET") == 0)
			{
				bChara = false;
			}
			if (strcmp(&aFile[0], "MOTIONSET") == 0)
			{
				bMotion = true;
				m_aMotionInfo[nCntMotion].nNumOrbit = nCntObt;
				nCntKey = 0;
				nCntCol = 0;
				nCntObt = 0;
			}
			if (strcmp(&aFile[0], "END_MOTIONSET") == 0)
			{
				bMotion = false;
				nCntMotion++;
			}
			if (strcmp(&aFile[0], "COLLISIONSET") == 0)
			{
				bColSet = true;
				nCntCol = 0;
			}
			if (strcmp(&aFile[0], "END_COLLISIONSET") == 0)
			{
				bColSet = false;
				m_aMotionInfo[nCntMotion].nNumCol = nCntCol;
			}
			if (strcmp(&aFile[0], "DAMAGESET") == 0)
			{
				bDmgSet = true;
				nCntDmg = 0;
			}
			if (strcmp(&aFile[0], "END_DAMAGESET") == 0)
			{
				bDmgSet = false;
				m_aMotionInfo[nCntMotion].nNumDmg = nCntDmg;
			}
			if (strcmp(&aFile[0], "END_SCRIPT") == 0) //終了の1文
			{
				break;
			}
		}
		fclose(pFile);
	}
	//「MOTIONTYPE_RETURN」は、ニュートラルモーションに戻る際に経由するモーション
	m_aMotionInfo[MOTIONTYPE_RETURN].bLoop = false;
	m_aMotionInfo[MOTIONTYPE_RETURN].aKeyInfo[0].nFrame = 14;
	m_aMotionInfo[MOTIONTYPE_RETURN].nNumKey = 1;
	m_aMotionInfo[MOTIONTYPE_RETURN].bGravity = true;
	m_aMotionInfo[MOTIONTYPE_RETURN].bHoming = false;
	m_aMotionInfo[MOTIONTYPE_RETURN].nNoDmg = 0;
	m_aMotionInfo[MOTIONTYPE_RETURN].nComboTime = 0;
	m_aMotionInfo[MOTIONTYPE_RETURN].nNonOpe = 0;
	for (nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{
		m_aMotionInfo[MOTIONTYPE_RETURN].aKeyInfo[0].aKey[nCntParts] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[m_aMotionInfo[MOTIONTYPE_NEUTRAL].nNumKey - 1].aKey[nCntParts];
	}
	m_aMotionInfo[MOTIONTYPE_RETURN].Damage[0].nStartTime = 1;
	m_aMotionInfo[MOTIONTYPE_RETURN].Damage[0].nFinishTime = 0;

	//「MOTIONTYPE_AIRRETURN」は、空中ニュートラルモーションに戻る際に経由するモーション
	m_aMotionInfo[MOTIONTYPE_AIRRETURN].bLoop = false;
	m_aMotionInfo[MOTIONTYPE_AIRRETURN].aKeyInfo[0].nFrame = 20;
	m_aMotionInfo[MOTIONTYPE_AIRRETURN].nNumKey = 1;
	m_aMotionInfo[MOTIONTYPE_AIRRETURN].bGravity = true;
	m_aMotionInfo[MOTIONTYPE_AIRRETURN].bHoming = false;
	m_aMotionInfo[MOTIONTYPE_AIRRETURN].nNoDmg = 0;
	m_aMotionInfo[MOTIONTYPE_AIRRETURN].nComboTime = 0;
	m_aMotionInfo[MOTIONTYPE_AIRRETURN].nNonOpe = 0;
	for (nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{
		m_aMotionInfo[MOTIONTYPE_AIRRETURN].aKeyInfo[0].aKey[nCntParts] = m_aMotionInfo[MOTIONTYPE_AIRNEUTRAL].aKeyInfo[m_aMotionInfo[MOTIONTYPE_NEUTRAL].nNumKey - 1].aKey[nCntParts];
	}
	m_aMotionInfo[MOTIONTYPE_AIRRETURN].Damage[0].nStartTime = 1;
	m_aMotionInfo[MOTIONTYPE_AIRRETURN].Damage[0].nFinishTime = 0;
}

bool CPlayer::Move(float fRotY)
{
	float fCameraRotY = m_pCamera->GetRotY();
	m_move.x += (sinf(fCameraRotY + D3DX_PI * fRotY) * m_fMove - m_move.x) * 0.1f;
	m_move.z += (cosf(fCameraRotY + D3DX_PI * fRotY) * m_fMove - m_move.z) * 0.1f;
	m_rotDesh.y = fCameraRotY + D3DX_PI * fRotY + D3DX_PI;
	if (m_rotDesh.y >= D3DX_PI)
	{
		m_rotDesh.y -= D3DX_PI * 2;
	}
	if (m_rotDesh.y < -D3DX_PI)
	{
		m_rotDesh.y += D3DX_PI * 2;
	}
	if (m_bRockon == false)
	{
		if (0.0f < fRotY && fRotY < 1.0f)
		{
			m_pCamera->AddRotY(0.01f);
		}
		else if (-1.0f < fRotY && fRotY < 0.0f)
		{
			m_pCamera->AddRotY(-0.01f);
		}
	}
	if (m_nRunStop > 0)
	{
		m_nRunStop = 0;
	}
	return true;
}

void CPlayer::MotionMove(float fRotY, float fMove)
{
	m_MotionMove.x -= (sinf(fRotY) * fMove + m_MotionMove.x) * 0.1f;
	m_MotionMove.z -= (cosf(fRotY) * fMove + m_MotionMove.z) * 0.1f;
	//m_MotionMove.x = -sinf(fRotY) * fMove;
	//m_MotionMove.z = -cosf(fRotY) * fMove;
}

void CPlayer::MotionStopMove()
{
	m_MotionMove.x -= m_MotionMove.x * 0.2f;
	m_MotionMove.z -= m_MotionMove.z * 0.2f;
	//m_MotionMove.x = 0.0f;
	//m_MotionMove.z = 0.0f;
}

void CPlayer::MotionHeight(float fHeight)
{
	m_MotionMove.y += (fHeight - m_MotionMove.y) * 0.1f;
	//m_MotionMove.y = fHeight;
}

void CPlayer::MotionStopHeight()
{
	m_MotionMove.y -= m_MotionMove.y * 0.2f;
	//m_MotionMove.y = 0.0f;
}

bool CPlayer::Stop()
{
	m_move.x -= m_move.x * 0.2f;
	m_move.z -= m_move.z * 0.2f;
	return false;
}

void CPlayer::Jump()
{
	if (m_bJump == true)
	{
		m_nCombo = 0;
		m_move.y = m_fJump;
		m_bJump = false;
		m_bLanding = false;
		m_bLandObject = false;
		MotionChange(MOTIONTYPE_JUMP);
	}
}

void CPlayer::CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld)
{
	CScene *pScene;
	pScene = GetScene(PRIORITY_OBJECT);
	bool bLand = false;
	while (pScene)
	{
		CScene *pSceneNext;
		pSceneNext = pScene->GetNext();
		if (pScene->GetObjType() == CScene::OBJECTTYPE_BLOCK)
		{
			if (pScene->GetCollision() == CScene::COLLISION_SPHERE)
			{
				D3DXVECTOR3 pos = pScene->GetPos();
				float fRadius = pScene->GetRadius();
				float fRotY;
				float fLengthX = pPos->x - pos.x;
				float fLengthY = (pPos->y + (m_fHeight / 2)) - pos.y;
				float fLengthZ = pPos->z - pos.z;
				fLengthX = powf(fLengthX, 2);
				fLengthY = powf(fLengthY, 2);
				fLengthZ = powf(fLengthZ, 2);
				if (fLengthX + fLengthY + fLengthZ <= powf((m_fHeight / 2) + fRadius, 2))
				{
					fRotY = atan2f(pos.x - pPos->x, pos.z - pPos->z);
					pPos->x = pos.x - sinf(fRotY) * ((m_fHeight / 2) + fRadius);
					pPos->z = pos.z - cosf(fRotY) * ((m_fHeight / 2) + fRadius);
				}
			}
			if (pScene->GetCollision() == CScene::COLLISION_SQUARE)
			{
				D3DXVECTOR3 pos = pScene->GetPos();
				D3DXVECTOR3 VtxMin = pScene->GetVtxMin();
				D3DXVECTOR3 VtxMax = pScene->GetVtxMax();
				float fPRadius = GetRadius();
				if (pos.x + VtxMin.x <= pPos->x + fPRadius && pPos->x - fPRadius <= pos.x + VtxMax.x &&
					pos.z + VtxMin.z <= pPos->z + fPRadius && pPos->z - fPRadius <= pos.z + VtxMax.z)
				{
					if (posOld.y >= pos.y + VtxMax.y && pPos->y <= pos.y + VtxMax.y)
					{
						pPos->y = pos.y + VtxMax.y;
						m_move.y = 0.0f;
						bLand = true;
						m_pShadow->MoveY(*pPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

						if (m_bLanding == false)
						{
							CSound::Play(CSound::SOUND_LABEL_LANDING);
							MotionChange(MOTIONTYPE_LANDING);
							m_bLanding = true;
						}
					}
					else if (posOld.y + m_fHeight <= pos.y + VtxMin.y && pPos->y + m_fHeight >= pos.y + VtxMin.y)
					{
						pPos->y = pos.y + VtxMin.y - m_fHeight;
						m_move.y = 0.0f;
					}
					else if (pos.y + VtxMin.y <= pPos->y + m_fHeight && pPos->y <= pos.y + VtxMax.y)
					{
						bool bIn = false;
						if (pos.x + VtxMin.x <= pPos->x + fPRadius && posOld.x + fPRadius <= pos.x + VtxMin.x)
						{
							pPos->x = pos.x + VtxMin.x - fPRadius;
							bIn = true;
						}
						else if (pos.x + VtxMax.x >= pPos->x - fPRadius && posOld.x - fPRadius >= pos.x + VtxMax.x)
						{
							pPos->x = pos.x + VtxMax.x + fPRadius + 0.1f;
							bIn = true;
						}
						if (pos.z + VtxMin.z <= pPos->z + fPRadius && posOld.z + fPRadius <= pos.z + VtxMin.z)
						{
							pPos->z = pos.z + VtxMin.z - fPRadius;
							bIn = true;
						}
						else if (pos.z + VtxMax.z >= pPos->z - fPRadius && posOld.z - fPRadius >= pos.z + VtxMax.z)
						{
							pPos->z = pos.z + VtxMax.z + fPRadius + 0.1f;
							bIn = true;
						}
						if (bIn == false)
						{
							float fMinDistance = 100000.0f;
							float fDistance[4];
							fDistance[0] = pos.x + VtxMax.x - pPos->x;
							fDistance[1] = pos.x + VtxMin.x - pPos->x;
							fDistance[2] = pos.z + VtxMax.z - pPos->z;
							fDistance[3] = pos.z + VtxMin.z - pPos->z;
							int nCnt;
							int nDistance;
							for (nCnt = 0; nCnt < 4; nCnt++)
							{
								if (fabsf(fMinDistance) > fabsf(fDistance[nCnt]))
								{
									fMinDistance = fDistance[nCnt];
									nDistance = nCnt;
								}
							}
							switch (nDistance)
							{
							case 0:
								pPos->x = pos.x + VtxMax.x + fPRadius + 0.1f;
								break;
							case 1:
								pPos->x = pos.x + VtxMin.x - fPRadius;
								break;
							case 2:
								pPos->z = pos.z + VtxMax.z + fPRadius + 0.1f;
								break;
							case 3:
								pPos->z = pos.z + VtxMin.z - fPRadius;
								break;
							default:
								break;
							}
						}
					}
				}
			}
		}
		pScene = pSceneNext;
	}
	m_bLandObject = bLand;
}

void CPlayer::CollisionPos(int nCntCol)
{
	D3DXMATRIX mtxTrans; //パーツ用計算用マトリックス
	D3DXMATRIX mtxParent; //親のマトリックス
	//当たり判定のワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MotionInfo.Collision[nCntCol].mtxWorld);
	//当たり判定の位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_MotionInfo.Collision[nCntCol].pos.x, m_MotionInfo.Collision[nCntCol].pos.y, m_MotionInfo.Collision[nCntCol].pos.z);
	D3DXMatrixMultiply(&m_MotionInfo.Collision[nCntCol].mtxWorld, &m_MotionInfo.Collision[nCntCol].mtxWorld, &mtxTrans);
	//親のマトリックス取得
	mtxParent = m_pModel[m_MotionInfo.Collision[nCntCol].nIdxModelParent]->GetMatrix();
	//算出した各当たり判定のワールドマトリックスと親のマトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_MotionInfo.Collision[nCntCol].mtxWorld,
		&m_MotionInfo.Collision[nCntCol].mtxWorld,
		&mtxParent);
}

bool CPlayer::CollisionDamage(D3DXVECTOR3 pos, float fRadius, float fHeight)
{
	if (m_bAttack == true)
	{
		int nCntCol;
		float fLengthX;		//当たり判定と敵のX方向の長さ
		float fLengthY;		//当たり判定と敵のY方向の長さ
		float fLengthZ;		//当たり判定と敵のZ方向の長さ
		float fHDistance;	//当たり判定と敵の水平方向の距離
		for (nCntCol = 0; nCntCol < m_MotionInfo.nNumCol; nCntCol++)
		{
			CollisionPos(nCntCol);
			fLengthX = powf(pos.x - m_MotionInfo.Collision[nCntCol].mtxWorld._41, 2);
			fLengthY = pos.y + (fHeight / 2) - m_MotionInfo.Collision[nCntCol].mtxWorld._42;
			fLengthZ = powf(pos.z - m_MotionInfo.Collision[nCntCol].mtxWorld._43, 2);
			fHDistance = sqrtf(fLengthX + fLengthZ);
			if (fHDistance <= fRadius + m_MotionInfo.Collision[nCntCol].fRadius && fabsf(fLengthY) <= fHeight + m_MotionInfo.Collision[nCntCol].fRadius)
			{
				/*CParticle::Create(D3DXVECTOR3(m_MotionInfo.Collision[nCntCol].mtxWorld._41, m_MotionInfo.Collision[nCntCol].mtxWorld._42, m_MotionInfo.Collision[nCntCol].mtxWorld._43),
					D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 40, -1, D3DXVECTOR3(4.0f, 0.0f, 0.0f));*/
				m_nCntCol = nCntCol;
				if (m_bHit == false)
				{
					m_bHit = true;
				}
				return true;
			}
		}
	}
	return false;
}

void CPlayer::CollisionEnemy(D3DXVECTOR3 *pPos)
{
	CScene *pScene;
	pScene = GetScene(PRIORITY_CHARA);
	while (pScene)
	{
		CScene *pSceneNext;
		pSceneNext = pScene->GetNext();
		if (pScene->GetObjType() == CScene::OBJECTTYPE_ENEMY)
		{
			D3DXVECTOR3 pos = pScene->GetPos();
			float fRadius = pScene->GetRadius() * 0.8f;
			float fHeight = pScene->GetHeight();
			float fRotY;
			float fLengthX = pPos->x - pos.x;
			//float fLengthY = (pPos->y + (m_fHeight / 2)) - (pos.y + (fHeight / 2));
			float fLengthZ = pPos->z - pos.z;
			fLengthX = powf(fLengthX, 2);
			//fLengthY = powf(fLengthY, 2);
			fLengthZ = powf(fLengthZ, 2);
			if (fLengthX + fLengthZ <= powf(m_fRadius * 0.8f + fRadius, 2.0f) && pos.y < pPos->y + m_fHeight && pPos->y < pos.y + fHeight)
			{
				fRotY = atan2f(pos.x - pPos->x, pos.z - pPos->z);
				pPos->x = pos.x - sinf(fRotY) * (m_fRadius * 0.8f + fRadius);
				pPos->z = pos.z - cosf(fRotY) * (m_fRadius * 0.8f + fRadius);
			}
		}
		pScene = pSceneNext;
	}
}

void CPlayer::Orbit()
{
	int nCntObt;
	for (nCntObt = 0; nCntObt < m_MotionInfo.nNumOrbit; nCntObt++)
	{
		if (m_MotionInfo.Orbit[nCntObt].nStartTime <= m_nTotalTime && m_nTotalTime < m_MotionInfo.Orbit[nCntObt].nFinishTime)
		{
			D3DXMATRIX mtxTrans; //パーツ用計算用マトリックス
			D3DXMATRIX mtxParent; //親のマトリックス
			//当たり判定のワールドマトリックスの初期化
			D3DXMatrixIdentity(&m_MotionInfo.Orbit[nCntObt].TipmtxWorld);
			//当たり判定の位置を反映
			D3DXMatrixTranslation(&mtxTrans, m_MotionInfo.Orbit[nCntObt].pos.x, m_MotionInfo.Orbit[nCntObt].pos.y, m_MotionInfo.Orbit[nCntObt].pos.z);
			D3DXMatrixMultiply(&m_MotionInfo.Orbit[nCntObt].TipmtxWorld, &m_MotionInfo.Orbit[nCntObt].TipmtxWorld, &mtxTrans);
			//親のマトリックス取得
			mtxParent = m_pModel[m_MotionInfo.Orbit[nCntObt].nIdxParent]->GetMatrix();
			//算出した各軌跡の先端のワールドマトリックスと親のマトリックスを掛け合わせる
			D3DXMatrixMultiply(&m_MotionInfo.Orbit[nCntObt].TipmtxWorld,
				&m_MotionInfo.Orbit[nCntObt].TipmtxWorld,
				&mtxParent);
			m_pOrbit[nCntObt]->SetOrbit(D3DXVECTOR3(mtxParent._41, mtxParent._42, mtxParent._43),
				D3DXVECTOR3(m_MotionInfo.Orbit[nCntObt].TipmtxWorld._41, m_MotionInfo.Orbit[nCntObt].TipmtxWorld._42, m_MotionInfo.Orbit[nCntObt].TipmtxWorld._43),
				m_MotionInfo.Orbit[nCntObt].fDis, m_MotionInfo.Orbit[nCntObt].col1, m_MotionInfo.Orbit[nCntObt].col2);
		}
	}
}

void CPlayer::Motion() //モーションの動作
{
	int nCntParts;
	m_nTotalTime++;
	m_nCntMotion++;
	if (m_MotionInfo.Damage[m_nDmgNumNow].nStartTime <= m_nTotalTime && m_nTotalTime < m_MotionInfo.Damage[m_nDmgNumNow].nFinishTime)
	{
		m_nCntDamage++;
		if (m_bAttack == false)
		{
			m_bAttack = true;
		}
	}
	else if (m_bAttack == true)
	{
		m_bAttack = false;
	}
	for (nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{
		D3DXVECTOR3 posOld;
		posOld = m_pModel[nCntParts]->GetPos();
		D3DXVECTOR3 rotOld;
		rotOld = m_pModel[nCntParts]->GetRot();
		if (m_nCntMotion == 1) //差分を求める(対応するキーの各パーツのpos(rot) - 今の各パーツのpos(rot))
		{
			m_aKeyDiff[nCntParts].pos.x = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].pos.x - posOld.x;
			m_aKeyDiff[nCntParts].pos.y = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].pos.y - posOld.y;
			m_aKeyDiff[nCntParts].pos.z = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].pos.z - posOld.z;
			m_aKeyDiff[nCntParts].rot.x = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].rot.x - rotOld.x;
			if (m_aKeyDiff[nCntParts].rot.x < -D3DX_PI)
			{
				m_aKeyDiff[nCntParts].rot.x += D3DX_PI * 2;
			}
			else if (m_aKeyDiff[nCntParts].rot.x > D3DX_PI)
			{
				m_aKeyDiff[nCntParts].rot.x -= D3DX_PI * 2;
			}
			m_aKeyDiff[nCntParts].rot.y = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].rot.y - rotOld.y;
			if (m_aKeyDiff[nCntParts].rot.y < -D3DX_PI)
			{
				m_aKeyDiff[nCntParts].rot.y += D3DX_PI * 2;
			}
			else if (m_aKeyDiff[nCntParts].rot.y > D3DX_PI)
			{
				m_aKeyDiff[nCntParts].rot.y -= D3DX_PI * 2;
			}
			m_aKeyDiff[nCntParts].rot.z = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].rot.z - rotOld.z;
			if (m_aKeyDiff[nCntParts].rot.z < -D3DX_PI)
			{
				m_aKeyDiff[nCntParts].rot.z += D3DX_PI * 2;
			}
			else if (m_aKeyDiff[nCntParts].rot.z > D3DX_PI)
			{
				m_aKeyDiff[nCntParts].rot.z -= D3DX_PI * 2;
			}
		}
		posOld.x += m_aKeyDiff[nCntParts].pos.x / m_MotionInfo.aKeyInfo[m_nKey].nFrame;
		posOld.y += m_aKeyDiff[nCntParts].pos.y / m_MotionInfo.aKeyInfo[m_nKey].nFrame;
		posOld.z += m_aKeyDiff[nCntParts].pos.z / m_MotionInfo.aKeyInfo[m_nKey].nFrame;
		m_pModel[nCntParts]->SetPos(posOld);

		rotOld.x += m_aKeyDiff[nCntParts].rot.x / m_MotionInfo.aKeyInfo[m_nKey].nFrame;
		rotOld.y += m_aKeyDiff[nCntParts].rot.y / m_MotionInfo.aKeyInfo[m_nKey].nFrame;
		rotOld.z += m_aKeyDiff[nCntParts].rot.z / m_MotionInfo.aKeyInfo[m_nKey].nFrame;
		m_pModel[nCntParts]->SetRot(rotOld);
	}
	if (m_nCntMotion == m_MotionInfo.aKeyInfo[m_nKey].nFrame)
	{
		m_nCntMotion = 0;
		m_nKey++;
		if (m_nKey == m_MotionInfo.nNumKey)
		{
			if (m_MotionInfo.bLoop == false)
			{
				if (m_bJump == true)
				{
					if (m_motionType != MOTIONTYPE_RETURN)
					{
						MotionChange(MOTIONTYPE_RETURN);
					}
					else
					{
						MotionChange(MOTIONTYPE_NEUTRAL);
						m_nCombo = 0;
					}
				}
				else
				{
					if (m_motionType != MOTIONTYPE_AIRRETURN)
					{
						MotionChange(MOTIONTYPE_AIRRETURN);
					}
					else
					{
						MotionChange(MOTIONTYPE_AIRNEUTRAL);
						m_nCombo = 0;
					}
				}
			}
			m_nKey = 0;
		}
	}
	if (m_nCntDamage > m_MotionInfo.Damage[m_nDmgNumNow].nDamageTime)
	{
		ResetEnemy();
		m_nCntDamage = 0;
	}
	if (m_nTotalTime >= m_MotionInfo.Damage[m_nDmgNumNow].nFinishTime)
	{
		ResetEnemy();
		m_nCntDamage = 0;
		if (m_nDmgNumNow < 7)
		{
			m_nDmgNumNow++;
		}
		else
		{
			m_nDmgNumNow = 0;
		}
	}
}

void CPlayer::HasteMotion() //モーションの動作
{
	int nCntParts;
	m_nTotalTime += 2;
	m_nCntMotion += 2;
	for (nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{
		if (m_nCntMotion == 2) //差分を求める(対応するキーの各パーツのpos(rot) - 今の各パーツのpos(rot))
		{
			m_aKeyDiff[nCntParts].pos.x = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].pos.x - m_pModel[nCntParts]->GetPos().x;
			m_aKeyDiff[nCntParts].pos.y = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].pos.y - m_pModel[nCntParts]->GetPos().y;
			m_aKeyDiff[nCntParts].pos.z = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].pos.z - m_pModel[nCntParts]->GetPos().z;
			m_aKeyDiff[nCntParts].rot.x = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].rot.x - m_pModel[nCntParts]->GetRot().x;
			if (m_aKeyDiff[nCntParts].rot.x < -D3DX_PI)
			{
				m_aKeyDiff[nCntParts].rot.x += D3DX_PI * 2;
			}
			else if (m_aKeyDiff[nCntParts].rot.x > D3DX_PI)
			{
				m_aKeyDiff[nCntParts].rot.x -= D3DX_PI * 2;
			}
			m_aKeyDiff[nCntParts].rot.y = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].rot.y - m_pModel[nCntParts]->GetRot().y;
			if (m_aKeyDiff[nCntParts].rot.y < -D3DX_PI)
			{
				m_aKeyDiff[nCntParts].rot.y += D3DX_PI * 2;
			}
			else if (m_aKeyDiff[nCntParts].rot.y > D3DX_PI)
			{
				m_aKeyDiff[nCntParts].rot.y -= D3DX_PI * 2;
			}
			m_aKeyDiff[nCntParts].rot.z = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].rot.z - m_pModel[nCntParts]->GetRot().z;
			if (m_aKeyDiff[nCntParts].rot.z < -D3DX_PI)
			{
				m_aKeyDiff[nCntParts].rot.z += D3DX_PI * 2;
			}
			else if (m_aKeyDiff[nCntParts].rot.z > D3DX_PI)
			{
				m_aKeyDiff[nCntParts].rot.z -= D3DX_PI * 2;
			}
		}
		D3DXVECTOR3 posOld;
		posOld = m_pModel[nCntParts]->GetPos();
		posOld.x += m_aKeyDiff[nCntParts].pos.x / (m_MotionInfo.aKeyInfo[m_nKey].nFrame / 2);
		posOld.y += m_aKeyDiff[nCntParts].pos.y / (m_MotionInfo.aKeyInfo[m_nKey].nFrame / 2);
		posOld.z += m_aKeyDiff[nCntParts].pos.z / (m_MotionInfo.aKeyInfo[m_nKey].nFrame / 2);
		m_pModel[nCntParts]->SetPos(posOld);

		D3DXVECTOR3 rotOld;
		rotOld = m_pModel[nCntParts]->GetRot();
		rotOld.x += m_aKeyDiff[nCntParts].rot.x / (m_MotionInfo.aKeyInfo[m_nKey].nFrame / 2);
		rotOld.y += m_aKeyDiff[nCntParts].rot.y / (m_MotionInfo.aKeyInfo[m_nKey].nFrame / 2);
		rotOld.z += m_aKeyDiff[nCntParts].rot.z / (m_MotionInfo.aKeyInfo[m_nKey].nFrame / 2);
		m_pModel[nCntParts]->SetRot(rotOld);
	}
	if (m_nCntMotion >= m_MotionInfo.aKeyInfo[m_nKey].nFrame)
	{
		m_nCntMotion = 0;
		m_nKey++;
		if (m_nKey == m_MotionInfo.nNumKey)
		{
			if (m_MotionInfo.bLoop == false)
			{
				if (m_motionType != MOTIONTYPE_RETURN)
				{
					MotionChange(MOTIONTYPE_RETURN);
				}
				else
				{
					MotionChange(MOTIONTYPE_NEUTRAL);
				}
			}
			m_nKey = 0;
		}
	}
}

void CPlayer::MotionChange(MOTIONTYPE motionType)
{
	m_motionTypeOld = m_motionType;
	m_motionType = motionType;
	m_MotionInfo = m_aMotionInfo[motionType];
	m_nCntMotion = 0;
	m_nTotalTime = 0;
	m_nKey = 0;
	ResetEnemy();
	m_nCntDamage = 0;
	m_nDmgNumNow = 0;
	if (motionType != MOTIONTYPE_RETURN && motionType != MOTIONTYPE_AIRRETURN)
	{
		m_nComboTime = m_MotionInfo.nComboTime;
		m_nNonOpe = m_MotionInfo.nNonOpe;
	}
	m_nCntNoDmg = m_MotionInfo.nNoDmg;
	m_bHit = false;
	if (m_bCounter == true)
	{
		m_bCounter = false;
		m_pCounterUI->ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}
}

bool CPlayer::CollisionLand(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, CScene *pScene)
{
	D3DXVECTOR3 pos = pScene->GetPos();
	D3DXVECTOR3 VtxMax = pScene->GetVtxMax();
	D3DXVECTOR3 VtxMin = pScene->GetVtxMin();
	D3DXVECTOR3 aPoint[4];
	D3DXVECTOR3 rot = pScene->GetRot();
	aPoint[0] = D3DXVECTOR3(pos.x + VtxMin.x * cosf(-rot.y), pos.y + VtxMax.y, pos.z + VtxMax.z * cosf(-rot.y));
	aPoint[1] = D3DXVECTOR3(pos.x + VtxMax.x * cosf(-rot.y), pos.y + VtxMin.y, pos.z + VtxMax.z * cosf(-rot.y));
	aPoint[2] = D3DXVECTOR3(pos.x + VtxMin.x * cosf(-rot.y), pos.y + VtxMax.y, pos.z + VtxMin.z * cosf(-rot.y));
	aPoint[3] = D3DXVECTOR3(pos.x + VtxMax.x * cosf(-rot.y), pos.y + VtxMin.y, pos.z + VtxMin.z * cosf(-rot.y));
	D3DXVECTOR3 Cross[3];
	D3DXVec3Cross(&Cross[0], &(aPoint[2] - aPoint[1]), &(aPoint[1] - aPoint[0]));
	float fLength = powf((Cross[0].x * Cross[0].x) + (Cross[0].y * Cross[0].y) + (Cross[0].z * Cross[0].z), 0.5f);
	Cross[0].x /= fLength;
	Cross[0].y /= fLength;
	Cross[0].z /= fLength; //ここまで法線ベクトル

	Cross[1] = *pPos - aPoint[0]; //プレイヤーのposと壁の任意の点のベクトル
	Cross[2] = posOld - aPoint[0]; //プレイヤーのOldposと壁の任意の点のベクトル

	float fDistance = Cross[0].x * Cross[1].x + Cross[0].y * Cross[1].y + Cross[0].z * Cross[1].z; //面との距離 マイナスの時、表側にいる
	float fOldDistance = Cross[0].x * Cross[2].x + Cross[0].y * Cross[2].y + Cross[0].z * Cross[2].z; //面との距離　マイナスの時、表側にいる
	D3DXVECTOR3 Point = D3DXVECTOR3(pPos->x - (Cross[0].x * fDistance), pPos->y - (Cross[0].y * fDistance), pPos->z - (Cross[0].z * fDistance)); //壁面上のプレイヤーの位置
	D3DXVECTOR3 OldPoint = D3DXVECTOR3(posOld.x - (Cross[0].x * fOldDistance), posOld.y - (Cross[0].y * fOldDistance), posOld.z - (Cross[0].z * fOldDistance)); //壁面上の1フレーム前のプレイヤーの位置
	if ((aPoint[0].x < Point.x + m_fRadius && Point.x - m_fRadius < aPoint[1].x || aPoint[1].x < Point.x + m_fRadius && Point.x - m_fRadius < aPoint[0].x) ||
		(aPoint[0].x < OldPoint.x + m_fRadius && OldPoint.x - m_fRadius < aPoint[1].x || aPoint[1].x < OldPoint.x + m_fRadius && OldPoint.x - m_fRadius < aPoint[0].x))
	{
		if ((aPoint[0].z < Point.z + m_fRadius && Point.z - m_fRadius < aPoint[2].z || aPoint[2].z < Point.z + m_fRadius && Point.z - m_fRadius < aPoint[0].z) ||
			(aPoint[0].z < OldPoint.z + m_fRadius && OldPoint.z - m_fRadius < aPoint[2].z || aPoint[2].z < OldPoint.z + m_fRadius && OldPoint.z - m_fRadius < aPoint[0].z))
		{
			if (fOldDistance - 3.0f <= 0.0f && fDistance + 3.0f >= 0.0f)
			{
				pPos->y += fDistance;
				m_pShadow->MoveY(*pPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				m_move.y = 0.0f;
				if (m_bLanding == false && m_motionType != MOTIONTYPE_DIVE)
				{
					CSound::Play(CSound::SOUND_LABEL_LANDING);
					MotionChange(MOTIONTYPE_LANDING);
				}
				return true;
			}
		}
	}
	return false;
}

void CPlayer::CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, CScene *pScene)
{
	D3DXVECTOR3 rot = pScene->GetRot();
	D3DXVECTOR3 aPoint[4]; //壁の4頂点
	D3DXVECTOR3 VtxMax = pScene->GetVtxMax();
	D3DXVECTOR3 VtxMin = pScene->GetVtxMin();
	D3DXVECTOR3 pos = pScene->GetPos();
	aPoint[0] = D3DXVECTOR3(pos.x + VtxMin.x * cosf(-rot.y), pos.y + VtxMax.y, pos.z + VtxMin.x * sinf(-rot.y));
	aPoint[1] = D3DXVECTOR3(pos.x + VtxMax.x * cosf(-rot.y), pos.y + VtxMax.y, pos.z + VtxMax.x * sinf(-rot.y));
	aPoint[2] = D3DXVECTOR3(pos.x + VtxMin.x * cosf(-rot.y), pos.y + VtxMin.y, pos.z + VtxMin.x * sinf(-rot.y));
	aPoint[3] = D3DXVECTOR3(pos.x + VtxMax.x * cosf(-rot.y), pos.y + VtxMin.y, pos.z + VtxMax.x * sinf(-rot.y));

	D3DXVECTOR3 Cross[3];
	D3DXVec3Cross(&Cross[0], &(aPoint[2] - aPoint[1]), &(aPoint[1] - aPoint[0]));
	float fLength = powf((Cross[0].x * Cross[0].x) + (Cross[0].y * Cross[0].y) + (Cross[0].z * Cross[0].z), 0.5f);
	Cross[0].x /= fLength;
	Cross[0].y /= fLength;
	Cross[0].z /= fLength; //ここまで法線ベクトル
			
	Cross[1] = *pPos - aPoint[0]; //プレイヤーのposと壁の任意の点のベクトル
	Cross[2] = posOld - aPoint[0]; //プレイヤーのOldposと壁の任意の点のベクトル

	float fDistance = Cross[0].x * Cross[1].x + Cross[0].y * Cross[1].y + Cross[0].z * Cross[1].z; //壁面との距離 マイナスの時、表側にいる
	float fOldDistance = Cross[0].x * Cross[2].x + Cross[0].y * Cross[2].y + Cross[0].z * Cross[2].z; //壁面との距離　マイナスの時、表側にいる
	D3DXVECTOR3 Point = D3DXVECTOR3(pPos->x - (Cross[0].x * fDistance), pPos->y - (Cross[0].y * fDistance), pPos->z - (Cross[0].z * fDistance)); //壁面上のプレイヤーの位置
	D3DXVECTOR3 OldPoint = D3DXVECTOR3(posOld.x - (Cross[0].x * fOldDistance), posOld.y - (Cross[0].y * fOldDistance), posOld.z - (Cross[0].z * fOldDistance)); //壁面上の1フレーム前のプレイヤーの位置

	/*D3DXVECTOR3 SubVector[12];
	SubVector[0] = D3DXVECTOR3(aPoint[1].x - aPoint[0].x, aPoint[1].y - aPoint[0].y, aPoint[1].z - aPoint[0].z);
	SubVector[1] = D3DXVECTOR3(aPoint[0].x - Point.x, aPoint[0].y - Point.y, aPoint[0].z - Point.z);
	SubVector[2] = D3DXVECTOR3(aPoint[0].x - OldPoint.x, aPoint[0].y - OldPoint.y, aPoint[0].z - OldPoint.z);
	SubVector[3] = D3DXVECTOR3(aPoint[3].x - aPoint[1].x, aPoint[3].y - aPoint[1].y, aPoint[3].z - aPoint[1].z);
	SubVector[4] = D3DXVECTOR3(aPoint[1].x - Point.x, aPoint[1].y - Point.y, aPoint[1].z - Point.z);
	SubVector[5] = D3DXVECTOR3(aPoint[1].x - OldPoint.x, aPoint[1].y - OldPoint.y, aPoint[1].z - OldPoint.z);
	SubVector[6] = D3DXVECTOR3(aPoint[2].x - aPoint[3].x, aPoint[2].y - aPoint[3].y, aPoint[2].z - aPoint[3].z);
	SubVector[7] = D3DXVECTOR3(aPoint[3].x - Point.x, aPoint[3].y - Point.y, aPoint[3].z - Point.z);
	SubVector[8] = D3DXVECTOR3(aPoint[3].x - OldPoint.x, aPoint[3].y - OldPoint.y, aPoint[3].z - OldPoint.z);
	SubVector[9] = D3DXVECTOR3(aPoint[0].x - aPoint[2].x, aPoint[0].y - aPoint[2].y, aPoint[0].z - aPoint[2].z);
	SubVector[10] = D3DXVECTOR3(aPoint[2].x - Point.x, aPoint[2].y - Point.y, aPoint[2].z - Point.z);
	SubVector[11] = D3DXVECTOR3(aPoint[2].x - OldPoint.x, aPoint[2].y - OldPoint.y, aPoint[2].z - OldPoint.z);

	D3DXVECTOR3 CrossVector[8];
	CrossVector[0] = D3DXVECTOR3(SubVector[0].y * SubVector[1].z - SubVector[1].y * SubVector[0].z, SubVector[0].z * SubVector[1].x - SubVector[1].z * SubVector[0].x, SubVector[0].x * SubVector[1].y - SubVector[1].x * SubVector[0].y);
	CrossVector[1] = D3DXVECTOR3(SubVector[0].y * SubVector[2].z - SubVector[2].y * SubVector[0].z, SubVector[0].z * SubVector[2].x - SubVector[2].z * SubVector[0].x, SubVector[0].x * SubVector[2].y - SubVector[2].x * SubVector[0].y);
	CrossVector[2] = D3DXVECTOR3(SubVector[3].y * SubVector[4].z - SubVector[4].y * SubVector[3].z, SubVector[3].z * SubVector[4].x - SubVector[4].z * SubVector[3].x, SubVector[3].x * SubVector[4].y - SubVector[4].x * SubVector[3].y);
	CrossVector[3] = D3DXVECTOR3(SubVector[3].y * SubVector[5].z - SubVector[5].y * SubVector[3].z, SubVector[3].z * SubVector[5].x - SubVector[5].z * SubVector[3].x, SubVector[3].x * SubVector[5].y - SubVector[5].x * SubVector[3].y);
	CrossVector[4] = D3DXVECTOR3(SubVector[6].y * SubVector[7].z - SubVector[7].y * SubVector[6].z, SubVector[6].z * SubVector[7].x - SubVector[7].z * SubVector[6].x, SubVector[6].x * SubVector[7].y - SubVector[7].x * SubVector[6].y);
	CrossVector[5] = D3DXVECTOR3(SubVector[6].y * SubVector[8].z - SubVector[8].y * SubVector[6].z, SubVector[6].z * SubVector[8].x - SubVector[8].z * SubVector[6].x, SubVector[6].x * SubVector[8].y - SubVector[8].x * SubVector[6].y);
	CrossVector[6] = D3DXVECTOR3(SubVector[9].y * SubVector[10].z - SubVector[10].y * SubVector[9].z, SubVector[9].z * SubVector[10].x - SubVector[10].z * SubVector[9].x, SubVector[9].x * SubVector[10].y - SubVector[10].x * SubVector[9].y);
	CrossVector[7] = D3DXVECTOR3(SubVector[9].y * SubVector[11].z - SubVector[11].y * SubVector[9].z, SubVector[9].z * SubVector[11].x - SubVector[11].z * SubVector[9].x, SubVector[9].x * SubVector[11].y - SubVector[11].x * SubVector[9].y);
	
	float fDot[8];
	fDot[0] = CrossVector[0].x * CrossVector[2].x + CrossVector[0].y * CrossVector[2].y + CrossVector[0].z * CrossVector[2].z;
	fDot[1] = CrossVector[2].x * CrossVector[4].x + CrossVector[2].y * CrossVector[4].y + CrossVector[2].z * CrossVector[4].z;
	fDot[2] = CrossVector[4].x * CrossVector[6].x + CrossVector[4].y * CrossVector[6].y + CrossVector[4].z * CrossVector[6].z;
	fDot[3] = CrossVector[6].x * CrossVector[0].x + CrossVector[6].y * CrossVector[0].y + CrossVector[6].z * CrossVector[0].z;
	fDot[4] = CrossVector[0].x * CrossVector[1].x + CrossVector[0].y * CrossVector[1].y + CrossVector[0].z * CrossVector[1].z;
	fDot[5] = CrossVector[2].x * CrossVector[3].x + CrossVector[2].y * CrossVector[3].y + CrossVector[2].z * CrossVector[3].z;
	fDot[6] = CrossVector[4].x * CrossVector[5].x + CrossVector[4].y * CrossVector[5].y + CrossVector[4].z * CrossVector[5].z;
	fDot[7] = CrossVector[6].x * CrossVector[7].x + CrossVector[6].y * CrossVector[7].y + CrossVector[6].z * CrossVector[7].z;
	if (fDot[0] > 0.0f && fDot[1] > 0.0f && fDot[2] > 0.0f && fDot[3] > 0.0f || fDot[4] > 0.0f && fDot[5] > 0.0f && fDot[6] > 0.0f && fDot[7] > 0.0f)
	{
		if (fabsf(fDistance) <= m_fRadius || fOldDistance <= -m_fRadius && fDistance >= -m_fRadius)
		{
			pPos->x -= sinf(rot.y) * (m_fRadius + fDistance);
			pPos->z -= cosf(rot.y) * (m_fRadius + fDistance);
		}
	}
	*/

	if (aPoint[2].y < Point.y + m_fHeight && Point.y < aPoint[0].y || aPoint[2].y < OldPoint.y + m_fHeight && OldPoint.y < aPoint[0].y)
	{
		if ((aPoint[0].x < Point.x + m_fRadius && Point.x - m_fRadius < aPoint[1].x || aPoint[1].x < Point.x + m_fRadius && Point.x - m_fRadius < aPoint[0].x) ||
			(aPoint[0].x < OldPoint.x + m_fRadius && OldPoint.x - m_fRadius < aPoint[1].x || aPoint[1].x < OldPoint.x + m_fRadius && OldPoint.x - m_fRadius < aPoint[0].x))
		{
			if ((aPoint[0].z < Point.z + m_fRadius && Point.z - m_fRadius < aPoint[1].z || aPoint[1].z < Point.z + m_fRadius && Point.z - m_fRadius < aPoint[0].z) ||
				(aPoint[0].z < OldPoint.z + m_fRadius && OldPoint.z - m_fRadius < aPoint[1].z || aPoint[1].z < OldPoint.z + m_fRadius && OldPoint.z - m_fRadius < aPoint[0].z))
			{
				if (fabsf(fDistance) <= m_fRadius || fOldDistance <= -m_fRadius && fDistance >= -m_fRadius)
				{
					pPos->x -= sinf(rot.y) * (m_fRadius + fDistance);
					pPos->z -= cosf(rot.y) * (m_fRadius + fDistance);
				}
			}
		}
	}
}

void CPlayer::CollisionPlane(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld)
{
	CScene *pScene;
	pScene = CScene::GetScene(CScene::PRIORITY_PLANE);
	CScene *pSceneNext;
	bool bLand = false;
	while (pScene)
	{
		pSceneNext = pScene->GetNext();
		if (pScene->GetObjType() == CScene::OBJECTTYPE_WALL)
		{
			CollisionWall(pPos, posOld, pScene);
		}
		else if (pScene->GetObjType() == CScene::OBJECTTYPE_FIELD && bLand == false)
		{
			bLand = CollisionLand(pPos, posOld, pScene);
		}
		pScene = pSceneNext;
	}
	m_bJump = bLand;
	m_bLanding = bLand;
}

void CPlayer::UnderSearch(D3DXVECTOR3 pos)
{
	CScene *pScene;
	int nCntScene;
	bool bUnder = false;
	bool bObject = false;
	float fUnderPosY = -100000.0f;
	float fFieldY;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 UnderRot;
	for (nCntScene = 0; nCntScene < PRIORITY_MAX; nCntScene++)
	{
		pScene = GetScene((CScene::PRIORITY)nCntScene);
		while (pScene)
		{
			CScene *pSceneNext;
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJECTTYPE_FIELD || pScene->GetObjType() == CScene::OBJECTTYPE_BLOCK)
			{
				D3DXVECTOR3 Scenepos = pScene->GetPos();
				D3DXVECTOR3 VtxMax = pScene->GetVtxMax();
				D3DXVECTOR3 VtxMin = pScene->GetVtxMin();
				if (pos.x - GetRadius() < Scenepos.x + VtxMax.x && Scenepos.x + VtxMin.x < pos.x + GetRadius() &&
					pos.z - GetRadius() < Scenepos.z + VtxMax.z && Scenepos.z + VtxMin.z < pos.z + GetRadius())
				{
					if (pScene->GetObjType() == CScene::OBJECTTYPE_FIELD)
					{
						D3DXVECTOR3 VtxPos[4];
						if (pScene->GetRotX() == false)
						{
							VtxPos[0] = D3DXVECTOR3(VtxMin.x, VtxMax.y, VtxMax.z);
							VtxPos[1] = D3DXVECTOR3(VtxMax.x, VtxMax.y, VtxMax.z);
							VtxPos[2] = D3DXVECTOR3(VtxMin.x, VtxMin.y, VtxMin.z);
							VtxPos[3] = D3DXVECTOR3(VtxMax.x, VtxMin.y, VtxMin.z);
						}
						else
						{
							VtxPos[0] = D3DXVECTOR3(VtxMin.x, VtxMax.y, VtxMax.z);
							VtxPos[1] = D3DXVECTOR3(VtxMax.x, VtxMin.y, VtxMax.z);
							VtxPos[2] = D3DXVECTOR3(VtxMin.x, VtxMax.y, VtxMin.z);
							VtxPos[3] = D3DXVECTOR3(VtxMax.x, VtxMin.y, VtxMin.z);
						}
						rot.x = atan2f(VtxPos[2].y - VtxPos[0].y, VtxPos[2].z - VtxPos[0].z);
						rot.z = atan2f(VtxPos[1].y - VtxPos[0].y, VtxPos[1].x - VtxPos[0].x);
						fFieldY = (pos.z - Scenepos.z) * tanf(rot.x) + (pos.x - Scenepos.x) * tanf(rot.z) + Scenepos.y;
						rot.x = D3DX_PI - rot.x;
						if (fUnderPosY < fFieldY && pos.y > fFieldY)
						{
							fUnderPosY = fFieldY;
							bObject = false;
							bUnder = true;
							UnderRot = rot;
						}
					}
					else
					{
						fFieldY = Scenepos.y + VtxMax.y;
						if (fUnderPosY < fFieldY && pos.y > fFieldY)
						{
							fUnderPosY = fFieldY;
							bObject = true;
							bUnder = true;
						}
					}
				}
			}
			pScene = pSceneNext;
		}
	}
	if (bUnder == true)
	{
		if (bObject == true)
		{
			m_pShadow->MoveY(D3DXVECTOR3(pos.x, fUnderPosY, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		else
		{
			m_pShadow->MoveY(D3DXVECTOR3(pos.x, fUnderPosY, pos.z), UnderRot);
		}
	}
	else
	{
		m_pShadow->MoveY(D3DXVECTOR3(pos.x, pos.y - 100000.0f, pos.z), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	}
}

CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *aFileName)
{
	CPlayer *pPlayer;
	pPlayer = new CPlayer(PRIORITY_CHARA);
	if (pPlayer != NULL)
	{
		pPlayer->Init(pos, rot, aFileName);
	}
	return pPlayer;
}

void CPlayer::Damage(int nDamage, D3DXVECTOR3 clash)
{
	m_nHP -= nDamage;
	if (m_nHP <= 0)
	{
		MotionChange(MOTIONTYPE_DEATH);
		m_nCntNoDmg = 1600;
		m_pHPUI->SetSize(D3DXVECTOR2(0.0f, 58.0f));
		m_pHPUI->SetPos(D3DXVECTOR3(0.0f, 620.0f, 0.0f));
		m_pCamera->SetRockon(true);
		m_pCamera->SetPosR(D3DXVECTOR3(GetPos().x, GetPos().y + (m_fHeight * 0.3f), GetPos().z));
		m_pCamera->SetPosV(D3DXVECTOR3(GetPos().x - 180.0f * sinf(GetRot().y - 0.3f), GetPos().y + (m_fHeight * 0.8f), GetPos().z - 180.0f * cosf(GetRot().y - 0.3f)));
		UnRockon();
		m_rotDesh = GetRot();
		CManager::SetGameEnd(true);
		CSound::Stop(CSound::SOUND_LABEL_GAMEBGM);
		CSound::Play(CSound::SOUND_LABEL_PLAYERDEATH);
	}
	else
	{
		if ((m_nMaxHP / 4) >= m_nHP && m_bPinch == false)
		{
			m_bPinch = true;
			m_nCntPinch = 20;
			CSound::Play(CSound::SOUND_LABEL_PINCH);
		}
		m_nCntNoDmg = 35;
		m_move = clash;
		m_pHPUI->SetSize(D3DXVECTOR2((float)m_nHP / (float)m_nMaxHP * 298.0f, 58.0f));
		m_pHPUI->SetPos(D3DXVECTOR3(1186.5f - ((float)m_nHP / (float)m_nMaxHP * 149.0f), 620.0f, 0.0f));
		CSound::Play(CSound::SOUND_LABEL_DAMAGE);
	}
}

bool CPlayer::SetEnemy(CScene *pEnemy)
{
	int nMax = -1;
	int nCntEnemy;
	for (nCntEnemy = 0; nCntEnemy < 16; nCntEnemy++)
	{
		if (m_pEnemy[nCntEnemy] == pEnemy)
		{
			return false;
		}
		else if (nMax == -1 && m_pEnemy[nCntEnemy] == NULL)
		{
			nMax = nCntEnemy;
		}
	}
	if (nMax > -1)
	{
		m_pEnemy[nMax] = pEnemy;
		if (m_bRockon == true)
		{
			if (pEnemy == m_pRockon->GetRockEnemy())
			{
				int nHP = pEnemy->GetHP() - Damage();
				if (nHP > 0)
				{
					m_pEnemyHPUI[2]->SetSize(D3DXVECTOR2((float)nHP / (float)pEnemy->GetMaxHP() * 248.0f, 38.0f));
					m_pEnemyHPUI[2]->SetPos(D3DXVECTOR3(1221.0f - ((float)nHP / (float)pEnemy->GetMaxHP() * 124.0f), 60.0f, 0.0f));
				}
				else
				{
					int nCntUI;
					for (nCntUI = 0; nCntUI < 3; nCntUI++)
					{
						m_pEnemyHPUI[nCntUI]->FadeColorChange(0);
					}
					UnRockon();
				}
			}
		}
		return true;
	}
	return false;
}

void CPlayer::ResetEnemy()
{
	int nCntEnemy;
	for (nCntEnemy = 0; nCntEnemy < 16; nCntEnemy++)
	{
		if (m_pEnemy[nCntEnemy] != NULL)
		{
			m_pEnemy[nCntEnemy] = NULL;
		}
	}
}

void CPlayer::Rockon()
{
	D3DXVECTOR3 CameraPos = CManager::GetRenderer()->GetCamera()->GetPosV();
	CScene *pScene;
	CScene *pRockScene = NULL;
	pScene = GetScene(PRIORITY_CHARA);
	float fMaxDistance = 100000.0f;
	bool bFront = false;
	while (pScene)
	{
		CScene *pSceneNext;
		pSceneNext = pScene->GetNext();
		if (pScene->GetObjType() == CScene::OBJECTTYPE_ENEMY)
		{
			D3DXVECTOR3 EPos = pScene->GetPos();
			float fRotY = m_pCamera->GetRotY() - atan2f(CameraPos.x - EPos.x, CameraPos.z - EPos.z);
			if (D3DX_PI * (160.0f / 180.0f) < fabsf(fRotY))
			{
				float fLengthX = powf(CameraPos.x - EPos.x, 2.0f);
				float fLengthY = powf((CameraPos.y + m_fHeight / 2) - (EPos.y + pScene->GetHeight() / 2), 2.0f);
				float fLengthZ = powf(CameraPos.z - EPos.z, 2.0f);
				float fHDistance = powf(sqrtf(fLengthX + fLengthZ), 2.0f);
				float fDistance = sqrtf(fHDistance + fLengthY);
				if (fMaxDistance > fDistance)
				{
					fMaxDistance = fDistance;
					pRockScene = pScene;
					bFront = true;
				}
			}
			else if (fabsf(fRotY) < D3DX_PI * (20.0f / 180.0f) && bFront == false)
			{
				float fLengthX = powf(CameraPos.x - EPos.x, 2.0f);
				float fLengthY = powf((CameraPos.y + m_fHeight / 2) - (EPos.y + pScene->GetHeight() / 2), 2.0f);
				float fLengthZ = powf(CameraPos.z - EPos.z, 2.0f);
				float fHDistance = powf(sqrtf(fLengthX + fLengthZ), 2.0f);
				float fDistance = sqrtf(fHDistance + fLengthY);
				if (fMaxDistance > fDistance)
				{
					fMaxDistance = fDistance;
					pRockScene = pScene;
				}
			}
		}
		pScene = pSceneNext;
	}
	if (pRockScene != NULL)
	{
		CSound::Play(CSound::SOUND_LABEL_SELECT);
		m_bRockon = true;
		m_pCamera->SetRockon(true);
		m_pRockon->SetRockon(pRockScene);
		m_pEnemyHPUI[0]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		m_pEnemyHPUI[1]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		m_pEnemyHPUI[2]->FadeColorChange(255);
		m_pEnemyHPUI[2]->SetSize(D3DXVECTOR2((float)pRockScene->GetHP() / (float)pRockScene->GetMaxHP() * 248.0f, 38.0f));
		m_pEnemyHPUI[2]->SetPos(D3DXVECTOR3(1221.0f - ((float)pRockScene->GetHP() / (float)pRockScene->GetMaxHP() * 124.0f), 60.0f, 0.0f));
	}
}

void CPlayer::UnRockon()
{
	m_bRockon = false;
	m_pCamera->SetRockon(false);
	m_pRockon->RemoveRockon();
	m_pEnemyHPUI[0]->ColorChange(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
	m_pEnemyHPUI[1]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	m_pEnemyHPUI[2]->FadeColorChange(0);
}

void CPlayer::RockCameraMove(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 RockPos = m_pRockon->GetPos();
	if (m_pRockon->GetRockon() == false)
	{
		D3DXVECTOR3 EPos = m_pRockon->GetRockEnemy()->GetPos();
		float fRotY = atan2f(pos.x - EPos.x, pos.z - EPos.z) + D3DX_PI;
		m_pCamera->SetRotDeshY(fRotY);
	}
	else
	{
		if (RockPos.x <= 50.0f || SCREEN_WIDTH - 50.0f <= RockPos.x)
		{
			D3DXVECTOR3 EPos = m_pRockon->GetRockEnemy()->GetPos();
			float fRotY = atan2f(pos.x - EPos.x, pos.z - EPos.z) + D3DX_PI;
			m_pCamera->SetRotDeshY(fRotY);
		}
		if (RockPos.y <= 50.0f || SCREEN_HEIGHT - 50.0f <= RockPos.y)
		{
			if (RockPos.y <= 50.0f)
			{
				m_pCamera->AddRotX(0.005f);
			}
			else if (SCREEN_HEIGHT - 50.0f <= RockPos.y)
			{
				m_pCamera->AddRotX(-0.01f);
			}
		}
	}
}

CPlayer::MOTIONTYPE CPlayer::RockAttack(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 Epos = m_pRockon->GetRockEnemy()->GetPos(); //敵の場所
	float fRotY = atan2f(pos.x - Epos.x, pos.z - Epos.z);
	m_rotDesh.y = fRotY;
	SetRot(D3DXVECTOR3(0.0f, fRotY, 0.0f));
	if (m_bCounter == true)
	{
		m_nCombo = 1;
		return MOTIONTYPE_COUNTER;
	}
	if (m_nCombo >= 2)
	{
		m_nCombo = 0;
		return MOTIONTYPE_FINISH;
	}
	m_nCombo++;
	if (Epos.y - pos.y > 140.0f) //敵が自分よりどれだけ高い位置にいるか
	{
		return MOTIONTYPE_ARIAL;
	}
	else if (Epos.y - pos.y < -140.0f) //敵が自分よりどれだけ低い位置にいるか
	{
		return MOTIONTYPE_DIVE;
	}
	else if (sqrtf(powf(Epos.x - pos.x, 2.0f) + powf(Epos.z - pos.z, 2.0f)) > 220.0f || 
		     m_motionType == MOTIONTYPE_SLAP || m_motionTypeOld == MOTIONTYPE_SLAP) //敵と自分との距離が長いか、または直前の攻撃が至近距離のやつだったか
	{
		return MOTIONTYPE_RUSH;
	}
	else if (AroundEnemy(pos) == true) //自分の周りに敵がいるか
	{
		return MOTIONTYPE_ROUND;
	}
	else //上記条件に該当せず（敵が近距離に1体だけ）
	{
		return MOTIONTYPE_SLAP;
	}
}

bool CPlayer::AroundEnemy(D3DXVECTOR3 pos)
{
	CScene *pScene = CScene::GetScene(CScene::PRIORITY_CHARA);
	CScene *pSceneNext;
	float fDistance;
	D3DXVECTOR3 Epos;
	while (pScene)
	{
		pSceneNext = pScene->GetNext();
		if (pScene->GetObjType() == CScene::OBJECTTYPE_ENEMY)
		{
			if (pScene != m_pRockon->GetRockEnemy())
			{
				Epos = pScene->GetPos();
				fDistance = sqrtf(powf(Epos.x - pos.x, 2.0f) + powf(Epos.z - pos.z, 2.0f));
				if (fDistance - m_fRadius - pScene->GetRadius() < 100.0f)
				{
					return true;
				}
			}
		}
		pScene = pSceneNext;

	}
	return false;
}

void CPlayer::SearchRock(D3DXVECTOR3 pos)
{
	if (m_MotionInfo.aKeyInfo[m_nKey].fHeight != 0.0f)
	{
		m_move.y = 0.0f;
	}
	if (m_MotionInfo.bHoming == true)
	{
		D3DXVECTOR3 Epos = m_pRockon->GetRockEnemy()->GetPos();
		m_rotDesh.y = atan2f(pos.x - Epos.x, pos.z - Epos.z);
		float fERadius = m_pRockon->GetRockEnemy()->GetRadius();
		float fHDistance = sqrtf(powf(Epos.x - pos.x, 2.0f) + powf(Epos.z - pos.z, 2.0f)) - m_fRadius - fERadius;	//水平距離
		float fVDistance = Epos.y + (m_pRockon->GetRockEnemy()->GetHeight() / 2) - pos.y;	//垂直距離
		if (fabsf(fHDistance) > fabsf(m_MotionInfo.aKeyInfo[m_nKey].fMove) * 0.8f)	//水平距離が遠すぎる場合
		{
			MotionMove(m_rotDesh.y, m_MotionInfo.aKeyInfo[m_nKey].fMove);
		}
		else //距離が近い場合
		{
			MotionMove(m_rotDesh.y, m_MotionInfo.aKeyInfo[m_nKey].fMove * 0.3f);
		}
		switch (m_MotionInfo.MotionUpdn)
		{
		case MOTIONUPDN_NONE:
			break;
		case MOTIONUPDN_UNDER:
			if (fVDistance < m_MotionInfo.aKeyInfo[m_nKey].fHeight * 0.8f)
			{
				MotionHeight(m_MotionInfo.aKeyInfo[m_nKey].fHeight);
			}
			else
			{
				MotionStopHeight();
			}
			break;
		case MOTIONUPDN_UPPER:
			if (fVDistance > m_MotionInfo.aKeyInfo[m_nKey].fHeight * 1.0f)
			{
				MotionHeight(m_MotionInfo.aKeyInfo[m_nKey].fHeight);
			}
			else
			{
				MotionStopHeight();
			}
			break;
		case MOTIONUPDN_UPDN:
			if (fVDistance < -150.0f)
			{
				MotionHeight(-m_MotionInfo.aKeyInfo[m_nKey].fHeight);
			}
			else if (fVDistance > 150.0f)
			{
				MotionHeight(m_MotionInfo.aKeyInfo[m_nKey].fHeight);
			}
			else
			{
				MotionStopHeight();
			}
			break;
		default:
			break;
		}
	}
	else
	{
		MotionMove(m_rotDesh.y, m_MotionInfo.aKeyInfo[m_nKey].fMove);
		MotionHeight(m_MotionInfo.aKeyInfo[m_nKey].fHeight);
	}
}

void CPlayer::SetParticle()
{
	int nCntPart;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	pos = D3DXVECTOR3(m_MotionInfo.Collision[m_nCntCol].mtxWorld._41, m_MotionInfo.Collision[m_nCntCol].mtxWorld._42, m_MotionInfo.Collision[m_nCntCol].mtxWorld._43);
	float CameraRotY = m_pCamera->GetRotY();
	float fRand = 0.0f;
	float fRandX;
	for (nCntPart = 0; nCntPart < 3; nCntPart++)
	{
		fRandX = fRand + 1.0f;
		if (fRandX > 1.0f)
		{
			fRandX -= 2.0f;
		}
		switch (nCntPart)
		{
		case 0:
			move = D3DXVECTOR3(0.0f, 28.0f, 0.0f); //上
			rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			break;
		case 1:
			move = D3DXVECTOR3(sinf(fRand * D3DX_PI + CameraRotY) * 28.0f, cosf(D3DX_PI * fRand) * 28.0f, sinf(CameraRotY) * -28.0f); //右下
			rot = D3DXVECTOR3(sinf(fRandX * D3DX_PI + CameraRotY) * 1.2f, 0.0f, cosf(fRand * D3DX_PI + CameraRotY) * -1.2f);
			break;
		case 2:
			move = D3DXVECTOR3(sinf(fRand * D3DX_PI + CameraRotY) * 28.0f, cosf(D3DX_PI * fRand) * 28.0f, sinf(CameraRotY) * 28.0f); //左下
			rot = D3DXVECTOR3(sinf(fRandX * D3DX_PI + CameraRotY) * -1.2f, 0.0f, cosf(fRand * D3DX_PI + CameraRotY) * 1.2f);
			break;
		default:
			break;
		}
		CParticle::Create(pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.02f), D3DXVECTOR3(3.0f, 35.0f, 0.0f), 50, 1, move, rot);
		fRand += 0.66f;
		if (fRand > 1.0f)
		{
			fRand -= 2.0f;
		}
	}
}

CPlayer::MOTIONTYPE CPlayer::NoRockAttack()
{
	if (m_bCounter == true)
	{
		m_nCombo = 1;
		return MOTIONTYPE_COUNTER;
	}
	else if (m_nCombo >= 2) //コンボフィニッシュの部分
	{
		m_nCombo = 0;
		return MOTIONTYPE_FINISH;
	}
	else if (m_motionType == MOTIONTYPE_SLAP || m_motionTypeOld == MOTIONTYPE_SLAP) //直前の攻撃が至近距離だった場合
	{
		m_nCombo++;
		return MOTIONTYPE_RUSH;
	}
	else //上記該当せず
	{
		m_nCombo++;
		return MOTIONTYPE_SLAP;
	}
}

void CPlayer::PlayerDamage(D3DXVECTOR3 pos, float fHeight, float fRadius)
{
	CScene *pScene;
	pScene = GetScene(CScene::PRIORITY_CHARA);
	while (pScene)
	{
		CScene *pSceneNext;
		pSceneNext = pScene->GetNext();
		if (pScene->GetObjType() == CScene::OBJECTTYPE_ENEMY)
		{
			if (pScene->CollisionDamage(pos, fRadius, fHeight) == true)
			{
				if (m_motionType == MOTIONTYPE_GUARD)
				{
					MotionChange(MOTIONTYPE_GUARD);
					pScene->Reflect();
					m_bCounter = true;
					m_bHit = true;
					m_pCounterUI->ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					CSound::Play(CSound::SOUND_LABEL_GUARD);
				}
				else if (m_nCntNoDmg <= 0)
				{
					float fRot = atan2f(pos.x - pScene->GetPos().x, pos.z - pScene->GetPos().z);
					float fVClash = pScene->VClash();
					float fHClash = pScene->HClash();
					D3DXVECTOR3 clash;
					clash.x = fHClash * sinf(fRot);
					clash.y = fVClash;
					clash.z = fHClash * cosf(fRot);
					if (fabsf(GetRot().y - fRot) < D3DX_PI * 0.5f)
					{
						MotionChange(MOTIONTYPE_DAMAGEFRONT);
					}
					else
					{
						MotionChange(MOTIONTYPE_DAMAGEBACK);
					}
					pScene->SetParticle();
					Damage(pScene->Damage(), clash);
				}
			}
		}
		pScene = pSceneNext;
	}
}

void CPlayer::PauseChange(int nAdd)
{
	m_pPauseUI[m_nPauseSelect]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f));
	m_nPauseSelect += nAdd;
	if (m_nPauseSelect < 0)
	{
		m_nPauseSelect = 2;
	}
	else if (m_nPauseSelect > 2)
	{
		m_nPauseSelect = 0;
	}
	m_pPauseUI[m_nPauseSelect]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	CSound::Play(CSound::SOUND_LABEL_SELECT);
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
	CSound::Play(CSound::SOUND_LABEL_OK);
}

void CPlayer::EndChange(int nAdd)
{
	m_pEndUI[m_nPauseSelect]->ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
	m_nPauseSelect += nAdd;
	if (m_nPauseSelect < 0)
	{
		m_nPauseSelect = 1;
	}
	else if (m_nPauseSelect > 1)
	{
		m_nPauseSelect = 0;
	}
	m_pEndUI[m_nPauseSelect]->ColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CSound::Play(CSound::SOUND_LABEL_SELECT);
}

void CPlayer::EndSelect()
{
	switch (m_nPauseSelect)
	{
	case 0:
		CFade::SetFade(CManager::MODE_GAME);
		break;
	case 1:
		CFade::SetFade(CManager::MODE_TITLE);
		break;
	default:
		break;
	}
	CSound::Play(CSound::SOUND_LABEL_OK);
}

void CPlayer::GameClear()
{
	MotionChange(MOTIONTYPE_CLEAR);
	UnRockon();
	m_pCamera->SetRockon(true);
	m_pCamera->SetPosR(D3DXVECTOR3(0.0f, m_fHeight * 0.6f, 0.0f));
	m_pCamera->SetPosV(D3DXVECTOR3(90.0f, m_fHeight * 0.8f, -400.0f));
	m_pCamera->SetRot(D3DXVECTOR3(0.0f, GetRot().y - 0.3f, 0.0f));
	m_bGameClear = true;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDesh = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pShadow->MoveY(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CManager::SetGameClear(true);
	CSound::Stop(CSound::SOUND_LABEL_GAMEBGM);
	CSound::Play(CSound::SOUND_LABEL_CLEAR);
}