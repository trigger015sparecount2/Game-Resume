//=============================================================================
// 3Dエフェクト処理 [BillEffect.cpp]
// Author : 佐藤秀亮
// 10/24 引数が多すぎたのでちょっと少なくしました（カラーには問題ないです）
//=============================================================================
#include "billEffect.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// マクロ
//=============================================================================
#define MAX_COLOR (255)


//=============================================================================
// コンストラクタ
//=============================================================================
CBillEffect::CBillEffect(int nPriority) : CPlane::CPlane(nPriority)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CBillEffect::~CBillEffect()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CBillEffect::Init(D3DXVECTOR3 Size, D3DXVECTOR3 MinSize, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, int nTex, int nLife)
{
	CPlane::Init(Size, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	SetTexture(nTex);

	m_Size = Size;			//大きさ
	m_MinSize = MinSize;	//大きさ変動

	//カラー
	m_Color = color;

	//カラー変動
	m_MinColor = Mincolor;

	m_nLife = nLife;
	m_bUninit = false;

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CBillEffect::Uninit()
{
	CPlane::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CBillEffect::Update()
{

	m_Size += m_MinSize;		//サイズ変更

								//カラー変更
	m_Color.r += m_MinColor.r;
	m_Color.g += m_MinColor.g;
	m_Color.b += m_MinColor.b;
	m_Color.a += m_MinColor.a;


	//サイズが0を下回りそう
	if (m_Size.x <= 0 ||
		m_Size.y <= 0)
	{
		m_bUninit = true;
	}

	//カラー値が0を下回りそう
	if (m_Color.r <= 0)
	{
		m_Color.r = 0;
	}
	if (m_Color.g <= 0)
	{
		m_Color.g = 0;
	}
	if (m_Color.b <= 0)
	{
		m_Color.b = 0;
	}
	if (m_Color.a <= 0)
	{
		m_Color.a = 0;
	}

	//カラー値がMAX_COLORを上回りそう
	if (m_Color.r >= MAX_COLOR)
	{
		m_Color.r = MAX_COLOR;
	}
	if (m_Color.g >= MAX_COLOR)
	{
		m_Color.g = MAX_COLOR;
	}
	if (m_Color.b >= MAX_COLOR)
	{
		m_Color.b = MAX_COLOR;
	}
	if (m_Color.a >= MAX_COLOR)
	{
		m_Color.a = MAX_COLOR;
	}

	//それぞれ適応
	ChangeSize(m_Size);
	CPlane::ColorChange(m_Color);

	//寿命減少
	m_nLife--;
	if (m_nLife <= 0)
	{
		m_bUninit = true;
	}

	//抹消
	if (m_bUninit == true)
	{
		SetDeath(true);
	}

}

//=============================================================================
// 描画
//=============================================================================
void CBillEffect::Draw()
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxTrans, mtxWorld; //計算用マトリックス
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する


	//Zテスト関係
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//アルファテスト関係
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//加算合成関係
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//ラインティングを無視する
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

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

	pDevice->SetTexture(0, m_pTexture[m_nTexType]);    //テクスチャの設定

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,  //開始する始点のインデックス
		2); //描画するプリミティブ数

			//Zテスト関係
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテスト関係
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

	//通常合成に戻す(加算合成)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//位置によっては映らないようにする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//=============================================================================
// ポリゴンテクスチャセット
//=============================================================================
void CBillEffect::SetTexture(int nTex)
{
	m_nTexType = nTex;
}
