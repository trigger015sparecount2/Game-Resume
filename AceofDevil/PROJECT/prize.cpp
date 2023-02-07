//=============================================================================
//
// プライズ処理 [prize.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "prize.h"
#include "renderer.h"
#include "pauseui.h"
#include "player.h"
#include "sound.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CPrize::m_pTexture = NULL;

CPrize::CPrize(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

CPrize::~CPrize(void)
{

}

//初期化
HRESULT CPrize::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, ELEMENT element, int nHeal)
{
	CScene2D::Init(PRIZE_SIZE, PRIZE_SIZE, pos, 1.0f);
	CScene::SetObjType(CScene::OBJTYPE_PRIZE);
	m_move = move;
	m_nHeal = nHeal;
	m_fSize = PRIZE_SIZE;
	m_pPos = GetPlayerPos();
	m_bUninit = false;
	m_nTime = 0;
	m_Element = element;

	//属性によって色替え
	switch (element)
	{
	case ELEMENT_FIRE:
		ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.7f));
		break;
	case ELEMENT_ICE:
		ChangeColor(D3DXCOLOR(0.5f, 0.5f, 1.0f, 0.7f));
		break;
	case ELEMENT_THUNDER:
		ChangeColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.7f));
		break;
	case ELEMENT_WATER:
		ChangeColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.7f));
		break;
	case ELEMENT_WIND:
		ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.7f));
		break;
	default:
		break;
	}

	//方向はランダム
	m_fRotY = (float)(rand() % 628) / 100.0f - D3DX_PI;
	return S_OK;
}

//終了
void CPrize::Uninit(void)
{
	CScene2D::Uninit();
}

//更新
void CPrize::Update(void)
{
	if (CPauseUi::GetPause() == false)
	{
		D3DXVECTOR3 pos;
		pos = GetPos();
		//消えない
		if (m_bUninit == false)
		{
			//出てきてすぐ、ランダムの方向に飛ばす
			if (m_nTime < 15)
			{
				m_nTime++;
				pos.x += 4.0f * sinf(m_fRotY);
				pos.y += 4.0f * cosf(m_fRotY);
				Set(m_fSize, m_fSize, pos);
			}
			//少し移動したのち、停止
			else if (m_nTime < 25)
			{
				m_nTime++;
				if (m_nTime >= 25)
				{
					CSound::Play(CSound::SOUND_LABEL_PRIZE);
				}
			}
			//少し停止、プレイヤーの方向に移動しつつ小さくする
			else
			{
				float fDistance;
				m_fRotY = atan2f(m_pPos->x - pos.x, pos.y - m_pPos->y);
				pos.x += m_move.x * sinf(m_fRotY);
				pos.y -= m_move.y * cosf(m_fRotY);
				fDistance = sqrtf(powf(m_pPos->x - pos.x, 2.0f) + powf(m_pPos->y - pos.y, 2.0f));
				if (fDistance < 180.0f)
				{
					m_fSize -= 0.9f;
				}
				Set(m_fSize, m_fSize, pos);

				//小さくなると消える
				if (m_fSize <= 0.0f)
				{
					m_bUninit = true;
				}
			}
		}
		else
		{
			//プレイヤーの属性の魔力を回復させる
			switch (m_Element)
			{
			case ELEMENT_FIRE:
				CPlayer::GetPrize(0, m_nHeal);
				break;
			case ELEMENT_ICE:
				CPlayer::GetPrize(1, m_nHeal);
				break;
			case ELEMENT_THUNDER:
				CPlayer::GetPrize(2, m_nHeal);
				break;
			case ELEMENT_WATER:
				CPlayer::GetPrize(3, m_nHeal);
				break;
			case ELEMENT_WIND:
				CPlayer::GetPrize(4, m_nHeal);
				break;
			default:
				break;
			}
			Uninit();
		}
	}
}

//描画
void CPrize::Draw(void)
{
	CScene2D::Draw();
}

//作成
CPrize *CPrize::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, ELEMENT element, int nHeal)
{
	CPrize *pPrize;
	pPrize = new CPrize(PRIORITY_BG);
	if (pPrize != NULL)
	{
		pPrize->Init(pos, move, element, nHeal);
		pPrize->BindTexture(m_pTexture);
	}
	return pPrize;
}

//テクスチャ読み込み
HRESULT CPrize::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/particle.png",
		&m_pTexture);
	return S_OK;
}

//テクスチャ破棄
void CPrize::UnLoad(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}