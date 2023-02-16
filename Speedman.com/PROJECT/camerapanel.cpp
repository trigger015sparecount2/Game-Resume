//---------------------------
//Author:三上航世
//踏んだらカメラの方向が切り替わる(kamerapanel.cpp)
//---------------------------
#include "camerapanel.h"
#include "manager.h"
#include "renderer.h"

CCameraPanel::CCameraPanel(int nPriority) : CPlane::CPlane(nPriority)
{

}

CCameraPanel::~CCameraPanel()
{

}

//初期化処理
HRESULT CCameraPanel::Init(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR3 posV, D3DXVECTOR3 posR)
{
	CPlane::Init(size, pos, D3DXVECTOR2(1.0f, 1.0f));
	CScene::SetObjType(OBJCTTYPE_CAMERAPANEL);
	CScene::SetCameraPosR(posR);
	CScene::SetCameraPosV(posV);
	return S_OK;
}

//終了処理
void CCameraPanel::Uninit()
{
	CPlane::Uninit();
}

//更新処理
void CCameraPanel::Update()
{

}

//描画処理
void CCameraPanel::Draw()
{
#ifdef _DEBUG
	CPlane::Draw();
#endif
}

//作成
CCameraPanel *CCameraPanel::Create(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR3 posV, D3DXVECTOR3 posR)
{
	CCameraPanel *pPanel;
	pPanel = new CCameraPanel(2);
	if (pPanel != NULL)
	{
		pPanel->Init(size, pos, posV, posR);
		pPanel->SetTexture(4);
	}
	return pPanel;
}