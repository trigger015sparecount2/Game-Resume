//*****************************************************************************
// (終了時に何かある)エフェクト処理 [EndEffect.cpp]
// Author : 佐藤秀亮
//*****************************************************************************

#include "endeffect.h"
#include "FallEffect.h"
//*****************************************************************************
//マクロ
//*****************************************************************************
#define RAND_RANKPOS (float(rand() % 100) - 100)

#define RAND_COLOR (int(rand() % 255) + 1)

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CEndEffect::CEndEffect(int nPriority) : CEffect(nPriority)
{
	m_Size = D3DXVECTOR2(0.0f, 0.0f);

	m_nLife = 0;
	m_bUninit = false;
	m_move = D3DXVECTOR2(0.0f, 0.0f);
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CEndEffect::~CEndEffect()
{

}

//*****************************************************************************
//	初期化
//*****************************************************************************
HRESULT CEndEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR2 move, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, D3DXVECTOR2 Size, int nLife, int nType, CEndEffect::TYPE_ENDEFFECT atype)
{
	CEffect::Init(pos, color, Mincolor, Size, nLife, nType);
	m_Size = Size;
	m_nType = atype;

	m_nLife = nLife;
	m_move = move;
	return S_OK;
}

//*****************************************************************************
//終了
//*****************************************************************************
void CEndEffect::Uninit()
{
	CEffect::Uninit();
}

//*****************************************************************************
//更新
//*****************************************************************************
void CEndEffect::Update()
{
	CEffect::Update();

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 randrankpos = D3DXVECTOR3(pos.x + RAND_RANKPOS, pos.y + RAND_RANKPOS, 0.0f);

	pos += D3DXVECTOR3(m_move.x, m_move.y, 0.0f);

	SetPosition(pos);

	//サイズ変更（ある場合）
	SetWhidth(m_Size.x);
	SetHight(m_Size.y);

	m_nLife--;		//ライフ減算

					//画面外処理
	if (pos.y < 0)
	{
		m_bUninit = true;
	}
	else if (pos.y > SCREEN_HEIGHT)
	{
		m_bUninit = true;
	}
	if (pos.x < 0)
	{
		m_bUninit = true;
	}
	else if (pos.x > SCREEN_WIDTH)
	{
		m_bUninit = true;
	}


	//高さか幅が0になったら
	if (m_Size.x < 0 || m_Size.y < 0)
	{
		m_bUninit = true;
	}
	//寿命が0になったら
	if (m_nLife < 0)
	{
		m_bUninit = true;
		switch (m_nType)
		{
		case(CEndEffect::TYPE_FIREWORK):
			CFallEffect::Create(pos,
				D3DXVECTOR2(0.0f, 0.0f),
				D3DXCOLOR(255.0f, 255.0f, 255.0f, 255.0f),
				D3DXCOLOR(0, 0, 0, -6),
				D3DXVECTOR2(20.0f, 20.0f),
				D3DXVECTOR2(8.0f, 8.0f),
				120, 27,
				D3DXVECTOR2(0.0f, 0.0f));

			for (int nCnt = 0; nCnt < 35; nCnt++)
			{
				CFallEffect::Create(pos,
					D3DXVECTOR2(CIRCLE * 2, CIRCLE * 2),
					D3DXCOLOR(RAND_COLOR, RAND_COLOR, RAND_COLOR, 255.0f),
					D3DXCOLOR(0, 0, 0, -5),
					D3DXVECTOR2(20.0f, 20.0f),
					D3DXVECTOR2(0.0f, 0.0f),
					100, 8,
					D3DXVECTOR2(0.0f, 0.1f));
			}

			break;

		default:
			break;
		}

	}

	//破棄
	if (m_bUninit == true)
	{
		SetDeath(true);
	}
}

//*****************************************************************************
//描画
//*****************************************************************************
void CEndEffect::Draw()
{
	CEffect::Draw();
}

//*****************************************************************************
//作成
//*****************************************************************************
CEndEffect *CEndEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR2 move, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, D3DXVECTOR2 Size, int nLife, int nType, CEndEffect::TYPE_ENDEFFECT atype)
{
	CEndEffect *pEndEffect = NULL;
	pEndEffect = new CEndEffect(1);		//メモリ確保
										//NULLチェック
	if (pEndEffect != NULL)
	{
		pEndEffect->Init(pos, move, color, Mincolor, Size, nLife, nType, atype);
	}

	return pEndEffect;
}
