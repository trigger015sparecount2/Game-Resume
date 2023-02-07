//=============================================================================
// セーブ管理[save.cpp]
// Author : 三上航世
//=============================================================================
//*****************************************************************************
//インクルード
//*****************************************************************************
#if 1
#include "save.h"
#include "keyboard.h"
#include "ui.h"
#include "manager.h"
#include "renderer.h"

#include "game.h"
#include "gamepad.h"
#include "fade.h"

#include "sound.h"
#endif

//*****************************************************************************
//静的
//*****************************************************************************
//int CTitle::m_SerectNam = 1;

#if 1
//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CSave::CSave(PRIORITY Priority) : CScene(Priority)
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CSave::~CSave()
{
}
#endif

//*****************************************************************************
// 初期化
//***************************************************************************** 
HRESULT CSave::Init(D3DXVECTOR3 /*pos*/)
{
	m_pKeyboard = CManager::GetKeyboard();
	m_pGamePad = CManager::GetGamepad();
	Load();
	CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), -1, D3DXCOLOR(0.0f, 0.3f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 50.0f, 0.0f), D3DXVECTOR2(110.0f, 50.0f), 20, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	
	//1番目
	m_pUI[0] = CUI::Create(D3DXVECTOR3(370.0f, 210.0f, 0.0f), D3DXVECTOR2(25.0f, 80.0f), 25, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[1] = CUI::Create(D3DXVECTOR3(910.0f, 210.0f, 0.0f), D3DXVECTOR2(25.0f, 80.0f), 25, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pUI[2] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 210.0f, 0.0f), D3DXVECTOR2(365.0f, 80.0f), 26, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[3] = CUI::Create(D3DXVECTOR3(405.0f, 210.0f, 0.0f), D3DXVECTOR2(34.0f, 60.0f), 3, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[4] = CUI::Create(D3DXVECTOR3(470.0f, 220.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), m_nTexAircraft[0], D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[5] = CUI::Create(D3DXVECTOR3(550.0f, 220.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), m_nTexAircraft[1], D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[6] = CUI::Create(D3DXVECTOR3(630.0f, 220.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), m_nTexAircraft[2], D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[7] = CUI::Create(D3DXVECTOR3(710.0f, 220.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), m_nTexAircraft[3], D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[8] = CUI::Create(D3DXVECTOR3(790.0f, 220.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), m_nTexAircraft[4], D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[9] = CUI::Create(D3DXVECTOR3(870.0f, 220.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), m_nTexAircraft[5], D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//2番目
	m_pUI[10] = CUI::Create(D3DXVECTOR3(370.0f, 330.0f, 0.0f), D3DXVECTOR2(20.0f, 80.0f), 25, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[11] = CUI::Create(D3DXVECTOR3(910.0f, 330.0f, 0.0f), D3DXVECTOR2(20.0f, 80.0f), 25, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pUI[12] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 330.0f, 0.0f), D3DXVECTOR2(365.0f, 80.0f), 26, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[13] = CUI::Create(D3DXVECTOR3(405.0f, 330.0f, 0.0f), D3DXVECTOR2(34.0f, 60.0f), 3, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[14] = CUI::Create(D3DXVECTOR3(470.0f, 340.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), m_nTexAircraft[0], D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[15] = CUI::Create(D3DXVECTOR3(550.0f, 340.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), m_nTexAircraft[1], D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[16] = CUI::Create(D3DXVECTOR3(630.0f, 340.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), m_nTexAircraft[2], D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[17] = CUI::Create(D3DXVECTOR3(710.0f, 340.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), m_nTexAircraft[3], D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[18] = CUI::Create(D3DXVECTOR3(790.0f, 340.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), m_nTexAircraft[4], D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[19] = CUI::Create(D3DXVECTOR3(870.0f, 340.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), m_nTexAircraft[5], D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//3番目
	m_pUI[20] = CUI::Create(D3DXVECTOR3(370.0f, 450.0f, 0.0f), D3DXVECTOR2(20.0f, 80.0f), 25, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[21] = CUI::Create(D3DXVECTOR3(910.0f, 450.0f, 0.0f), D3DXVECTOR2(20.0f, 80.0f), 25, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pUI[22] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 450.0f, 0.0f), D3DXVECTOR2(365.0f, 80.0f), 26, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[23] = CUI::Create(D3DXVECTOR3(405.0f, 450.0f, 0.0f), D3DXVECTOR2(34.0f, 60.0f), 3, D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[24] = CUI::Create(D3DXVECTOR3(470.0f, 460.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), m_nTexAircraft[0], D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[25] = CUI::Create(D3DXVECTOR3(550.0f, 460.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), m_nTexAircraft[1], D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[26] = CUI::Create(D3DXVECTOR3(630.0f, 460.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), m_nTexAircraft[2], D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[27] = CUI::Create(D3DXVECTOR3(710.0f, 460.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), m_nTexAircraft[3], D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[28] = CUI::Create(D3DXVECTOR3(790.0f, 460.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), m_nTexAircraft[4], D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[29] = CUI::Create(D3DXVECTOR3(870.0f, 460.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), m_nTexAircraft[5], D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//画面下の説明文
	CUI::Create(D3DXVECTOR3(235.0f, 610.0f, 0.0f), D3DXVECTOR2(26.0f, 100.0f), 5, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CUI::Create(D3DXVECTOR3(1045.0f, 610.0f, 0.0f), D3DXVECTOR2(26.0f, 100.0f), 5, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 610.0f, 0.0f), D3DXVECTOR2(550.0f, 100.0f), 6, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pUI[3]->SetTex(1, 0.1f);
	m_pUI[13]->SetTex(2, 0.1f);
	m_pUI[23]->SetTex(3, 0.1f);
	m_nClear[0] = 255;
	m_nClear[1] = 180;
	m_nClear[2] = 180;

	int nCntGroup;
	int nCntAir;
	for (nCntGroup = 0; nCntGroup < 3; nCntGroup++)
	{
		if (m_aData[nCntGroup] == true)
		{
			D3DXCreateFont(CManager::GetRenderer()->CRenderer::GetDevice(), 26, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
				OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "PixelMplus10", &m_pFont[nCntGroup]);
			for (nCntAir = 0; nCntAir < 6; nCntAir++)
			{
				if (m_aAircraft[nCntAir][nCntGroup] == false)
				{
					m_pUI[nCntGroup * 10 + nCntAir + 4]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
				}
			}
		}
		else
		{
			D3DXCreateFont(CManager::GetRenderer()->CRenderer::GetDevice(), 90, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
				OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "PixelMplus10", &m_pFont[nCntGroup]);
			for (nCntAir = 0; nCntAir < 6; nCntAir++)
			{
				m_pUI[nCntGroup * 10 + nCntAir + 4]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			}
		}
	}
	D3DXCreateFont(CManager::GetRenderer()->CRenderer::GetDevice(), 30, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "PixelMplus10", &m_pFont[3]);

	CManager::SetPause(false);
	m_nSelect = 0;
	m_bSave = false;
	m_bClear = false;
	m_nCntChange = 0;
	TextChange(TEXTTYPE_NONE);
	return S_OK;
}

//*****************************************************************************
// 終了
//***************************************************************************** 
void CSave::Uninit()
{
	int nCnt;
	if (m_pGamePad != NULL)
	{
		m_pGamePad = NULL;
	}
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard = NULL;
	}
	for (nCnt = 0; nCnt < 30; nCnt++)
	{
		if (m_pUI[nCnt] != NULL)
		{
			m_pUI[nCnt] = NULL;
		}
	}
	for (nCnt = 0; nCnt < 4; nCnt++)
	{
		if (m_pFont[nCnt] != NULL)
		{
			m_pFont[nCnt]->Release();
			m_pFont[nCnt] = NULL;
		}
	}
	CScene::Release();
}

//*****************************************************************************
// 更新
//***************************************************************************** 
void CSave::Update()
{
	//テキスト変更カウント進める。進んだらテキストを元に戻す
	if (m_nCntChange > 0)
	{
		m_nCntChange--;
		if (m_nCntChange <= 0)
		{
			m_nCntChange = 0;
			TextChange(TEXTTYPE_NONE);
		}
	}
	//ゲームパッド繋がっているか
	if (m_pGamePad != NULL)
	{
		//選択
		if (m_pGamePad->TriggerCrossKey() == 0.0f)
		{
			Select(-1);
		}
		else if (m_pGamePad->TriggerCrossKey() == 18000.0f)
		{
			Select(1);
		}

		//決定
		if (m_pGamePad->GetButton(CGamePad::DIP_B) == true)
		{
			//フェード動いてない
			if (CFade::GetFade() == CFade::FADE_NONE)
			{
				//セーブもデータ消去もしない
				if (m_bSave == false && m_bClear == false)
				{
					//セーブする？
					m_bSave = true;
					TextChange(TEXTTYPE_SAVE);
					if (m_nCntChange > 0)
					{
						m_nCntChange = 0;
					}
					CSound::Play(CSound::SOUND_LABEL_YES);
				}
				//データ消去する
				else if (m_bClear == true)
				{
					DataReset();
					m_bClear = false;
				}
				else
				{
					//セーブする
					Save();
				}
			}
		}

		//キャンセル
		if (m_pGamePad->GetButton(CGamePad::DIP_A) == true)
		{
			//データ消去する場合、キャンセルする
			if (m_bClear == true)
			{
				m_bClear = false;
				TextChange(TEXTTYPE_NONE);
				CSound::Play(CSound::SOUND_LABEL_CANCEL);
			}
			//セーブする場合、キャンセルする
			else if (m_bSave == true)
			{
				m_bSave = false;
				TextChange(TEXTTYPE_NONE);
				CSound::Play(CSound::SOUND_LABEL_CANCEL);
			}
			//タイトルに戻る
			else if (CFade::GetFade() == CFade::FADE_NONE)
			{
				CFade::SetFade(CManager::MODE_TITLE);
				CSound::Play(CSound::SOUND_LABEL_CANCEL);
			}
		}

		//消去
		if (m_pGamePad->GetButton(CGamePad::DIP_Y) == true)
		{
			//消去もセーブもしない
			if (m_bClear == false && m_bSave == false)
			{
				//消去しようとする
				if (m_nCntChange > 0)
				{
					m_nCntChange = 0;
				}
				TextChange(TEXTTYPE_CLEAR);
				m_bClear = true;
				CSound::Play(CSound::SOUND_LABEL_YES);
			}
		}
	}
	//キーボードが繋がってる
	else if (m_pKeyboard != NULL)
	{
		//選択
		if (m_pKeyboard->GetKey(DIK_W) == true)
		{
			Select(-1);
		}
		else if (m_pKeyboard->GetKey(DIK_S) == true)
		{
			Select(1);
		}

		//決定
		if (m_pKeyboard->GetKey(DIK_J) == true)
		{
			//セーブも消去もしない
			if (m_bSave == false && m_bClear == false)
			{
				//セーブする
				TextChange(TEXTTYPE_SAVE);
				m_bSave = true;
				if (m_nCntChange > 0)
				{
					m_nCntChange = 0;
				}
				CSound::Play(CSound::SOUND_LABEL_YES);
			}
			//データ消去する
			else if (m_bClear == true)
			{
				DataReset();
				m_bClear = false;
			}
			//セーブする
			else
			{
				Save();
			}
		}

		//キャンセル
		if (m_pKeyboard->GetKey(DIK_I) == true)
		{
			//データ消去する場合、キャンセルする
			if (m_bClear == true)
			{
				CSound::Play(CSound::SOUND_LABEL_CANCEL);
				m_bClear = false;
				TextChange(TEXTTYPE_NONE);
			}
			//セーブする場合、キャンセルする
			else if (m_bSave == true)
			{
				CSound::Play(CSound::SOUND_LABEL_CANCEL);
				m_bSave = false;
				TextChange(TEXTTYPE_NONE);
			}
			else if(CFade::GetFade() == CFade::FADE_NONE)
			{
				//タイトルに戻る
				CSound::Play(CSound::SOUND_LABEL_CANCEL);
				CFade::SetFade(CManager::MODE_TITLE);
			}
		}

		//消去
		if (m_pKeyboard->GetKey(DIK_DELETE) == true)
		{
			//消去もセーブもしない
			if (m_bClear == false && m_bSave == false)
			{
				//消去する？
				if (m_nCntChange > 0)
				{
					m_nCntChange = 0;
				}
				TextChange(TEXTTYPE_CLEAR);
				m_bClear = true;
				CSound::Play(CSound::SOUND_LABEL_YES);
			}
		}
	}
}

//*****************************************************************************
//描画
//***************************************************************************** 
void CSave::Draw()
{
	RECT rect[4];
	rect[0] = { 450, 165, 900, SCREEN_HEIGHT - 20 };
	rect[1] = { 450, 285, 900, SCREEN_HEIGHT - 20 };
	rect[2] = { 450, 405, 900, SCREEN_HEIGHT - 20 };
	rect[3] = { 240, 555, SCREEN_WIDTH - 240, SCREEN_HEIGHT - 20 };

	char str[256];
	int nCntText;
	for (nCntText = 0; nCntText < 3; nCntText++)
	{
		memset(&str[0], 0, sizeof(str));
		if (m_aData[nCntText] == true)
		{
			wsprintf(&str[0], "MONEY:%6d       AIRCRAFT:%d\n", m_nMoney[nCntText], m_nNumAircraft[nCntText]);
		}
		else
		{
			wsprintf(&str[0], "NO DATA\n");
			rect[nCntText].left += 20;
		}

		// テキスト描画
		m_pFont[nCntText]->DrawText(NULL, str, -1, &rect[nCntText], DT_WORDBREAK, D3DCOLOR_ARGB(255, 0, m_nClear[nCntText], 0));
	}
	m_pFont[3]->DrawText(NULL, m_Str, -1, &rect[3], DT_WORDBREAK, D3DCOLOR_ARGB(255, 0, 255, 0));
}

//*****************************************************************************
//作成
//***************************************************************************** 
CSave *CSave::Create()
{
	CSave *pSelect = NULL;
	pSelect = new CSave(PRIORITY_PAUSEUI);		//メモリ確保
												//NULLチェック
	if (pSelect != NULL)
	{
		pSelect->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	return pSelect;
}

//ファイル読み込み
void CSave::Load()
{
	//ファイル読み込み
	char aFile[256];
	FILE *pFile;
	int nCntLoad;
	int nAircraft;
	int nCntAir = 0;
	bool bStatus = false;
	for (nCntLoad = 0; nCntLoad < 3; nCntLoad++)
	{
		m_aData[nCntLoad] = true;
		switch (nCntLoad)
		{
		case 0:
			pFile = fopen(FILE_SAVE1, "r");
			break;
		case 1:
			pFile = fopen(FILE_SAVE2, "r");
			break;
		case 2:
			pFile = fopen(FILE_SAVE3, "r");
			break;
		default:
			pFile = fopen(FILE_SAVE1, "r");
			break;
		}
		if (pFile != NULL)
		{
			while (true)
			{
				fscanf(pFile, "%s", &aFile[0]);
				if (strcmp(&aFile[0], "NO_DATA") == 0) //NO_DATAの文字列
				{
					m_aData[nCntLoad] = false;
					break;
				}
				if (strcmp(&aFile[0], "MANEY") == 0) //MANEYの文字列
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_nMoney[nCntLoad]);
				}
				if (strcmp(&aFile[0], "NUM_AIRCRAFT") == 0) //NUM_AIRCRAFTの文字列
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_nNumAircraft[nCntLoad]);
				}
				if (strcmp(&aFile[0], "AIRCRAFT") == 0) //AIRCRAFTの文字列
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nAircraft);
					if (nAircraft == -1)
					{
						m_aAircraft[nCntAir][nCntLoad] = false;
					}
					else
					{
						m_aAircraft[nCntAir][nCntLoad] = true;
					}
					nCntAir++;
				}
				if (strcmp(&aFile[0], "END_SCRIPT") == 0) //END_SCRIPTの文字列を見つけたら
				{
					nCntAir = 0;
					break;
				}
			}
			fclose(pFile);
		}
	}
	nCntAir = 0;
	pFile = fopen(FILE_AIRCRAFT, "r");
	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]);
			if (strcmp(&aFile[0], "STATUS") == 0) //STATUSの文字列
			{
				bStatus = true;
			}
			if (strcmp(&aFile[0], "END_STATUS") == 0) //END_STATUSの文字列
			{
				bStatus = false;
				nCntAir++;
			}
			if (bStatus == true)
			{
				if (strcmp(&aFile[0], "HUD_TEXTYPE") == 0) //HUD_TEXTYPEの文字列
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_nTexAircraft[nCntAir]);
				}
			}
			if (strcmp(&aFile[0], "END_SCRIPT") == 0) //END_SCRIPTの文字列を見つけたら
			{
				break;
			}
		}
		fclose(pFile);
	}
}

//セーブ
void CSave::Save()
{
	FILE *pFile;
	switch (m_nSelect)
	{
	case 0:
		pFile = fopen(FILE_SAVE1, "w");
		break;
	case 1:
		pFile = fopen(FILE_SAVE2, "w");
		break;
	case 2:
		pFile = fopen(FILE_SAVE3, "w");
		break;
	default:
		pFile = fopen(FILE_SAVE1, "w");
		break;
	}
	int nCntAircraft;
	if (pFile != NULL)
	{
		fprintf(pFile, "MANEY = %d\n", CManager::GetMoney());
		fprintf(pFile, "NUM_AIRCRAFT = %d\n", CManager::GetNumAircraft());
		for (nCntAircraft = 0; nCntAircraft < 6; nCntAircraft++)
		{
			if (CManager::GetAircraft(nCntAircraft) == true)
			{
				fprintf(pFile, "AIRCRAFT = 0\n");
			}
			else
			{
				fprintf(pFile, "AIRCRAFT = -1\n");
			}
		}
		fprintf(pFile, "\nEND_SCRIPT");
		fclose(pFile);
		m_bSave = false;
		TextChange(TEXTTYPE_SAVED);
		int nCntAir;
		m_nMoney[m_nSelect] = CManager::GetMoney();
		m_nNumAircraft[m_nSelect] = CManager::GetNumAircraft();
		m_pFont[m_nSelect]->Release();
		D3DXCreateFont(CManager::GetRenderer()->CRenderer::GetDevice(), 26, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "PixelMplus10", &m_pFont[m_nSelect]);
		for (nCntAir = 0; nCntAir < 6; nCntAir++)
		{
			m_aAircraft[nCntAir][m_nSelect] = CManager::GetAircraft(nCntAir);
			if (m_aAircraft[nCntAir][m_nSelect] == false)
			{
				m_pUI[m_nSelect * 10 + nCntAir + 4]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			}
			else
			{
				m_pUI[m_nSelect * 10 + nCntAir + 4]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
			}
		}
		m_aData[m_nSelect] = true;
	}
	else
	{
		TextChange(TEXTTYPE_SAVEFAILED);
	}
	m_nCntChange = 100;
	CSound::Play(CSound::SOUND_LABEL_YES);
}

//データ消去
void CSave::DataReset()
{
	FILE *pFile;
	switch (m_nSelect)
	{
	case 0:
		pFile = fopen(FILE_SAVE1, "w");
		break;
	case 1:
		pFile = fopen(FILE_SAVE2, "w");
		break;
	case 2:
		pFile = fopen(FILE_SAVE3, "w");
		break;
	default:
		pFile = fopen(FILE_SAVE1, "w");
		break;
	}
	if (pFile != NULL)
	{
		int nCntAir;
		fprintf(pFile, "NO_DATA");
		fclose(pFile);
		m_pFont[m_nSelect]->Release();
		D3DXCreateFont(CManager::GetRenderer()->CRenderer::GetDevice(), 90, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "PixelMplus10", &m_pFont[m_nSelect]);
		for (nCntAir = 0; nCntAir < 6; nCntAir++)
		{
			m_aAircraft[nCntAir][m_nSelect] = false;
			m_pUI[m_nSelect * 10 + nCntAir + 4]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		}
		TextChange(TEXTTYPE_CLEARED);
		m_aData[m_nSelect] = false;
	}
	else
	{
		TextChange(TEXTTYPE_CLEARFAILED);
	}
	m_nCntChange = 100;
	CSound::Play(CSound::SOUND_LABEL_YES);
}

//データ選択
void CSave::Select(int nAdd)
{
	if (m_bClear == false && m_bSave == false)
	{
		int nCntSelect;
		for (nCntSelect = 0; nCntSelect < 10; nCntSelect++)
		{
			if (nCntSelect > 3)
			{
				if (m_aAircraft[nCntSelect - 4][m_nSelect] == true)
				{
					m_pUI[m_nSelect * 10 + nCntSelect]->ColorChange(D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f));
				}
			}
			else
			{
				m_pUI[m_nSelect * 10 + nCntSelect]->ColorChange(D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f));
			}
		}
		m_nClear[m_nSelect] = 180;
		m_nSelect += nAdd;
		if (m_nSelect < 0)
		{
			m_nSelect = 2;
		}
		else if (m_nSelect > 2)
		{
			m_nSelect = 0;
		}
		for (nCntSelect = 0; nCntSelect < 10; nCntSelect++)
		{
			if (nCntSelect > 3)
			{
				if (m_aAircraft[nCntSelect - 4][m_nSelect] == true)
				{
					m_pUI[m_nSelect * 10 + nCntSelect]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
				}
			}
			else
			{
				m_pUI[m_nSelect * 10 + nCntSelect]->ColorChange(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
			}
		}
		m_nClear[m_nSelect] = 255;
	}
	CSound::Play(CSound::SOUND_LABEL_CHOICE);
}

//文章変更
void CSave::TextChange(TEXTTYPE Texttype)
{
	memset(&m_Str[0], 0, sizeof(m_Str));
	switch (Texttype)
	{
	case TEXTTYPE_NONE:
		wsprintf(&m_Str[0], "セーブするファイルを選んでください\n");
		if (m_pGamePad != NULL)
		{
			wsprintf(&m_Str[strlen(m_Str)], "選択:方向キー   決定:B   削除:Y   タイトルに戻る:A\n");
		}
		else
		{
			wsprintf(&m_Str[strlen(m_Str)], "選択:W,S   決定:J   削除:DEL   タイトルに戻る:I\n");
		}
		break;
	case TEXTTYPE_SAVE:
		wsprintf(&m_Str[0], "本当にこのファイルにセーブしますか？\n");
		wsprintf(&m_Str[strlen(m_Str)], "決定:J   キャンセル:I\n");
		break;
	case TEXTTYPE_SAVED:
		wsprintf(&m_Str[0], "ファイルにセーブしました\n");
		break;
	case TEXTTYPE_SAVEFAILED:
		wsprintf(&m_Str[0], "ファイルのセーブに失敗しました・・・\n");
		break;
	case TEXTTYPE_CLEAR:
		wsprintf(&m_Str[0], "本当にこのファイルを削除しますか？\n");
		if (m_pGamePad != NULL)
		{
			wsprintf(&m_Str[strlen(m_Str)], "決定:B   キャンセル:A\n");
		}
		else
		{
			wsprintf(&m_Str[strlen(m_Str)], "決定:J   キャンセル:I\n");
		}
		break;
	case TEXTTYPE_CLEARED:
		wsprintf(&m_Str[0], "ファイルを削除しました\n");
		break;
	case TEXTTYPE_CLEARFAILED:
		wsprintf(&m_Str[0], "ファイルの削除に失敗しました・・・\n");
		break;
	default:
		break;
	}
}