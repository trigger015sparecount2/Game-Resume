//---------------------------
// Author:三上航世
// 軌跡(orbit.cpp)
//---------------------------
#include "orbit.h"
#include "manager.h"
#include "renderer.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 COrbit::m_pTexture = NULL;

COrbit::COrbit(PRIORITY Priority) : CScene::CScene(Priority)
{

}

COrbit::~COrbit(void)
{

}

//初期化処理
HRESULT COrbit::Init(D3DXVECTOR3)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する
	m_nNumOrbit = 0;
	m_nCnt = 0;
	m_nStart = 0;
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_ORBIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	int nCntOrbit;
	VERTEX_3D *pVtx; //頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	//頂点座標の設定
	for (nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		pVtx[nCntOrbit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[nCntOrbit].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[nCntOrbit].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[nCntOrbit].tex = D3DXVECTOR2(0.0f, 0.0f);
	}
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (MAX_ORBIT + 2),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);
	WORD *pIdx; //インデックス情報へのポインタ
	//インデックスバッファをロックし、番号データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void **)&pIdx, 0);
	for (nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		pIdx[nCntOrbit] = (WORD)nCntOrbit;
	}
	pIdx[MAX_ORBIT] = 0;
	pIdx[MAX_ORBIT + 1] = 1;
	//インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();
	return S_OK;
}

//終了処理
void COrbit::Uninit(void)
{
	/*if (m_pTexture != NULL)
	{
	m_pTexture->Release();
	m_pTexture = NULL;
	}*/
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	//頂点バッファの破棄
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
	Release();
}

//更新処理
void COrbit::Update(void)
{
	int nCntOrbit;
	VERTEX_3D *pVtx; //頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		if (m_col[nCntOrbit].a > 0.0f)
		{
			m_col[nCntOrbit].a -= m_fDis[nCntOrbit];
			pVtx[nCntOrbit].col = m_col[nCntOrbit];
			if (m_col[nCntOrbit].a <= 0.0f)
			{
				//pVtx[nCntOrbit].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
				m_nNumOrbit--;
				m_nStart++;
				if (m_nStart >= MAX_ORBIT)
				{
					m_nStart = 0;
				}
			}
		}
	}
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//描画処理
void COrbit::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する
	//ラインティングを無視する
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//カリングオフ
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);
	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->SetTexture(0, NULL);    //テクスチャの設定
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		MAX_ORBIT, //頂点の数
		m_nStart, //最初の頂点番号
		m_nNumOrbit - 2); //描画するプリミティブ数
	if (m_nNumOrbit > MAX_ORBIT - m_nStart)
	{
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			0,
			MAX_ORBIT, //頂点の数
			0, //最初の頂点番号
			m_nNumOrbit - (MAX_ORBIT - m_nStart) - 2); //描画するプリミティブ数
	}
	//ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//カリングオン
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

COrbit *COrbit::Create(void)
{
	COrbit *pOrbit;
	pOrbit = new COrbit(PRIORITY_ORBIT);
	if (pOrbit != NULL)
	{
		pOrbit->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	return pOrbit;
}

void COrbit::SetOrbit(D3DXVECTOR3 pos, D3DXVECTOR3 Tippos, float fDis, D3DXCOLOR col1, D3DXCOLOR col2)
{
	VERTEX_3D *pVtx; //頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	pVtx[m_nCnt].pos = pos;
	pVtx[m_nCnt].col = col2;
	m_col[m_nCnt] = col2;
	m_fDis[m_nCnt] = fDis;
	m_nCnt++;
	pVtx[m_nCnt].pos = Tippos;
	pVtx[m_nCnt].col = col1;
	m_col[m_nCnt] = col1;
	m_fDis[m_nCnt] = fDis;
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	m_nCnt++;
	if (m_nCnt >= MAX_ORBIT)
	{
		m_nCnt = 0;
	}
	m_nNumOrbit += 2;
}