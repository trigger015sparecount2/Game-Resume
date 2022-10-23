//*****************************************************************************
// エフェクト処理 [particle.cpp]
// Author : 三上航世
//*****************************************************************************

#include "particle.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CParticle::CParticle(CScene::PRIORITY Priority) : CPlane(Priority)
{
	m_Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_MinColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CParticle::~CParticle()
{

}

//*****************************************************************************
//初期化
//*****************************************************************************
HRESULT CParticle::Init(D3DXVECTOR3 pos, D3DXCOLOR color, D3DXCOLOR Mincolor, D3DXVECTOR3 Size, int nLife, int nType, D3DXVECTOR3 Move, D3DXVECTOR3 Rot)
{
	CPlane::Init(Size, pos, D3DXVECTOR2(1.0f, 1.0f), false);
	m_nType = nType;
	m_nLife = nLife;
	m_Move = Move;
	m_Color = color;
	m_MinColor = Mincolor;
	m_Rot = Rot;
	ChangeColor(m_Color);
	return S_OK;
}

//*****************************************************************************
//終了
//*****************************************************************************
void CParticle::Uninit()
{
	CPlane::Uninit();
}

//*****************************************************************************
//更新
//*****************************************************************************
void CParticle::Update()
{
	if (CManager::GetPause() == false && CManager::GetEnd() == false)
	{
		D3DXMATRIX mtxView;
		// 頂点情報を設定
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスの取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		D3DXVECTOR3 pos = GetPos();
		m_Move -= m_Move * 0.05f;
		pos += m_Move;
		SetPos(pos);

		//カラー変更
		m_Color += m_MinColor;

		//カラーが0以下の時
		if (m_Color.r < 0)
		{
			m_Color.r = 0;
		}
		if (m_Color.g < 0)
		{
			m_Color.g = 0;
		}
		if (m_Color.b < 0)
		{
			m_Color.b = 0;
		}
		if (m_Color.a < 0)
		{
			m_Color.a = 0;
		}

		ChangeColor(m_Color);
		m_nLife--;
		if (m_nLife <= 0)
		{
			SetDeath(true);
		}
	}
}

//*****************************************************************************
// 描画
//*****************************************************************************
void CParticle::Draw()
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld; //計算用マトリックス
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する
														//ラインティングを無視する
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//常に表示されるようにする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//ワールドマトリックスの初期化
	//D3DXMatrixIdentity(&g_Billboard[nCntBoard].mtxWorld);
	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&mtxWorld, NULL, &mtxView); //逆行列を求める
	mtxWorld._41 = 0.0f;
	mtxWorld._42 = 0.0f;
	mtxWorld._43 = 0.0f;
	/*										左辺のこと
	g_mtxWorldBillboard._11 = mtxView._11;
	g_mtxWorldBillboard._12 = mtxView._21;
	g_mtxWorldBillboard._13 = mtxView._31;
	g_mtxWorldBillboard._14 = mtxView._41;
	g_mtxWorldBillboard._21 = mtxView._12;
	g_mtxWorldBillboard._22 = mtxView._22;
	g_mtxWorldBillboard._23 = mtxView._32;
	g_mtxWorldBillboard._24 = mtxView._42;
	g_mtxWorldBillboard._31 = mtxView._13;
	g_mtxWorldBillboard._32 = mtxView._23;
	g_mtxWorldBillboard._33 = mtxView._33;
	g_mtxWorldBillboard._34 = mtxView._43;
	g_mtxWorldBillboard._41 = mtxView._14;	xのPos
	g_mtxWorldBillboard._42 = mtxView._24;	yのPos
	g_mtxWorldBillboard._43 = mtxView._34;	zのPos
	g_mtxWorldBillboard._44 = mtxView._44;
	*/
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	SetMatrix(mtxWorld);
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, GetVtx(), 0, sizeof(VERTEX_3D));
	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->SetTexture(0, *GetTexture(m_nType));    //テクスチャの設定
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,  //開始する始点のインデックス
		2); //描画するプリミティブ数
			//ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//位置によっては映らないようにする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXCOLOR color, D3DXCOLOR Mincolor, D3DXVECTOR3 Size, int nLife, int nType, D3DXVECTOR3 Move, D3DXVECTOR3 Rot)
{
	CParticle *pParticle;
	pParticle = new CParticle(PRIORITY_EFFECT);
	if (pParticle != NULL)
	{
		pParticle->Init(pos, color, Mincolor, Size, nLife, nType, Move, Rot);
	}
	return pParticle;
}