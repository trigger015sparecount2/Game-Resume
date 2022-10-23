//=============================================================================
//
// シーン処理 [scene2d.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_
#include "main.h"
#include "scene.h"
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1) //頂点フォーマット

typedef struct
{
	D3DXVECTOR3 pos; //頂点座標
	float rhw;       //1.0fで固定
	D3DCOLOR col;    //頂点カラー
	D3DXVECTOR2 tex; //テクスチャ座標
}VERTEX_2D;

class CScene2D : public CScene
{
public:
	CScene2D(PRIORITY Priority = PRIORITY_CHARA);
	~CScene2D();
	HRESULT Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(float fWidth, float fHeight, D3DXVECTOR3 pos);
	void SetRot(float fRot);
	void SetSize(float fSize);
	static CScene2D *Create(float fWidth, float fHeight, D3DXVECTOR3 pos, PRIORITY Priority);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	void SetTexture(int nCntTex, int nNumTex);
	void ScrollTexture(float fTex);
	void ChangeColor(D3DCOLOR col);
	bool GetDamage(void);
	void Damage(int nDamage);
	float GetRot(int nRot);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL; //頂点バッファへのポインタ
	float m_fRot[4]; //ポリゴンの向き(上が1.0f)
	float m_fScale;    //ポリゴンの大きさ
	bool m_bScale;     //ポリゴンが大きくなるか小さくなるか
	LPDIRECT3DTEXTURE9 m_pTexture = NULL; //テクスチャへのポインタ
};

#endif