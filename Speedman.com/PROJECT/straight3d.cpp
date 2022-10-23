//=============================================================================
// (直線)3Dエフェクト処理 [Straight3D.cpp]
// Author : 佐藤秀亮
// 10/24 引数が多す(ｒｙ
//=============================================================================
#include "Straight3D.h"

#include "renderer.h"
#include "Camera.h"
#include "manager.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CStraight3D::CStraight3D(int nPriority) : CBillEffect(nPriority)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CStraight3D::~CStraight3D()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CStraight3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 Size, D3DXVECTOR3 MinSize, D3DXVECTOR3 move, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, int nType, int nLife)
{
	m_move = move;

	CBillEffect::Init(Size, MinSize, color, Mincolor, nType, nLife);

	m_pCamera = CManager::GetRenderer()->GetCamera();

	//m_rot.x = m_pCamera->GetRotX();
	//m_rot.y = m_pCamera->GetRotY();

	SetPos(pos);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CStraight3D::Uninit()
{
	CBillEffect::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CStraight3D::Update()
{
	bool Pause = CManager::GetPause();

	if (Pause == false)
	{
		D3DXVECTOR3 pos = GetPos();

		pos += m_move;

		SetPos(pos);

		CBillEffect::Update();
	}
}

//=============================================================================
// 描画
//=============================================================================
void CStraight3D::Draw()
{
	CBillEffect::Draw();
}

//=============================================================================
// 作成
//=============================================================================
CStraight3D *CStraight3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 Size, D3DXVECTOR3 MinSize, D3DXVECTOR3 move, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, int nType, int nLife)
{
	CStraight3D *pStraight3D = new CStraight3D(3);

	if (pStraight3D != NULL)
	{
		pStraight3D->Init(pos, Size, MinSize, move, color, Mincolor, nType, nLife);
	}

	return pStraight3D;
}
