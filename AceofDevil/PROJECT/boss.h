//=============================================================================
//
// ボス処理 [boss.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"
#include "scene2d.h"

class CBoss : public CScene2D
{
public:
	CBoss(PRIORITY Priority = PRIORITY_CHARA);
	~CBoss();
	HRESULT Init(void);			//初期化処理
	void Uninit(void);			//終了処理
	void Update(void);			//更新処理
	void Draw(void);			//描画処理
	bool GetDamage(void);		//ダメージを受けるか
	void Damage(int);			//ダメージ処理

	static HRESULT Load(void);	//テクスチャ読み込み
	static void UnLoad(void);	//テクスチャ破棄
	static CBoss *Create(void);	//作成

private:
	void Move(D3DXVECTOR3 pos);				//移動処理
	void Shot(D3DXVECTOR3 pos);				//弾発射
	float SearchPlayer(D3DXVECTOR3 pos);	//プレイヤーを見つける

	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
	D3DXVECTOR3 m_move;						//移動量
	CScene2D *m_pShadowScene;				//影
	int m_nBullet;							//弾発射カウント
	int m_nBulletInter;						//弾発射カウントの最大
	int m_nHP;								//体力
	int m_nDamageTime;						//ダメージを食らってからの時間
	bool m_bUninit;							//削除するか
	bool m_bSlide;							//撃破後の左右のぶれ
	bool m_bDrop;							//登場時の上から出てくるか
	bool m_bLeft;							//火属性の弾を撃つとき、左右に振る
	int m_nSlide;							//撃破後の左右のぶれのカウント
	int m_nScore;							//スコア
	int m_nDTime;							//撃破後の爆発エフェクト
	int m_nCntBullet;						//火属性の弾を撃つときのカウント
	int m_nCycle;							//攻撃時のサイクル(同じ属性で何回弾を発射するか)
	int m_nBTime;							//弾発射時の次弾撃つまでのカウント
	int m_nType;							//何属性の弾を撃つか
	float m_fScale;							//大きさ
	float m_fRotY;							//プレイヤーの向き
};
#endif