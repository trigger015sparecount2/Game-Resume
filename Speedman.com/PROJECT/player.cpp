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
#include "camerapanel.h"
#include "gamepad.h"
#include "game.h"
#include "select_ui.h"
#include "veceffect3d.h"
#include "straight3d.h"
#include "effect_ui.h"

#include "score.h"
#include "sound.h"
#include "select_ui.h"
//=============================================================================
//静的
//=============================================================================
int CPlayer::m_nControl = 0;
int CPlayer::m_nLife = 0;
//=============================================================================
//マクロ
//=============================================================================
#define RAND_COLOR (int(rand()%255) + 30)	//色ランダム
#define RAND_MOVE ((int(rand() % 3) + 1 )- (int (rand()%3) + 1))	//移動値ランダム
#define RAND_MOVE_2 ((int(rand() % 3) + 1 ))	//移動値ランダム

CPlayer::CPlayer(int nPriority) : CScene3D::CScene3D(nPriority)
{

}

CPlayer::~CPlayer()
{

}

//初期化処理
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *aFileName)
{
	//プレイヤーのモデルなど読み込み
	Load(aFileName);

	//設定
	SetPos(pos);
	SetRot(rot);
	m_rotDesh = rot;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PosOld = pos;
	m_RestartPos = pos;
	m_pKeyboard = CManager::GetKeyboard();
	m_pGamePad = CManager::GetGamepad();
	m_pMouse = CManager::GetMouse();
	m_pCamera = CManager::GetRenderer()->GetCamera();
	m_pCamera->ResetRot();
	m_bSlack = false;
	m_bSlackMove = false;
	m_bJump = true;
	m_bLandObject = false;
	D3DXVECTOR3 size;
	size.x = GetRadius();
	size.y = 0.0f;
	size.z = GetRadius();
	m_pShadow = CShadow::Create(size, D3DXVECTOR3(pos.x, pos.y + 0.1f, pos.z));
	MotionChange(MOTIONTYPE_RETURN);
	m_nControl = 0;
	m_bLanding = true;
	m_nLife = MAX_LIFE;

	if (m_pGamePad != NULL)
	{
		//ゲームパッドのUI
		CSelectUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, 35, 0, CSelectUI::SELECT_UI_CONTROL);
		CSelectUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, 36, 1, CSelectUI::SELECT_UI_CONTROL);
	}
	else if (m_pKeyboard != NULL)
	{
		//キーボードのUI
		CSelectUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, 37, 0, CSelectUI::SELECT_UI_CONTROL);
		CSelectUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, 38, 1, CSelectUI::SELECT_UI_CONTROL);
	}
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
	CScene3D::Uninit();
}

//更新処理
void CPlayer::Update()
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	pos = GetPos();
	posOld = pos;
	bool bRun = false;
	bool bJump;

	//着地してない
	if (m_bJump == false && m_bLandObject == false)
	{
		bJump = false;
	}
	//着地してる場合、ジャンプ可能
	else
	{
		bJump = true;
		m_bJump = true;
	}

	bool bGameClear = CManager::GetGameClear();
	bool bCountDown = CGame::GetCountFlag();

	//この方法でしかPause実装思い出せませんでした、すまん
	bool Pause = CManager::GetPause();

	//クリアしてない、カウントダウン中
	if (bGameClear == false && bCountDown == true)
	{
		//ポーズしてない
		if (Pause == false)
		{
			//ゲームパッド繋がってる
			if (m_pGamePad != NULL)
			{
				//普通に移動中
				if (m_bSlackMove == false)
				{
					//左スティックで移動
					if (m_pGamePad->LeftStickX() != 0.0f || m_pGamePad->LeftStickY() != 0.0f)
					{
						Move(atan2f(m_pGamePad->LeftStickX(), -m_pGamePad->LeftStickY()) / D3DX_PI);
						bRun = true;
					}
					else
					{
						//止まる
						Stop();
					}

					//移動中、ジャンプ可能、モーションが走る以外の場合、走るモーションにする
					if (bRun == true && m_bJump == true && m_motionType != MOTIONTYPE_RUN)
					{
						MotionChange(MOTIONTYPE_RUN);
					}

					//移動してない、走るモーションの場合、リターンモーション
					if (bRun == false && m_motionType == MOTIONTYPE_RUN)
					{
						MotionChange(MOTIONTYPE_RETURN);
					}

					//A押す、着地してる
					if (m_pGamePad->GetButton(CGamePad::DIP_A) == true && m_bJump == true)
					{
						//ロープ掴むフラグが立ってる
						if (m_bSlack == true)
						{
							//ロープ掴める
							if (CatchSlackline(&pos) == true)
							{
								//ロープ掴んで移動中にする
								m_bLanding = false;
								m_bSlackMove = true;
								CSound::Play(CSound::SOUND_LABEL_GRAB);
								CSound::ControlVoice(CSound::SOUND_LABEL_GRAB, 3);

							}
							//掴めない
							else
							{
								//ジャンプする
								m_move.y = m_fJump;
								m_bJump = false;
								m_bLanding = false;
								MotionChange(MOTIONTYPE_JUMP);
								CSound::Play(CSound::SOUND_LABEL_JUMP);
								CSound::ControlVoice(CSound::SOUND_LABEL_JUMP, 2);

							}
						}
						//ロープ掴むフラグが立ってない
						else
						{
							//ジャンプする
							m_move.y = m_fJump;
							m_bJump = false;
							m_bLanding = false;
							CSound::Play(CSound::SOUND_LABEL_JUMP);
							CSound::ControlVoice(CSound::SOUND_LABEL_JUMP, 2);
							MotionChange(MOTIONTYPE_JUMP);
						}
					}

					//着地してない
					if (m_bJump == false)
					{
						m_move.y -= m_fGravity;
					}

					//オブジェクトに着地してる
					if (m_bLandObject == true)
					{
						//ジャンプ可能
						m_bJump = true;
					}

					//ジャンプ可能かつ直前のフレームでは空中にいる
					if (m_bJump == true && bJump == false)
					{
						//着地モーション
						MotionChange(MOTIONTYPE_LANDING);
					}
				}
				//ロープ掴んで移動中
				else
				{
					//上に倒す
					if (m_pGamePad->LeftStickY() < 0.0f)
					{
						//進む
						SlackMove(m_fRot - D3DX_PI);
						m_rotDesh.y = m_fRot;
						bRun = true;
					}
					//下に倒す
					else if (m_pGamePad->LeftStickY() > 0.0f)
					{
						//戻る
						SlackMove(m_fRot);
						m_rotDesh.y = m_fRot - D3DX_PI;
						bRun = true;
					}
					else
					{
						//止まる
						Stop();
					}

					//A押す
					if (m_pGamePad->GetButton(CGamePad::DIP_A) == true)
					{
						//リターンモーション、普通に移動開始
						MotionChange(MOTIONTYPE_RETURN);
						m_bSlackMove = false;
					}

					//移動してるかつロープ掴んだ移動以外のモーション
					if (bRun == true && m_motionType != MOTIONTYPE_ZIPMOVE)
					{
						//ロープ掴んだ移動モーション
						MotionChange(MOTIONTYPE_ZIPMOVE);
					}

					//移動してないかつロープ掴んだ移動のモーション
					if (bRun == false && m_motionType == MOTIONTYPE_ZIPMOVE)
					{
						//ロープ掴んだリターンモーション
						MotionChange(MOTIONTYPE_ZIPRETURN);
					}
				}

				//右スティック傾けるとカメラ回転
				if (m_pGamePad->RightStickX() != 0.0f)
				{
					//横回転
					m_pCamera->AddRotY(m_pGamePad->RightStickX() * 0.03f);
				}
				if (m_pGamePad->RightStickY() != 0.0f)
				{
					//縦回転
					m_pCamera->AddRotX(m_pGamePad->RightStickY() * 0.03f);
				}
			}
			//ゲームパッドは繋がってないが、キーボードが繋がってる
			else if (m_pKeyboard != NULL)
			{
				//通常移動
				if (m_bSlackMove == false)
				{
					//矢印かWASDによる移動
					if (m_pKeyboard->GetPress(DIK_LEFT) == true || m_pKeyboard->GetPress(DIK_A) == true)
					{
						if (m_pKeyboard->GetPress(DIK_DOWN) == true || m_pKeyboard->GetPress(DIK_S) == true)
						{
							//左下
							Move(-0.75f);
						}
						else if (m_pKeyboard->GetPress(DIK_UP) == true || m_pKeyboard->GetPress(DIK_W) == true)
						{
							//左上
							Move(-0.25f);
						}
						else
						{
							//左
							Move(-0.5f);
						}
						bRun = true;
					}
					else if (m_pKeyboard->GetPress(DIK_RIGHT) == true || m_pKeyboard->GetPress(DIK_D) == true)
					{
						if (m_pKeyboard->GetPress(DIK_DOWN) == true || m_pKeyboard->GetPress(DIK_S) == true)
						{
							//右下
							Move(0.75f);
						}
						else if (m_pKeyboard->GetPress(DIK_UP) == true || m_pKeyboard->GetPress(DIK_W) == true)
						{
							//右上
							Move(0.25f);
						}
						else
						{
							//右
							Move(0.5f);
						}
						bRun = true;
					}
					else if (m_pKeyboard->GetPress(DIK_UP) == true || m_pKeyboard->GetPress(DIK_W) == true)
					{
						//上
						Move(0.0f);
						bRun = true;
					}
					else if (m_pKeyboard->GetPress(DIK_DOWN) == true || m_pKeyboard->GetPress(DIK_S) == true)
					{
						//下
						Move(1.0f);
						bRun = true;
					}
					else
					{
						//止まる
						Stop();
						bRun = false;
					}

					//移動中、着地してる、モーションが走る以外
					if (bRun == true && m_bJump == true && m_motionType != MOTIONTYPE_RUN)
					{
						//走るモーション
						MotionChange(MOTIONTYPE_RUN);
					}

					//移動してない、走るモーション
					if (bRun == false && m_motionType == MOTIONTYPE_RUN)
					{
						//リターンモーション
						MotionChange(MOTIONTYPE_RETURN);
					}

					//スペース押した、着地済み
					if (m_pKeyboard->GetKey(DIK_SPACE) == true && m_bJump == true)
					{
						//ロープに掴める
						if (m_bSlack == true)
						{
							//ロープ掴む
							if (CatchSlackline(&pos) == true)
							{
								m_bLanding = false;
								m_bSlackMove = true;
								CSound::Play(CSound::SOUND_LABEL_GRAB);
								CSound::ControlVoice(CSound::SOUND_LABEL_GRAB, 3);
							}
							//ロープ掴めない
							else
							{
								//ジャンプ
								m_move.y = m_fJump;
								m_bJump = false;
								m_bLanding = false;
								MotionChange(MOTIONTYPE_JUMP);
								CSound::Play(CSound::SOUND_LABEL_JUMP);
								CSound::ControlVoice(CSound::SOUND_LABEL_JUMP, 2);
							}
						}
						//ロープ掴めない
						else
						{
							//ジャンプ
							m_move.y = m_fJump;
							m_bJump = false;
							m_bLanding = false;

							CSound::Play(CSound::SOUND_LABEL_JUMP);
							CSound::ControlVoice(CSound::SOUND_LABEL_JUMP, 2);

							MotionChange(MOTIONTYPE_JUMP);
						}
					}

					//着地してない
					if (m_bJump == false)
					{
						m_move.y -= m_fGravity;
					}

					//オブジェクトに乗ってる
					if (m_bLandObject == true)
					{
						//着地してる
						m_bJump = true;
					}
				}
				//ロープ掴んでる
				else
				{
					//上
					if (m_pKeyboard->GetPress(DIK_UP) == true || m_pKeyboard->GetPress(DIK_W) == true)
					{
						//進む
						SlackMove(m_fRot - D3DX_PI);
						m_rotDesh.y = m_fRot;
						bRun = true;
					}
					//下
					else if (m_pKeyboard->GetPress(DIK_DOWN) == true || m_pKeyboard->GetPress(DIK_S) == true)
					{
						//戻る
						SlackMove(m_fRot);
						m_rotDesh.y = m_fRot - D3DX_PI;
						bRun = true;
					}
					else
					{
						//止まる
						Stop();
					}

					//スペース
					if (m_pKeyboard->GetKey(DIK_SPACE) == true)
					{
						//リターンモーション、普通に移動開始
						MotionChange(MOTIONTYPE_RETURN);
						m_bSlackMove = false;
					}

					//移動してるかつロープ掴んだ移動以外のモーション
					if (bRun == true && m_motionType != MOTIONTYPE_ZIPMOVE)
					{
						//ロープ掴んだ移動モーション
						MotionChange(MOTIONTYPE_ZIPMOVE);
					}

					//移動してないかつロープ掴んだ移動のモーション
					if (bRun == false && m_motionType == MOTIONTYPE_ZIPMOVE)
					{
						//ロープ掴んだリターンモーション
						MotionChange(MOTIONTYPE_ZIPRETURN);
					}
				}

				//マウス繋がってる
				if (m_pMouse != NULL)
				{
					//動かすとカメラ動く
					m_pCamera->AddRotY(m_pMouse->MouseX() * 0.003f);
					m_pCamera->AddRotX(m_pMouse->MouseY() * 0.003f);
				}
			}
			pos += m_move;

			//様々な物との当たり判定
			CollisionObject(&pos, posOld);
			m_bJump = Land(&pos, posOld);
			Trampoline(&pos, posOld);
			CameraWarp(pos);
			m_bSlack = Slackline(&pos);

			//空中にいる場合、真下にあるものを探す(影の処理)
			if (m_bJump == false)
			{
				UnderSearch(pos);
			}
		}
	}

	//カメラや自分、影の移動
	m_pCamera->SetPosV(D3DXVECTOR3(pos.x, pos.y + 140.0f, pos.z - 300.0f));
	m_pCamera->SetPosR(D3DXVECTOR3(pos.x, pos.y + m_fHeight, pos.z));
	SetPos(pos);
	m_pShadow->Move(pos.y);

	//ゴールとの当たり判定
	Goal(pos);

	//落下用のリスポーン
	if (pos.y < -300.0f)
	{
		//ライフ減らす
		m_nLife--;
		if (m_nLife < 0)
		{
			m_nLife = 0;
		}
		//まだライフある
		else if(m_nLife >= 1)
		{
			//リスポーン
			m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			SetPos(m_RestartPos);
			MotionChange(MOTIONTYPE_RETURN);
			CSound::Play(CSound::SOUND_LABEL_FALL);
		}
	}

	//説明の切り替え
	if (m_bSlackMove == true || m_bSlack == true)
	{
		m_nControl = 1;
	}
	else
	{
		m_nControl = 0;
	}

	////チート操作
	//if (m_pKeyboard != NULL)
	//{
	//	//リスポーン地点に戻る
	//	if (m_pKeyboard->GetKey(DIK_RETURN) == true)
	//	{
	//		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//		SetPos(m_RestartPos);
	//	}
	//	//リスポーン地点の設定
	//	if (m_pKeyboard->GetKey(DIK_LSHIFT) == true)
	//	{
	//		m_RestartPos = pos;
	//	}
	//}

	//自分の方向関連
	D3DXVECTOR3 rot = GetRot();

	//rotが狂わないようにする
	if (m_rotDesh.y >= D3DX_PI)
	{
		m_rotDesh.y -= D3DX_PI * 2;
	}
	if (m_rotDesh.y < -D3DX_PI)
	{
		m_rotDesh.y += D3DX_PI * 2;
	}
	if (rot.y >= D3DX_PI)
	{
		rot.y -= D3DX_PI * 2;
	}
	if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2;
	}

	//自分の方向と目的の方向が同じじゃない
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
	if (Pause == false)
	{
		//モーション、rot設定
		Motion();
		SetRot(rot);
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
		m_pModel[nCntModel]->Draw();
	}
}

//キャラ読み込み
void CPlayer::Load(const char *aFileName)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する
	D3DXVECTOR3 rot;
	D3DXVECTOR3 pos;
	int nParent;
	CModel *paModel[32];

	int nCntParts = 0;
	//int nCntModelParts = 0;
	int nCntMotion = 0;
	int nCntKey = 0;
	char aFile[256];
	float fRadius;
	int nLoop = 0;

	bool bChara = false;
	bool bParts = false;
	bool bMotion = false;
	bool bKey = false;
	bool bKeySet = false;
	int nIndex;
	//int nLoop;

	FILE *pFile;
	pFile = fopen(aFileName, "r");
	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]); //fscanfを繰り返してファイルを読み取っていく
			if (strcmp(&aFile[0], "NUM_MODEL") == 0) //モデル数
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &m_nNumModel);
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
					fscanf(pFile, "%f", &fRadius);
					SetRadius(fRadius);
				}
				if (strcmp(&aFile[0], "HEIGHT") == 0) //身長
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &m_fHeight);
				}
				if (strcmp(&aFile[0], "PARTSSET") == 0)	//パーツセット
				{
					bParts = true;
				}
				if (strcmp(&aFile[0], "END_PARTSSET") == 0)	//パーツセット終わり
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
			if (bMotion == true) //MOTIONSET時
			{
				if (strcmp(&aFile[0], "LOOP") == 0) //そのモーションがループするか(0だとしない。それ以外だとする)
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
				if (strcmp(&aFile[0], "NUM_KEY") == 0) //いくつキーがあるか
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].nNumKey);
				}
				//if (strcmp(&aFile[0], "NON_OPE") == 0) //操作不能時間
				//{
				//	fscanf(pFile, "%s", &aFile[0]);
				//	fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].nNonOpe);
				//}
				if (strcmp(&aFile[0], "KEYSET") == 0)	//キー設定開始
				{
					bKeySet = true;
					nCntParts = 0;
				}
				if (strcmp(&aFile[0], "END_KEYSET") == 0)	//キー設定終了
				{
					bKeySet = false;
					nCntKey++;
				}
				if (bKeySet == true) //KEYSET時
				{
					if (strcmp(&aFile[0], "FRAME") == 0) //フレーム数
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
					}
					//if (strcmp(&aFile[0], "MOVE") == 0) //移動量
					//{
					//	fscanf(pFile, "%s", &aFile[0]);
					//	fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].fMove);
					//}
					if (strcmp(&aFile[0], "KEY") == 0)	//キーごとのposやrot
					{
						bKey = true;
					}
					if (strcmp(&aFile[0], "END_KEY") == 0)	//キーごとのposやrot
					{
						bKey = false;
						nCntParts++;
					}
					if (bKey == true) //KEY時
					{
						if (strcmp(&aFile[0], "POS") == 0) //場所
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%f %f %f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosX,
								&m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosY, &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosZ);
						}
						if (strcmp(&aFile[0], "ROT") == 0) //角度
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%f %f %f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotX,
								&m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotY, &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotZ);
						}
					}
				}
			}
			if (strcmp(&aFile[0], "CHARACTERSET") == 0)	//キャラ設定開始
			{
				bChara = true;
				nCntParts = 0;
			}
			if (strcmp(&aFile[0], "END_CHARACTERSET") == 0)	//キャラ設定終了
			{
				bChara = false;
			}
			if (strcmp(&aFile[0], "MOTIONSET") == 0)	//モーション設定開始
			{
				bMotion = true;
				nCntKey = 0;
			}
			if (strcmp(&aFile[0], "END_MOTIONSET") == 0)	//モーション設定終了
			{
				bMotion = false;
				nCntMotion++;
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
	m_aMotionInfo[MOTIONTYPE_RETURN].aKeyInfo[0].nFrame = 10;
	m_aMotionInfo[MOTIONTYPE_RETURN].nNumKey = 1;
	for (nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{
		m_aMotionInfo[MOTIONTYPE_RETURN].aKeyInfo[0].aKey[nCntParts].fPosX = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fPosX;
		m_aMotionInfo[MOTIONTYPE_RETURN].aKeyInfo[0].aKey[nCntParts].fPosY = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fPosY;
		m_aMotionInfo[MOTIONTYPE_RETURN].aKeyInfo[0].aKey[nCntParts].fPosZ = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fPosZ;
		m_aMotionInfo[MOTIONTYPE_RETURN].aKeyInfo[0].aKey[nCntParts].fRotX = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fRotX;
		m_aMotionInfo[MOTIONTYPE_RETURN].aKeyInfo[0].aKey[nCntParts].fRotY = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fRotY;
		m_aMotionInfo[MOTIONTYPE_RETURN].aKeyInfo[0].aKey[nCntParts].fRotZ = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fRotZ;
	}

	//「MOTIONTYPE_ZIPRETURN」は、ロープ掴んでる間のニュートラルモーションに戻る際に経由するモーション
	m_aMotionInfo[MOTIONTYPE_ZIPRETURN].bLoop = false;
	m_aMotionInfo[MOTIONTYPE_ZIPRETURN].aKeyInfo[0].nFrame = 10;
	m_aMotionInfo[MOTIONTYPE_ZIPRETURN].nNumKey = 1;
	for (nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{
		m_aMotionInfo[MOTIONTYPE_ZIPRETURN].aKeyInfo[0].aKey[nCntParts].fPosX = m_aMotionInfo[MOTIONTYPE_ZIPNUETRAL].aKeyInfo[1].aKey[nCntParts].fPosX;
		m_aMotionInfo[MOTIONTYPE_ZIPRETURN].aKeyInfo[0].aKey[nCntParts].fPosY = m_aMotionInfo[MOTIONTYPE_ZIPNUETRAL].aKeyInfo[1].aKey[nCntParts].fPosY;
		m_aMotionInfo[MOTIONTYPE_ZIPRETURN].aKeyInfo[0].aKey[nCntParts].fPosZ = m_aMotionInfo[MOTIONTYPE_ZIPNUETRAL].aKeyInfo[1].aKey[nCntParts].fPosZ;
		m_aMotionInfo[MOTIONTYPE_ZIPRETURN].aKeyInfo[0].aKey[nCntParts].fRotX = m_aMotionInfo[MOTIONTYPE_ZIPNUETRAL].aKeyInfo[1].aKey[nCntParts].fRotX;
		m_aMotionInfo[MOTIONTYPE_ZIPRETURN].aKeyInfo[0].aKey[nCntParts].fRotY = m_aMotionInfo[MOTIONTYPE_ZIPNUETRAL].aKeyInfo[1].aKey[nCntParts].fRotY;
		m_aMotionInfo[MOTIONTYPE_ZIPRETURN].aKeyInfo[0].aKey[nCntParts].fRotZ = m_aMotionInfo[MOTIONTYPE_ZIPNUETRAL].aKeyInfo[1].aKey[nCntParts].fRotZ;
	}
}

//移動
void CPlayer::Move(float fRotY)
{
	m_move.x += (sinf(m_pCamera->GetRotY() + D3DX_PI * fRotY) * m_fMove - m_move.x) * 0.1f;
	m_move.z += (cosf(m_pCamera->GetRotY() + D3DX_PI * fRotY) * m_fMove - m_move.z) * 0.1f;
	m_rotDesh.y = m_pCamera->GetRotY() + D3DX_PI * fRotY + D3DX_PI;
}

//ロープを掴んでる間の移動
void CPlayer::SlackMove(float fRotY)
{
	m_move.x += (sinf(fRotY) * m_fMove * 0.2f - m_move.x) * 0.05f;
	m_move.z += (cosf(fRotY) * m_fMove * 0.2f - m_move.z) * 0.05f;
}

//止まる
void CPlayer::Stop()
{
	m_move.x -= m_move.x * 0.2f;
	m_move.z -= m_move.z * 0.2f;
}

//ゴールに触れたか
void CPlayer::Goal(D3DXVECTOR3 PlayerPos)
{
	CScene *pScene;
	pScene = GetScene(4);
	while (pScene)
	{
		CScene *pSceneNext;
		pSceneNext = pScene->GetNext();
		//ゴールのみ
		if (pScene->GetObjType() == CScene::OBJCTTYPE_GOAL)
		{
			//posなどを取得、距離計算
			D3DXVECTOR3 pos = pScene->GetPos();
			float fRadius = pScene->GetRadius();
			float fLengthX = powf(GetPos().x - pos.x, 2);
			float fLengthZ = powf(GetPos().z - pos.z, 2);
			float fDistance = sqrtf(fLengthX + fLengthZ);

			//距離がお互いの半径より短い
			if (fDistance <= fRadius + GetRadius())
			{
				//自身の頭がゴールよりも上
				if (PlayerPos.y + m_fHeight >= pos.y)
				{
					//チュートリアル
					if (CManager::GetMode() == CManager::MODE_TUTORIAL)
					{
						//ゲームに移行
						CFade::SetFade(CManager::MODE_GAME);
						CSound::Stop(CSound::SOUND_LABEL_GAMEBGM);
						CScore::SetTime(TIME_LIMIT);
					}
					//ゲーム
					else if (CManager::GetMode() == CManager::MODE_GAME)
					{
						//クリアしてない(触れた瞬間)
						bool GameClear = CManager::GetGameClear();
						if (GameClear == false)
						{
							//諸々の生成
							CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 190.0f, 0.0f), 920.0f, 110.0f, 26);
							CManager::SetGameEnd(true);

							CUI::Create(D3DXVECTOR3(640.0f, 280.0f, 0), 300.0f, 50.0f, 28);		//UI

							//プレイヤースコア
							CScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 125.0f, 360.0f, 0), 50, 70, 2, 5, CScore::NUMBER_SCORE);

							//パーティクル出す
							CEffectUI::Create(D3DXVECTOR3(0.0, SCREEN_HEIGHT - 2, 0.0f), 0, 0, 0, CEffectUI::EFFECT_PETAL);
							CManager::SetGameClear(true);
							MotionChange(MOTIONTYPE_NEUTRAL);
							CSound::Play(CSound::SOUND_LABEL_CHEERS);
							CSound::ControlVoice(CSound::SOUND_LABEL_CHEERS, 2);
							CSound::Stop(CSound::SOUND_LABEL_GAMEBGM);
							CSound::Play(CSound::SOUND_LABEL_RESULTBGM);
							CSound::ControlVoice(CSound::SOUND_LABEL_RESULTBGM, 1.3f);
						}
					}
				}
			}
			break;
		}
		pScene = pSceneNext;
	}
}

//カメラパネルを踏む
void CPlayer::CameraWarp(D3DXVECTOR3 pos)
{
	CScene *pScene;
	pScene = GetScene(2);
	while (pScene)
	{
		CScene *pSceneNext;
		pSceneNext = pScene->GetNext();
		//カメラパネル
		if (pScene->GetObjType() == CScene::OBJCTTYPE_CAMERAPANEL)
		{
			D3DXVECTOR3 ScenePos = pScene->GetPos();
			D3DXVECTOR3 VtxMax = pScene->GetVtxMax();
			D3DXVECTOR3 VtxMin = pScene->GetVtxMin();
			float fPRadius = GetRadius();

			//当たり判定
			if (ScenePos.x + VtxMin.x <= pos.x + fPRadius && pos.x - fPRadius <= ScenePos.x + VtxMax.x &&
				ScenePos.z + VtxMin.z <= pos.z + fPRadius && pos.z - fPRadius <= ScenePos.z + VtxMax.z)
			{
				if (pos.y <= ScenePos.y && ScenePos.y <= pos.y + m_fHeight)
				{
					//カメラが指定の向きに切り替わる
					m_pCamera->SetPosR(pScene->GetCameraPosR());
					m_pCamera->SetPosV(pScene->GetCameraPosV());
					m_pCamera->SetRot();
				}
			}
		}
		pScene = pSceneNext;
	}
}

//トランポリン
void CPlayer::Trampoline(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld)
{
	CScene *pScene;
	pScene = GetScene(4);
	while (pScene)
	{
		CScene *pSceneNext;
		pSceneNext = pScene->GetNext();
		//トランポリン
		if (pScene->GetObjType() == CScene::OBJCTTYPE_TRAMPOLINE)
		{
			D3DXVECTOR3 pos = pScene->GetPos();
			float fRadius = pScene->GetRadius();
			float fLengthX = powf(pPos->x - pos.x, 2);
			float fLengthZ = powf(pPos->z - pos.z, 2);
			float fDistance = sqrtf(fLengthX + fLengthZ);

			//当たり判定
			if (fDistance <= fRadius + GetRadius())
			{
				if (posOld.y >= pos.y && pos.y >= pPos->y)
				{
					//大ジャンプする、エフェクト作成
					pPos->y = pos.y;
					m_move.y = m_fJump * 1.7f;
					MotionChange(MOTIONTYPE_JUMP);

					CStraight3D::Create(D3DXVECTOR3(pPos->x, pPos->y, pPos->z),
						D3DXVECTOR3(2.0f, 2.0f, 0.0f),
						D3DXVECTOR3(4.0f, 4.0f, 0.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXCOLOR(255, 255, 255, 255),
						D3DXCOLOR(0, 0, 0, 0),
						5, 20);

					for (int nCnt = 0; nCnt < 10; nCnt++)
					{
						CVecEffect3D::Create(D3DXVECTOR3(pPos->x, pPos->y + 20, pPos->z),
							D3DXVECTOR3(5.0f, 5.0f, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXVECTOR3(sinf(CIRCLE) * 3, 0.0f, cosf(CIRCLE) * 3),
							D3DXCOLOR(0, 255, 255, 255),
							D3DXCOLOR(0, 0, 0, 0),
							3,
							30);
					}

					//音鳴らす
					CSound::Play(CSound::SOUND_LABEL_JUMPPAD);
					CSound::ControlVoice(CSound::SOUND_LABEL_JUMPPAD,2);
				}
			}
		}
		pScene = pSceneNext;
	}
}

//ロープ掴めるか
bool CPlayer::Slackline(D3DXVECTOR3 *pPos)
{
	bool bSlack = false;
	CScene *pScene;
	pScene = GetScene(3);
	while (pScene)
	{
		CScene *pSceneNext;
		pSceneNext = pScene->GetNext();
		//スラックライン
		if (pScene->GetObjType() == CScene::OBJCTTYPE_SLACKLINE)
		{
			float fRadius = pScene->GetRadius();
			float fPRadius = GetRadius();
			D3DXVECTOR3 pos = pScene->GetPos();
			D3DXVECTOR3 VtxMin = pScene->GetVtxMin();
			D3DXVECTOR3 VtxMax = pScene->GetVtxMax();
			float fLengthX = pPos->x - pos.x;
			float fLengthY = pPos->y - pos.y;
			//float fLengthYbot = pPos->y + m_fHeight - pos.y;
			float fLengthZ = pPos->z - pos.z;
			fLengthX = powf(fLengthX, 2);
			fLengthY = powf(fLengthY, 2);
			fLengthZ = powf(fLengthZ, 2);
			
			//十分近い
			if (fLengthX + fLengthZ <= powf(fPRadius * 1.5f + fRadius, 2))
			{
				if (pPos->y <= pos.y + VtxMax.y && pPos->y + m_fHeight >= pos.y)
				{
					//フラグオン
					bSlack = true;
				}
			}

			//当たってる
			if (sqrtf(fLengthX + fLengthZ) <= fPRadius + fRadius)
			{
				if (pPos->y <= pos.y + VtxMax.y && pPos->y + m_fHeight >= pos.y)
				{
					//プレイヤー押しのけられる
					float fRotY = atan2f(pPos->x - pos.x, pPos->z - pos.z);
					pPos->x = pos.x + sinf(fRotY) * (fPRadius + fRadius);
					pPos->z = pos.z + cosf(fRotY) * (fPRadius + fRadius);
					
					//ロープ掴んで移動中だったら、解除する
					if (m_bSlackMove == true)
					{
						MotionChange(MOTIONTYPE_RETURN);
						m_bSlackMove = false;
					}
				}
			}
		}
		pScene = pSceneNext;
	}
	return bSlack;
}

bool CPlayer::CatchSlackline(D3DXVECTOR3 *pPos)
{
	CScene *pScene;
	pScene = GetScene(3);
	while (pScene)
	{
		CScene *pSceneNext;
		pSceneNext = pScene->GetNext();
		//スラックライン
		if (pScene->GetObjType() == CScene::OBJCTTYPE_SLACKLINE)
		{
			D3DXVECTOR3 pos = pScene->GetPos();
			D3DXVECTOR3 VtxMin = pScene->GetVtxMin();
			D3DXVECTOR3 VtxMax = pScene->GetVtxMax();
			float fRadius = pScene->GetRadius();
			float fPRadius = GetRadius();
			float fRotY;
			int nSlack = pScene->GetSlack();

			//近い
			if (pos.x - fRadius <= pPos->x + fPRadius && pPos->x - fPRadius <= pos.x + fRadius &&
				pos.z - fRadius <= pPos->z + fPRadius && pPos->z - fPRadius <= pos.z + fRadius)
			{
				if (pPos->y <= pos.y + VtxMax.y && pPos->y + m_fHeight >= pos.y)
				{
					//次に向かう方向を検索
					D3DXVECTOR3 NextPos;
					if (nSlack % 2 == 0)
					{
						NextPos = SearchSlackline(nSlack + 1);
					}
					else
					{
						NextPos = SearchSlackline(nSlack - 1);
					}

					//特定の場所にワープ、次の方向に向く
					fRotY = atan2f(pos.x - NextPos.x, pos.z - NextPos.z);
					m_rotDesh.y = fRotY;
					SetRot(m_rotDesh);
					m_fRot = fRotY;
					pPos->x = pos.x - sinf(fRotY) * (fRadius + fPRadius + 4.0f);
					pPos->z = pos.z - cosf(fRotY) * (fRadius + fPRadius + 4.0f);
					pPos->y = pos.y + 35.0f;
					m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					MotionChange(MOTIONTYPE_ZIPRETURN);
					return true;
				}
			}
		}
		pScene = pSceneNext;
	}
	return false;
}

//スラックライン検索
D3DXVECTOR3 CPlayer::SearchSlackline(int nNumber)
{
	CScene *pScene;
	pScene = GetScene(3);
	while (pScene)
	{
		CScene *pSceneNext;
		pSceneNext = pScene->GetNext();
		//スラックライン
		if (pScene->GetObjType() == CScene::OBJCTTYPE_SLACKLINE)
		{
			//番号一致
			if (pScene->GetSlack() == nNumber)
			{
				return pScene->GetPos();
			}
		}
		pScene = pSceneNext;
	}
	return D3DXVECTOR3(0.0f,0.0f,0.0f);
}

//オブジェクトとの当たり判定
void CPlayer::CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld)
{
	CScene *pScene;
	pScene = GetScene(2);
	bool bLand = false;
	while (pScene)
	{
		CScene *pSceneNext;
		pSceneNext = pScene->GetNext();
		//ブロック
		if(pScene->GetObjType() == CScene::OBJECTTYPE_BLOCK)
		{
			//当たり判定が球状
			if (pScene->GetCollision() == CScene::COLLISION_SPHERE)
			{
				D3DXVECTOR3 pos = pScene->GetPos();
				float fRadius = pScene->GetRadius();
				//float fPRadius = GetRadius();
				float fRotY;
				float fLengthX = pPos->x - pos.x;
				float fLengthY = (pPos->y + (m_fHeight / 2)) - pos.y;
				//float fLengthYbot = pPos->y + m_fHeight - pos.y;
				float fLengthZ = pPos->z - pos.z;
				fLengthX = powf(fLengthX, 2);
				fLengthY = powf(fLengthY, 2);
				fLengthZ = powf(fLengthZ, 2);

				//当たった
				if (fLengthX + fLengthY + fLengthZ <= powf((m_fHeight / 2) + fRadius, 2)/* || fLengthX + fLengthYbot + fLengthZ <= powf(fPRadius + fRadius, 2)*/)
				{
					//プレイヤーを移動させる
					fRotY = atan2f(pos.x - pPos->x, pos.z - pPos->z);
					pPos->x = pos.x - sinf(fRotY) * ((m_fHeight / 2) + fRadius);
					pPos->z = pos.z - cosf(fRotY) * ((m_fHeight / 2) + fRadius);
					m_bSlackMove = false;
				}
			}

			//当たり判定が箱型
			if (pScene->GetCollision() == CScene::COLLISION_SQUARE)
			{
				D3DXVECTOR3 pos = pScene->GetPos();
				D3DXVECTOR3 VtxMin = pScene->GetVtxMin();
				D3DXVECTOR3 VtxMax = pScene->GetVtxMax();
				float fPRadius = GetRadius();

				//XZに当たってる
				if (pos.x + VtxMin.x <= pPos->x + fPRadius && pPos->x - fPRadius <= pos.x + VtxMax.x &&
					pos.z + VtxMin.z <= pPos->z + fPRadius && pPos->z - fPRadius <= pos.z + VtxMax.z)
				{
					//上から当たる(着地)
					if (posOld.y >= pos.y + VtxMax.y && pPos->y <= pos.y + VtxMax.y)
					{
						pPos->y = pos.y + VtxMax.y;
						m_move.y = 0.0f;
						bLand = true;
						m_pShadow->MoveY(*pPos, 0.0f);

						//着地してない
						if (m_bLanding == false)
						{
							CSound::Play(CSound::SOUND_LABEL_LANDING);
							//着地パーティクル
							for (int nCnt = 0; nCnt < 10; nCnt++)
							{
								CVecEffect3D::Create(D3DXVECTOR3(pPos->x, pPos->y + 10, pPos->z),
									D3DXVECTOR3(3.0f, 3.0f, 0.0f),
									D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									D3DXVECTOR3(sinf(CIRCLE) * 1, 0.0f, cosf(CIRCLE) * 1),
									D3DXCOLOR(255, 255, 0, 255),
									D3DXCOLOR(0, 0, 0, -10),
									3,
									30);
							}
							MotionChange(MOTIONTYPE_LANDING);
							m_bLanding = true;
						}
					}
					//下から当たる
					else if (posOld.y + m_fHeight <= pos.y + VtxMin.y && pPos->y + m_fHeight >= pos.y + VtxMin.y)
					{
						pPos->y = pos.y + VtxMin.y - m_fHeight;
						m_move.y = 0.0f;
					}
					//Yも当たってる
					else if (pos.y + VtxMin.y <= pPos->y + m_fHeight && pPos->y <= pos.y + VtxMax.y)
					{
						bool bIn = false;
						//左に当たる
						if (pos.x + VtxMin.x <= pPos->x + fPRadius && posOld.x + fPRadius <= pos.x + VtxMin.x)
						{
							pPos->x = pos.x + VtxMin.x - fPRadius;
							m_bSlackMove = false;
							bIn = true;
						}
						//右に当たる
						else if (pos.x + VtxMax.x >= pPos->x - fPRadius && posOld.x - fPRadius >= pos.x + VtxMax.x)
						{
							pPos->x = pos.x + VtxMax.x + fPRadius + 0.1f;
							m_bSlackMove = false;
							bIn = true;
						}
						//手前に当たる
						if (pos.z + VtxMin.z <= pPos->z + fPRadius && posOld.z + fPRadius <= pos.z + VtxMin.z)
						{
							pPos->z = pos.z + VtxMin.z - fPRadius;
							m_bSlackMove = false;
							bIn = true;
						}
						//奥に当たる
						else if (pos.z + VtxMax.z >= pPos->z - fPRadius && posOld.z - fPRadius >= pos.z + VtxMax.z)
						{
							pPos->z = pos.z + VtxMax.z + fPRadius + 0.1f;
							m_bSlackMove = false;
							bIn = true;
						}
						//向こうが動いていてそれに当たる
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
							//4面からの距離を計算、一番短い距離から
							for (nCnt = 0; nCnt < 4; nCnt++)
							{
								if (fabsf(fMinDistance) > fabsf(fDistance[nCnt]))
								{
									fMinDistance = fDistance[nCnt];
									nDistance = nCnt;
								}
							}

							//方向により、押し出す位置を変える
							switch (nDistance)
							{
							case 0:	//右
								pPos->x = pos.x + VtxMax.x + fPRadius + 0.1f;
								break;
							case 1:	//左
								pPos->x = pos.x + VtxMin.x - fPRadius;
								break;
							case 2:	//奥
								pPos->z = pos.z + VtxMax.z + fPRadius + 0.1f;
								break;
							case 3:	//手前
								pPos->z = pos.z + VtxMin.z - fPRadius;
								break;
							default:
								break;
							}
							m_bSlackMove = false;
						}
					}
				}
			}
		}

		//チェックポイント
		if (pScene->GetObjType() == CScene::OBJCTTYPE_CHECKPOINT)
		{
			D3DXVECTOR3 pos = pScene->GetPos();
			float fRadius = pScene->GetRadius();
			float fPRadius = GetRadius();
			float fRotY;
			float fLengthX = pPos->x - pos.x;
			float fLengthY = pPos->y - pos.y;
			//float fLengthYbot = pPos->y + m_fHeight - pos.y;
			float fLengthZ = pPos->z - pos.z;
			fLengthX = powf(fLengthX, 2);
			fLengthY = powf(fLengthY, 2);
			fLengthZ = powf(fLengthZ, 2);

			//水平方向の距離を計算
			if (fLengthX + fLengthZ <= powf(fPRadius + fRadius, 2))
			{
				//Y方向に当たってるか
				if (pos.y <= pPos->y + m_fHeight && pPos->y <= pos.y + fRadius)
				{
					fRotY = atan2f(pos.x - pPos->x, pos.z - pPos->z);
					pPos->x = pos.x - sinf(fRotY) * (fPRadius + fRadius);
					pPos->z = pos.z - cosf(fRotY) * (fPRadius + fRadius);
					m_RestartPos = pos;

					//チェックポイントパーティクル
					for (int nCnt = 0; nCnt < 20; nCnt++)
					{
						int nColorR = RAND_COLOR;
						int nColorG = RAND_COLOR;
						int nColorB = RAND_COLOR;

						float r = CIRCLE;
						int Movex = RAND_MOVE_2;
						int Movez = RAND_MOVE_2;
						int Movey = RAND_MOVE_2;

						CVecEffect3D::Create(D3DXVECTOR3(pos.x, pos.y + 10, pos.z),
							D3DXVECTOR3(3.0f, 3.0f, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXVECTOR3(sinf(r) * Movex, Movey, cosf(r) * Movez),
							D3DXCOLOR(nColorR, nColorG, nColorB, 255.0f),
							D3DXCOLOR(0, 0, 0, -0),
							3,
							30);
					}

					CSound::Play(CSound::SOUND_LABEL_CHECKPOINT);
					CSound::ControlVoice(CSound::SOUND_LABEL_CHECKPOINT, 2);

					pScene->SetDeath(true);
				}
			}
		}
		pScene = pSceneNext;
	}
	m_bLandObject = bLand;
}

void CPlayer::Motion() //モーションの動作
{
	int nCntParts;
	m_nCntMotion++;
	for (nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{
		if (m_nCntMotion == 1) //差分を求める(対応するキーの各パーツのpos(rot) - 今の各パーツのpos(rot))
		{
			/*if (nCntParts != g_player.nMoveParts)
			{*/
			m_aKeyDiff[nCntParts].fPosX = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].fPosX - m_pModel[nCntParts]->GetPos().x;
			m_aKeyDiff[nCntParts].fPosY = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].fPosY - m_pModel[nCntParts]->GetPos().y;
			m_aKeyDiff[nCntParts].fPosZ = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].fPosZ - m_pModel[nCntParts]->GetPos().z;
			//}
			//else
			//{
			//	g_player.KeyMove.fPosX = sinf(D3DX_PI + g_player.rot.y) * g_player.MotionInfo.aKeyInfo[g_player.nKey].fMove;
			//	g_player.KeyMove.fPosY = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntParts].fPosY;
			//	g_player.KeyMove.fPosZ = cosf(D3DX_PI + g_player.rot.y) * g_player.MotionInfo.aKeyInfo[g_player.nKey].fMove;
			//}

			//角度が狂わないようにする
			m_aKeyDiff[nCntParts].fRotX = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].fRotX - m_pModel[nCntParts]->GetRot().x;
			if (m_aKeyDiff[nCntParts].fRotX < -D3DX_PI)
			{
				m_aKeyDiff[nCntParts].fRotX += D3DX_PI * 2;
			}
			else if (m_aKeyDiff[nCntParts].fRotX > D3DX_PI)
			{
				m_aKeyDiff[nCntParts].fRotX -= D3DX_PI * 2;
			}
			m_aKeyDiff[nCntParts].fRotY = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].fRotY - m_pModel[nCntParts]->GetRot().y;
			if (m_aKeyDiff[nCntParts].fRotY < -D3DX_PI)
			{
				m_aKeyDiff[nCntParts].fRotY += D3DX_PI * 2;
			}
			else if (m_aKeyDiff[nCntParts].fRotY > D3DX_PI)
			{
				m_aKeyDiff[nCntParts].fRotY -= D3DX_PI * 2;
			}
			m_aKeyDiff[nCntParts].fRotZ = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].fRotZ - m_pModel[nCntParts]->GetRot().z;
			if (m_aKeyDiff[nCntParts].fRotZ < -D3DX_PI)
			{
				m_aKeyDiff[nCntParts].fRotZ += D3DX_PI * 2;
			}
			else if (m_aKeyDiff[nCntParts].fRotZ > D3DX_PI)
			{
				m_aKeyDiff[nCntParts].fRotZ -= D3DX_PI * 2;
			}
		}
		//if (nCntParts != g_player.nMoveParts)
		//{

		//posを徐々に足していく
		D3DXVECTOR3 posOld;
		posOld = m_pModel[nCntParts]->GetPos();
		posOld.x += m_aKeyDiff[nCntParts].fPosX / m_MotionInfo.aKeyInfo[m_nKey].nFrame;
		posOld.y += m_aKeyDiff[nCntParts].fPosY / m_MotionInfo.aKeyInfo[m_nKey].nFrame;
		posOld.z += m_aKeyDiff[nCntParts].fPosZ / m_MotionInfo.aKeyInfo[m_nKey].nFrame;
		m_pModel[nCntParts]->SetPos(posOld);
		//}
		//else
		//{
		//	if (g_player.KeyMove.fPosX != 0.0f)
		//	{
		//		g_player.move.x = g_player.KeyMove.fPosX;
		//	}
		//	//g_player.pos.y += g_player.KeyMove.fPosY / g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame;
		//	if (g_player.KeyMove.fPosZ != 0.0f)
		//	{
		//		g_player.move.z = g_player.KeyMove.fPosZ;
		//	}
		//}

		//rotを徐々に足していく
		D3DXVECTOR3 rotOld;
		rotOld = m_pModel[nCntParts]->GetRot();
		rotOld.x += m_aKeyDiff[nCntParts].fRotX / m_MotionInfo.aKeyInfo[m_nKey].nFrame;
		rotOld.y += m_aKeyDiff[nCntParts].fRotY / m_MotionInfo.aKeyInfo[m_nKey].nFrame;
		rotOld.z += m_aKeyDiff[nCntParts].fRotZ / m_MotionInfo.aKeyInfo[m_nKey].nFrame;
		m_pModel[nCntParts]->SetRot(rotOld);
	}

	//現在のキーの時間がそのモーションのキーの時間と一致
	if (m_nCntMotion == m_MotionInfo.aKeyInfo[m_nKey].nFrame)
	{
		//現在のキーの時間リセット、キーをプラス
		m_nCntMotion = 0;
		m_nKey++;

		//キーが現在のモーションの最大キーと一致
		if (m_nKey == m_MotionInfo.nNumKey)
		{
			//ループしない
			if (m_MotionInfo.bLoop == false)
			{
				//ロープ掴んで移動中じゃない
				if (m_bSlackMove == false)
				{
					//リターンモーション以外
					if (m_motionType != MOTIONTYPE_RETURN)
					{
						//リターンモーション
						MotionChange(MOTIONTYPE_RETURN);
					}
					//リターンモーション中
					else
					{
						//ニュートラル
						MotionChange(MOTIONTYPE_NEUTRAL);
					}
				}
				//ロープ掴んで移動中じゃない
				else
				{
					//リターンモーション以外
					if (m_motionType != MOTIONTYPE_ZIPRETURN)
					{
						//リターンモーション
						MotionChange(MOTIONTYPE_ZIPRETURN);
					}
					//リターンモーション中
					else
					{
						//ニュートラル
						MotionChange(MOTIONTYPE_ZIPNUETRAL);
					}
				}
			}
			m_nKey = 0;
		}
	}
}

//モーション変更
void CPlayer::MotionChange(MOTIONTYPE motionType)
{
	m_motionType = motionType;
	m_MotionInfo = m_aMotionInfo[motionType];
	m_nCntMotion = 0;
	m_nKey = 0;
}

//地面との当たり判定
bool CPlayer::Land(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld)
{
	CScene *pScene;
	pScene = GetScene(0);
	while (pScene)
	{
		CScene *pSceneNext;
		pSceneNext = pScene->GetNext();
		//床
		if (pScene->GetObjType() == CScene::OBJECTTYPE_FIELD)
		{
			D3DXVECTOR3 pos = pScene->GetPos();
			D3DXVECTOR3 VtxMax = pScene->GetVtxMax();
			D3DXVECTOR3 VtxMin = pScene->GetVtxMin();
			
			//XZ方向に当たってる
			if (pPos->x - GetRadius() <= pos.x + VtxMax.x && pos.x + VtxMin.x <= pPos->x + GetRadius() &&
				pPos->z - GetRadius() <= pos.z + VtxMax.z && pos.z + VtxMin.z <= pPos->z + GetRadius())
			{
				//プレイヤーがいる位置の地面の高さ計算
				float fRotX = atan2f(VtxMax.y - VtxMin.y, VtxMax.z - VtxMin.z);
				float fFieldY = (pPos->z - (pos.z + VtxMin.z * 0.75f)) * tanf(fRotX) + pos.y + VtxMin.y * 0.75f;
				
				//上記の計算結果と大体一致している
				if (posOld.y + 3.0f >= fFieldY && fFieldY >= pPos->y - 3.0f)
				{
					//着地させる、影を移動させる
					m_move.y = 0.0f;
					pPos->y = fFieldY;
					m_pShadow->MoveY(*pPos, -fRotX);

					//着地した瞬間
					if (m_bLanding == false)
					{
						CSound::Play(CSound::SOUND_LABEL_LANDING);
						//ジャンプパーティクル
						for (int nCnt = 0; nCnt < 10; nCnt++)
						{
							CVecEffect3D::Create(D3DXVECTOR3(pPos->x, pPos->y + 10, pPos->z),
								D3DXVECTOR3(3.0f, 3.0f, 0.0f),
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								D3DXVECTOR3(sinf(CIRCLE) * 1, 0.0f, cosf(CIRCLE) * 1),
								D3DXCOLOR(255, 255, 0, 255),
								D3DXCOLOR(0, 0, 0, -10),
								3,
								30);
						}
						MotionChange(MOTIONTYPE_LANDING);
						m_bLanding = true;
					}
					return true;
				}

			}
		}
		pScene = pSceneNext;
	}
	return false;
}

//真下にあるオブジェクトや床を探す
void CPlayer::UnderSearch(D3DXVECTOR3 pos)
{
	CScene *pScene;
	CScene *pUnderScene = NULL;
	int nCntScene;
	for (nCntScene = 0; nCntScene < 7; nCntScene++)
	{
		pScene = GetScene(nCntScene);
		float fPosY = -10000.0f;
		while (pScene)
		{
			CScene *pSceneNext;
			pSceneNext = pScene->GetNext();
			//床かオブジェクト
			if (pScene->GetObjType() == CScene::OBJECTTYPE_FIELD || pScene->GetObjType() == CScene::OBJECTTYPE_BLOCK)
			{
				D3DXVECTOR3 Scenepos = pScene->GetPos();
				D3DXVECTOR3 VtxMax = pScene->GetVtxMax();
				D3DXVECTOR3 VtxMin = pScene->GetVtxMin();
				//XZ方向に当たってる
				if (pos.x - GetRadius() < Scenepos.x + VtxMax.x && Scenepos.x + VtxMin.x < pos.x + GetRadius() &&
					pos.z - GetRadius() < Scenepos.z + VtxMax.z && Scenepos.z + VtxMin.z < pos.z + GetRadius())
				{
					//自分の位置がものの一番下の位置より高く、現在の影の位置より低い
					if (pos.y >= Scenepos.y + VtxMin.y && fPosY <= Scenepos.y + VtxMin.y)
					{
						//影の位置入れ替え、もののポインタ取得
						fPosY = Scenepos.y + VtxMin.y;
						pUnderScene = pScene;
					}
				}
			}
			pScene = pSceneNext;
		}
	}

	//ポインタがNULLじゃない
	if (pUnderScene != NULL)
	{
		D3DXVECTOR3 Scenepos = pUnderScene->GetPos();
		D3DXVECTOR3 VtxMax = pUnderScene->GetVtxMax();
		D3DXVECTOR3 VtxMin = pUnderScene->GetVtxMin();
		float fRotX;
		float fFieldY;
		//地面の場合
		if (pUnderScene->GetObjType() == CScene::OBJECTTYPE_FIELD)
		{
			//角度を計算、その地点の地面の高さを計算
			fRotX = atan2f(VtxMax.y - VtxMin.y, VtxMax.z - VtxMin.z);
			fFieldY = (pos.z - (Scenepos.z + VtxMin.z * 0.75f)) * tanf(fRotX) + Scenepos.y + VtxMin.y * 0.75f;
			if (pos.y < fFieldY)
			{
				fFieldY = -100000.0f;
			}
		}
		//それ以外(=オブジェクトの場合)
		else
		{
			//そのものの最大の高さ
			fRotX = 0.0f;
			fFieldY = Scenepos.y + VtxMax.y;
		}
		//影を傾けながら計算した高さにする
		m_pShadow->MoveY(D3DXVECTOR3(pos.x, fFieldY, pos.z), -fRotX);
	}
	else
	{
		//適当な位置にする
		m_pShadow->MoveY(D3DXVECTOR3(pos.x, -10000.0f, pos.z), D3DX_PI);
	}
}

//作成
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *aFileName)
{
	CPlayer *pPlayer;
	pPlayer = new CPlayer(3);
	if (pPlayer != NULL)
	{
		pPlayer->Init(pos, rot, aFileName);
	}
	return pPlayer;
}