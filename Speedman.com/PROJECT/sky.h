//=============================================================================
//
// 空処理 [sky.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _SKY_H_
#define _SKY_H_

#include "main.h"
#include "scene3d.h"

class CSky : public CScene
{
public:
	CSky(int nPriority);
	~CSky();

	HRESULT Init(D3DXVECTOR3 pos);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画

	void SetTexture(LPDIRECT3DTEXTURE9 *Texture) { m_pTexture = *Texture; }				//テクスチャ設定
	void SetScroll(float fScroll) { m_fScroll = fScroll; }	//スクロール速度設定

	static CSky *Create(D3DXVECTOR3 pos, float fScroll, LPDIRECT3DTEXTURE9 *Texture);	//作成

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff = NULL;	//インデックスバッファ
	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 m_pos;		//場所
	D3DXVECTOR3 m_rot;		//向き

	float m_fScroll;		//スクロール速度
};

#endif