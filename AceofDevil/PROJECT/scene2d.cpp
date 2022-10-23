//=============================================================================
//
// シーン処理 [scene.h]
// Author : 三上航世
//
//=============================================================================
#include "scene2d.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

CScene2D::CScene2D(PRIORITY Priority) : CScene::CScene(Priority)
{

}

CScene2D::~CScene2D(void)
{

}

HRESULT CScene2D::Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	m_fRot[0] = -0.75f;
	m_fRot[1] = 0.75f;
	m_fRot[2] = -0.25f;
	m_fRot[3] = 0.25f;
	m_pTexture = NULL;
	SetPos(pos);
	// 頂点情報を設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pos.x + fWidth * sinf(m_fRot[0] * D3DX_PI), pos.y + fHeight * cosf(m_fRot[0] * D3DX_PI), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth * sinf(m_fRot[1] * D3DX_PI), pos.y + fHeight * cosf(m_fRot[1] * D3DX_PI), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x + fWidth * sinf(m_fRot[2] * D3DX_PI), pos.y + fHeight * cosf(m_fRot[2] * D3DX_PI), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth * sinf(m_fRot[3] * D3DX_PI), pos.y + fHeight * cosf(m_fRot[3] * D3DX_PI), 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTexSize, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fTexSize, 1.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

void CScene2D::Uninit(void)
{
	if (m_pTexture != NULL)
	{
		//m_pTexture->Release();
		m_pTexture = NULL;
	}
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	Release();
}

void CScene2D::Update(void)
{

}

void CScene2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	// 頂点フォーマットの設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	pDevice->SetTexture(0, m_pTexture);    //テクスチャの設定

	// ポリゴンの描画
	//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, //プリミティブの種類
	//	2,                   //プリミティブの数
	//	&m_aVertex[0], sizeof(VERTEX_2D));
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,  //開始する始点のインデックス
		2); //描画するプリミティブ数
}

void CScene2D::Set(float fWidth, float fHeight, D3DXVECTOR3 pos)
{
	SetPos(pos);
	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pos.x + fWidth * sinf(m_fRot[0] * D3DX_PI), pos.y + fHeight * cosf(m_fRot[0] * D3DX_PI), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth * sinf(m_fRot[1] * D3DX_PI), pos.y + fHeight * cosf(m_fRot[1] * D3DX_PI), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x + fWidth * sinf(m_fRot[2] * D3DX_PI), pos.y + fHeight * cosf(m_fRot[2] * D3DX_PI), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth * sinf(m_fRot[3] * D3DX_PI), pos.y + fHeight * cosf(m_fRot[3] * D3DX_PI), 0.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

void CScene2D::SetRot(float fRotY) //fRotYの分だけ回す
{
	int nCntRot;
	for (nCntRot = 0; nCntRot < 4; nCntRot++)
	{
		m_fRot[nCntRot] += fRotY;
		if (m_fRot[nCntRot] > 1.0f)
		{
			m_fRot[nCntRot] -= 2.0f;
		}
		else if (m_fRot[nCntRot] < -1.0f)
		{
			m_fRot[nCntRot] += 2.0f;
		}
	}
	/*m_fRot[0] += fRotY;
	if (m_fRot[0] > 1.0f)
	{
		m_fRot[0] -= 2.0f;
	}
	else if (m_fRot[0] < -1.0f)
	{
		m_fRot[0] += 2.0f;
	}
	m_fRot[1] += fRotY;
	if (m_fRot[1] < -1.0f)
	{
		m_fRot[1] += 2.0f;
	}
	m_fRot[2] += fRotY;
	if (m_fRot[2] > 1.0f)
	{
		m_fRot[2] -= 2.0f;
	}
	m_fRot[3] += fRotY;
	if (m_fRot[3] < -1.0f)
	{
		m_fRot[3] += 2.0f;
	}*/
}

void CScene2D::SetSize(float fSize) //0.0f ～ 0.5f　基本は0.25f
{
	m_fRot[0] = -1.0f + fSize; //-0.96f
	m_fRot[1] = 1.0f - fSize; //0.96f
	m_fRot[2] = -fSize; //-0.04f
	m_fRot[3] = fSize; //0.04f
}

CScene2D *CScene2D::Create(float fWidth, float fHeight, D3DXVECTOR3 pos, PRIORITY Priority)
{
	CScene2D *pScene2D;
	pScene2D = new CScene2D(Priority);
	if (pScene2D != NULL)
	{
		pScene2D->Init(fWidth, fHeight, pos, 1.0f);
		pScene2D->Set(fWidth, fHeight, pos);
	}
	return pScene2D;
}

void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

void CScene2D::SetTexture(int nCntTex, int nNumTex)
{
	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2((float)nCntTex / (float)nNumTex, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((float)(nCntTex + 1) / (float)nNumTex, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((float)nCntTex / (float)nNumTex, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((float)(nCntTex + 1) / (float)nNumTex, 1.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

void CScene2D::ScrollTexture(float fTex)
{
	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, fTex);
	pVtx[1].tex = D3DXVECTOR2(24.0f, fTex);
	pVtx[2].tex = D3DXVECTOR2(0.0f, fTex + 15.0f);
	pVtx[3].tex = D3DXVECTOR2(24.0f, fTex + 15.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

void CScene2D::ChangeColor(D3DCOLOR col)
{
	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	//テクスチャ座標
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

void CScene2D::Damage(int nDamage)
{

}

bool CScene2D::GetDamage(void)
{
	return false;
}

float CScene2D::GetRot(int nRot)
{
	return m_fRot[nRot];
}