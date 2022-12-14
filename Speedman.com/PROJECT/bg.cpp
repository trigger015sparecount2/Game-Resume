//*****************************************************************************
// 背景関連処理(BG.cpp)
// Author : satou syusuke
//*****************************************************************************

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "Bg.h"
#include "Scene2D.h"
#include "manager.h"
#include "Title.h"
#include "Game.h"

//*****************************************************************************
//静的
//*****************************************************************************

//*****************************************************************************
//マクロ
//*****************************************************************************
#define MAX_SPEED (4.0f)

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CBG::CBG(int nPriority) : CScene2D(nPriority)
{
	m_bUninit = false;

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CBG::~CBG()
{

}

//*****************************************************************************
//初期化
//*****************************************************************************
HRESULT CBG::Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType)
{
	CScene2D::Init(pos);

	SetObjType(CScene::OBJECTTYPE_UI);

	m_pos = pos;
	m_bUninit = false;

	CScene2D::SetTexture(nType);	//選択した番号のテクスチャを貼る
	CScene2D::SetWhidth(SizeX);		//大きさ適応
	CScene2D::SetHight(SizeY);
	return S_OK;
}

//*****************************************************************************
//終了
//*****************************************************************************
void CBG::Uninit()
{
	CScene2D::Uninit();
}

//*****************************************************************************
//更新
//*****************************************************************************
void CBG::Update()
{

	//削除
	if (m_bUninit == true)
	{
		SetDeath(true);
	}
}

//*****************************************************************************
//描画
//*****************************************************************************
void CBG::Draw()
{
	CScene2D::Draw();
}

//*****************************************************************************
//作成
//*****************************************************************************
CBG *CBG::Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType)
{
	CBG * pBg = NULL;
	pBg = new CBG(0);
	if (pBg != NULL)
	{
		pBg->Init(pos, SizeX, SizeY, nType);
	}
	return pBg;
}