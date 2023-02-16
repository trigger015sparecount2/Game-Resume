//=============================================================================
//
// シーン処理 [billboard.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"
#include "plane.h"

class CBillboard : public CPlane
{
public:
	CBillboard(int nPriority);
	~CBillboard();
	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画
	static CBillboard *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos);	//作成

private:
	LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
};

#endif