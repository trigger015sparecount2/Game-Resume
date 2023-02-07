//=============================================================================
//
// 氷弾処理 [ice.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _ICE_H_
#define _ICE_H_
#include "main.h"
#include "scene2d.h"

//マクロ定義
#define ICE_MAX_ENEMY (16)

class CIce : public CScene2D
{
public:
	CIce(PRIORITY Priority = PRIORITY_BULLET);
	~CIce();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRotY);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画
	bool GetDamage(void) { return false; }
	void Damage(int) { ; }

	static HRESULT Load(void);	//テクスチャ読み込み
	static void UnLoad(void);	//テクスチャ破棄
	static CIce *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRotY);	//作成

private:
	bool EnemyList(int nEnemy);	//リスト内と比較、当たっているか
	void ListPush(int nEnemy);	//リストに入れる

	D3DXVECTOR3 m_move;	//移動速度
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
	float m_fRotY;	//向いてる方向
	bool m_bUninit;	//削除するか
	int m_nListCnt;	//リストの何番目に入れるか
	int m_nEffect;	//エフェクトカウント
	int m_aEnemies[ICE_MAX_ENEMY];	//当たった敵のリスト
};

#endif