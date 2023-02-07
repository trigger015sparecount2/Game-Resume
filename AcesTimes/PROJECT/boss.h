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

class CBossParts;
class CShadow;
class CBoss : public CScene2D
{
public:
	CBoss(PRIORITY Priority = PRIORITY_CHARA);
	~CBoss();
	HRESULT Init(D3DXVECTOR3 pos, float fInterval, D3DXVECTOR2 size, int nShadowTex, int nHP, float fBulletMax, int nMoney, int nScore, bool bParts, int nBGMType);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画
	OBJTYPE GetObjType() { return OBJECTTYPE_ENEMY; }	//種類
	void Damage(int nDamage);	//ダメージ処理

	static CBoss *Create(D3DXVECTOR3 pos, int nTexType, int nShadowTex, float fInterval, D3DXVECTOR2 size, int nHP, float fBulletMax, int nMoney, int nScore, bool bParts, int nBGMType);	//作成

private:
	D3DXVECTOR3 Move();									//移動
	D3DXVECTOR3 SlowMove();								//遅い移動
	void CollisionBullet(D3DXVECTOR3 pos);				//弾との当たり判定
	bool CollisionBomb(D3DXVECTOR3 pos);				//爆弾との当たり判定
	void Shot(D3DXVECTOR3 pos);							//撃つ
	void PartsShot(D3DXVECTOR3 pos, int nCntBullet);	//パーツが撃つ

	CShadow *m_pShadow;				//影
	float m_fBullet[7];				//撃つ
	float m_fBulletMax;				//次撃つまでのインターバル
	int m_nHP;						//体力
	int m_nDamageTime;				//ダメージを食らってからの時間
	int m_nType;					//敵の種類
	bool m_bSlow;					//爆弾に当たっているか
	int m_nMoney;					//金
	int m_nScore;					//スコア
	bool m_bParts;					//パーツ持ちか
	bool m_bMove[2];				//上下左右の移動
	int m_nCntStart;				//出現してからの時間
	int m_nBullet;					//連続で何回撃つか
	bool m_bDeath;					//消すか
	int m_nCntDeath;				//やられてからのカウント
	CBossParts *m_pBossParts[8];	//ボスのパーツ
};
#endif