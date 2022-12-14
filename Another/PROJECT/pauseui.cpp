//=============================================================================
//ポーズ管理[pauseui.cpp]
// Author : 三上　航世
//=============================================================================
//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "pauseui.h"
#include "manager.h"

//*****************************************************************************
//マクロ
//*****************************************************************************
#define MAX_GAMESERECT (4)

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CPauseUI::CPauseUI(PRIORITY Priority) : CUI::CUI(Priority)
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CPauseUI::~CPauseUI()
{

}

//*****************************************************************************
// 初期化
//***************************************************************************** 
HRESULT CPauseUI::Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, D3DXCOLOR col)
{
	CScene2D::Init(pos);
	CScene2D::SetTexture(nType);	//選択した番号のテクスチャを貼る
	CScene2D::SetSize(D3DXVECTOR2(SizeX, SizeY));
	CScene2D::ColorChange(col);
	return S_OK;
}

//*****************************************************************************
// 終了
//***************************************************************************** 
void CPauseUI::Uninit()
{
	CUI::Uninit();
}

//*****************************************************************************
// 更新
//***************************************************************************** 
void CPauseUI::Update()
{
	
}

//*****************************************************************************
//描画
//***************************************************************************** 
void CPauseUI::Draw()
{
	if (CManager::GetPause() == true)
	{
		CUI::Draw();
	}
}

//*****************************************************************************
//作成
//***************************************************************************** 
CPauseUI *CPauseUI::Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, D3DXCOLOR col)
{
	CPauseUI *pPauseUI = NULL;
	pPauseUI = new CPauseUI(PRIORITY_PAUSEUI);		//メモリ確保
	//NULLチェック
	if (pPauseUI != NULL)
	{
		pPauseUI->Init(pos, SizeX, SizeY, nType, col);
	}

	return pPauseUI;
}