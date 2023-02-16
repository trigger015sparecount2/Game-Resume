//*****************************************************************************
// (落ちる)エフェクト処理 [FallEffect.cpp]
// Author : 佐藤秀亮
//*****************************************************************************

#include "FallEffect.h"

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CFallEffect::CFallEffect(int nPriority) : CEffect(nPriority)
{
	m_Size = D3DXVECTOR2(0.0f, 0.0f);

	m_nLife = 0;
	m_bUninit = false;
	m_move = D3DXVECTOR2(0.0f, 0.0f);
	m_Addpos = D3DXVECTOR2(0.0f, 0.0f);
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CFallEffect::~CFallEffect()
{

}

//*****************************************************************************
//	初期化
//*****************************************************************************
HRESULT CFallEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR2 move, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, D3DXVECTOR2 Size, D3DXVECTOR2 MinSize, int nLife, int nType, D3DXVECTOR2 AddMovement)
{
	CEffect::Init(pos, color, Mincolor, Size, nLife, nType);
	m_Size = Size;
	m_MinSize = MinSize;

	m_nLife = nLife;
	m_Addpos = AddMovement;
	m_move = move;
	return S_OK;
}

//*****************************************************************************
//終了
//*****************************************************************************
void CFallEffect::Uninit()
{
	CEffect::Uninit();
}

//*****************************************************************************
//更新
//*****************************************************************************
void CFallEffect::Update()
{
	CEffect::Update();

	D3DXVECTOR3 pos = GetPosition();

	m_move += m_Addpos;		//移動値に慣性を加算

	pos += D3DXVECTOR3(m_move.x, m_move.y, 0.0f);

	SetPosition(pos);
	m_Size += m_MinSize;

	//サイズ変更（ある場合）
	SetWidth(m_Size.x);
	SetHeight(m_Size.y);

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
void CFallEffect::Draw()
{
	CEffect::Draw();
}

//*****************************************************************************
//作成
//*****************************************************************************
CFallEffect *CFallEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR2 move, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, D3DXVECTOR2 Size, D3DXVECTOR2 MinSize, int nLife, int nType, D3DXVECTOR2 AddMovement)
{
	CFallEffect *pFallEffect = NULL;
	pFallEffect = new CFallEffect(3);		//メモリ確保
											//NULLチェック
	if (pFallEffect != NULL)
	{
		pFallEffect->Init(pos, move, color, Mincolor, Size, MinSize, nLife, nType, AddMovement);
	}

	return pFallEffect;
}
