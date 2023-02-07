//=============================================================================
//
// 爆発処理 [explosion.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"
#include "scene2d.h"

class CExplosion : public CScene2D
{
public:
	CExplosion(PRIORITY Priority = PRIORITY_EFFECT);
	~CExplosion();
	HRESULT Init(D3DXVECTOR3 pos);	//初期化
	void Uninit(void);				//終了
	void Update(void);				//更新
	void Draw(void);				//描画
	bool GetDamage(void) { return false; }
	void Damage(int) { ; }

	static HRESULT Load(void);					//テクスチャ読み込み
	static void UnLoad(void);					//テクスチャ破棄
	static CExplosion *Create(D3DXVECTOR3 pos);	//作成

private:
	int m_nPatternAnim;	//アニメーションパターン
	int m_nCounterAnim;	//アニメーションカウント
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
};

#endif