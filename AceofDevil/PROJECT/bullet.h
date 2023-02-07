//=============================================================================
//
// 弾丸処理 [bullet.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
#include "scene2d.h"

#define BULLET_SIZE (10.0f)

class CBullet : public CScene2D
{
public:
	CBullet(PRIORITY Priority = PRIORITY_BULLET);
	~CBullet();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, bool bEnemy, CScene::ELEMENT element);	//初期化処理
	void Uninit(void);						//終了処理
	void Update(void);						//更新処理
	void Draw(void);						//描画処理
	bool GetDamage(void) { return false; }	//何もしない
	void Damage(int) { ; }					//何もしない

	static HRESULT Load(void);	//テクスチャ読み込み
	static void UnLoad(void);	//テクスチャの破棄
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, bool bEnemy, CScene::ELEMENT element);	//作成

private:
	D3DXVECTOR3 m_move;						//移動速度
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
	int m_nEffect;							//エフェクトの発生
	bool m_bUninit;							//削除するか
};

#endif