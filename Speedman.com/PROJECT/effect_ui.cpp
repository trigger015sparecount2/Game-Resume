//*****************************************************************************
// UI（エフェクト）関連処理(Effect_UI.cpp)
// Author : 佐藤秀亮
// 10/24 従来のui.cppが膨れ上がったので細分化しました
//*****************************************************************************
#include "Effect_UI.h"

#include "FallEffect.h"
#include "endeffect.h"

//*****************************************************************************
//マクロ
//*****************************************************************************
#define RAND_COOLTIME (int(rand() % 120) + 30)
#define RAND_POS (float(rand() % 400) - 200)
#define RAND_RANKPOS (float(rand() % 200) - 100)

#define RAND_COLOR (int(rand() % 255) + 1)

#define RAND_SCREENX (int(rand() % 1280) +1)
//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CEffectUI::CEffectUI(int nPriority) : CUI(nPriority)
{
	m_aEffectType = CEffectUI::EFFECT_NONE;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectUI::~CEffectUI()
{

}

//*****************************************************************************
//初期化
//*****************************************************************************
HRESULT CEffectUI::Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, CEffectUI::EFFECT_TYPE EffectType)
{
	CUI::Init(pos, SizeX, SizeY, nType);
	m_aEffectType = EffectType;
	m_pos = pos;
	m_cooltime = RAND_COOLTIME;
	return S_OK;
}

//*****************************************************************************
//終了
//*****************************************************************************
void CEffectUI::Uninit()
{
	CUI::Uninit();
}

//*****************************************************************************
//更新
//*****************************************************************************
void CEffectUI::Update()
{
	D3DXVECTOR3 randpos = D3DXVECTOR3(m_pos.x + RAND_POS, m_pos.y + RAND_POS, 0.0f);
	D3DXVECTOR3 randposx = D3DXVECTOR3(m_pos.x + RAND_SCREENX, m_pos.y, 0.0f);

	switch (m_aEffectType)
	{
	case(CEffectUI::EFFECT_FIRE):
		CFallEffect::Create(m_pos,
			D3DXVECTOR2(CIRCLE, 0.0f),
			D3DXCOLOR(255, 255, 0, 255),
			D3DXCOLOR(0, -3, 0, 0),
			D3DXVECTOR2(20.0f, 20.0f),
			D3DXVECTOR2(0.0f, 0.0f),
			100, 8,
			D3DXVECTOR2(0.0f, -0.04f));
		break;
	case(CEffectUI::EFFECT_CYCLE):
		m_cooltime--;
		if (m_cooltime <= 0)
		{
			CFallEffect::Create(randpos,
				D3DXVECTOR2(0.0f, 0.0f),
				D3DXCOLOR(255, 255, 255, 255),
				D3DXCOLOR(0, 0, 0, -6),
				D3DXVECTOR2(20.0f, 20.0f),
				D3DXVECTOR2(10.0f, 10.0f),
				120, 27,
				D3DXVECTOR2(0.0f, 0.0f));

			for (int nCnt = 0; nCnt < 50; nCnt++)
			{
				CFallEffect::Create(randpos,
					D3DXVECTOR2(CIRCLE * 3, CIRCLE * 3),
					D3DXCOLOR(RAND_COLOR, RAND_COLOR, RAND_COLOR, 255),
					D3DXCOLOR(0, 0, 0, -5),
					D3DXVECTOR2(20.0f, 20.0f),
					D3DXVECTOR2(0.0f, 0.0f),
					100, 8,
					D3DXVECTOR2(0.0f, 0.1f));
			}
			m_cooltime = RAND_COOLTIME;
		}
		break;
	case(CEffectUI::EFFECT_RANKCYCLE):
		m_cooltime--;
		if (m_cooltime <= 0)
		{
			CEndEffect::Create(D3DXVECTOR3(m_pos.x + (float(rand() % 200) - 100), m_pos.y, m_pos.z),
				D3DXVECTOR2(0.0f, -8.0f),
				D3DXCOLOR(255, 255, 255, 255),
				D3DXCOLOR(0, 0, 0, -6),
				D3DXVECTOR2(20.0f, 20.0f),
				60, 8,
				CEndEffect::TYPE_FIREWORK);

			m_cooltime = RAND_COOLTIME;
		}
		break;
	case(CEffectUI::EFFECT_PETAL):
		CFallEffect::Create(randposx,
			D3DXVECTOR2(0.0f, -8.0f),
			D3DXCOLOR(RAND_COLOR, RAND_COLOR, RAND_COLOR, 255.0f),
			D3DXCOLOR(0, 0, 0, 0),
			D3DXVECTOR2(20.0f, 20.0f),
			D3DXVECTOR2(0.0f, 0.0f),
			140, 8,
			D3DXVECTOR2(-0.02f, 0.04f));

		CFallEffect::Create(randposx,
			D3DXVECTOR2(0.0f, -8.0f),
			D3DXCOLOR(RAND_COLOR, RAND_COLOR, RAND_COLOR, 255.0f),
			D3DXCOLOR(0, 0, 0, 0),
			D3DXVECTOR2(20.0f, 20.0f),
			D3DXVECTOR2(0.0f, 0.0f),
			140, 8,
			D3DXVECTOR2(0.02f, 0.04f));

		m_cooltime--;
		if (m_cooltime <= 0)
		{
			CEndEffect::Create(randposx,
				D3DXVECTOR2(0.0f, -8.0f),
				D3DXCOLOR(255, 255, 255, 255),
				D3DXCOLOR(0, 0, 0, -8),
				D3DXVECTOR2(20.0f, 20.0f),
				60, 8,
				CEndEffect::TYPE_FIREWORK);

			m_cooltime = RAND_COOLTIME;
		}

		break;

	}
}

//*****************************************************************************
//描画
//*****************************************************************************
void CEffectUI::Draw()
{
	CUI::Draw();
}

//*****************************************************************************
//作成
//*****************************************************************************
CEffectUI *CEffectUI::Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, CEffectUI::EFFECT_TYPE EffectType)
{
	CEffectUI * pEffectUI = NULL;
	pEffectUI = new CEffectUI(1);
	if (pEffectUI != NULL)
	{
		pEffectUI->Init(pos, SizeX, SizeY, nType, EffectType);
	}
	return pEffectUI;
}