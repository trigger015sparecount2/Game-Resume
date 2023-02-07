//=============================================================================
//スタートUI管理[startui.cpp]
// Author : 三上　航世
//=============================================================================
//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "startui.h"
#include "manager.h"
#include "sound.h"

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CStartUI::CStartUI(PRIORITY Priority) : CUI::CUI(Priority)
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CStartUI::~CStartUI()
{

}

//*****************************************************************************
// 初期化
//***************************************************************************** 
HRESULT CStartUI::Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, D3DXCOLOR col, int nUIType)
{
	CUI::Init(pos, SizeX, SizeY, nType, col);
	size.x = SizeX;
	size.y = SizeY;
	m_nUIType = nUIType;
	m_nTime = 0;
	m_bStop = false;
	m_nFade = 255;
	if (nType == 0)
	{
		CSound::Play(CSound::SOUND_LABEL_READY);
	}
	return S_OK;
}

//*****************************************************************************
// 終了
//***************************************************************************** 
void CStartUI::Uninit()
{
	CUI::Uninit();
}

//*****************************************************************************
// 更新
//***************************************************************************** 
void CStartUI::Update()
{
	//UIタイプによって処理を変える
	switch (m_nUIType)
	{
	case 0:	//最初のREADY
		//止まったら
		if (m_bStop == true)
		{
			//止まってる間のカウント
			m_nTime++;

			//カウントが進んだら
			if (m_nTime > 50)
			{
				//次のパターンを生成し、自分は消える
				CStartUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f - 100.0f, 0.0f), 200.0f, 80.0f, 16, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1);
				SetDeath(true);
				CSound::Play(CSound::SOUND_LABEL_GO);
			}
		}
		else
		{
			//サイズを小さくしていく
			size.x -= 16.4f;
			size.y -= 4.5f;

			//一定以下のサイズになったら
			if (size.x < 200.0f || size.y < 50.0f)
			{
				size.x = 200.0f;
				size.y = 50.0f;
				m_bStop = true;
			}
			SetSize(size);
		}
		break;
	case 1:	//GO
		//カウント進める、カウントダウンフラグをfalseにする
		if (m_nTime <= 0)
		{
			m_nTime++;
			CManager::SetCountdown(false);
		}

		//大きくなるの早い
		if (m_bStop == false)
		{
			//徐々に大きくする
			size.x += 14.0f;
			size.y += 9.0f;

			//一定以上の大きさになったら
			if (size.x > 600.0f || size.y > 250.0f)
			{
				//大きくするの遅くする
				m_bStop = true;
			}
		}
		//大きくなるの遅い
		else
		{
			//徐々に大きくする(遅め)
			size.x += 3.0f;
			size.y += 1.7f;

			//徐々に色薄くする
			m_nFade -= 10;

			//透明になる
			if (m_nFade < 0)
			{
				//消す
				SetDeath(true);
			}
			//まだ色がある
			else
			{
				//フェードさせる
				FadeColorChange(m_nFade);
			}
		}
		//大きさ変更
		SetSize(size);
		break;
	default:
		break;
	}
}

//*****************************************************************************
//描画
//***************************************************************************** 
void CStartUI::Draw()
{
	CUI::Draw();
}

//*****************************************************************************
//作成
//***************************************************************************** 
CStartUI *CStartUI::Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, D3DXCOLOR col, int nUIType)
{
	CStartUI *pStartUI = NULL;
	pStartUI = new CStartUI(PRIORITY_UI);		//メモリ確保
													//NULLチェック
	if (pStartUI != NULL)
	{
		pStartUI->Init(pos, SizeX, SizeY, nType, col, nUIType);
	}

	return pStartUI;
}