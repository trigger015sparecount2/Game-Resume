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
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetTexture(LPDIRECT3DTEXTURE9 *Texture);
	void SetScroll(float fScroll);
	static CSky *Create(D3DXVECTOR3 pos, float fScroll, LPDIRECT3DTEXTURE9 *Texture);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL; //頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff = NULL;
	D3DXMATRIX m_mtxWorld; //ワールドマトリックス
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;

	float m_fScroll;
};

#endif