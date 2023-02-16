//---------------------------
//Author:三上航世
//ビルボード(billboard.cpp)
//---------------------------
#include "billboard.h"
#include "manager.h"
#include "renderer.h"

CBillboard::CBillboard(int nPriority) : CPlane::CPlane(nPriority)
{

}

CBillboard::~CBillboard()
{

}


//初期化処理
HRESULT CBillboard::Init(D3DXVECTOR3 size, D3DXVECTOR3 pos)
{
	CPlane::Init(size, pos, D3DXVECTOR2(1.0f, 1.0f));
	return S_OK;
}

//終了処理
void CBillboard::Uninit()
{
	CPlane::Uninit();
}

//更新処理
void CBillboard::Update()
{
	
}

//描画処理
void CBillboard::Draw()
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxTrans, mtxWorld; //計算用マトリックス
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
	pDevice->SetTexture(0, NULL);    //テクスチャの設定
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,  //開始する始点のインデックス
		2); //描画するプリミティブ数

	//ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//位置によっては映らないようにする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//作成
CBillboard *CBillboard::Create(D3DXVECTOR3 size, D3DXVECTOR3 pos)
{
	CBillboard *pBillboard;
	pBillboard = new CBillboard(3);
	if (pBillboard != NULL)
	{
		pBillboard->Init(size, pos);
	}
	return pBillboard;
}