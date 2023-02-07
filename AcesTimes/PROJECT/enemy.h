//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "scene2d.h"

class CShadow;

class CEnemy : public CScene2D
{
public:
	CEnemy(PRIORITY Priority = PRIORITY_CHARA);
	~CEnemy();
	HRESULT Init(D3DXVECTOR3 pos, float fRotY, float fInterval, float fSpeed, D3DXVECTOR2 size, int nShadowTex, int nHP, float fBulletMax, int nMoney, int nScore, float fBulletSpeed, int nBulletType);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画
	OBJTYPE GetObjType() { return OBJECTTYPE_ENEMY; }	//種類
	void Damage(int nDamage);	//ダメージ処理

	static CEnemy *Create(D3DXVECTOR3 pos, int nTexType, int nShadowTex, float fRotY, float fInterval, float fSpeed, D3DXVECTOR2 size, int nHP, float fBulletMax, int nMoney, int nScore, float fBulletSpeed, int nBulletType);	//作成
private:
	D3DXVECTOR3 Move();						//移動
	D3DXVECTOR3 SlowMove();					//遅い移動
	void CollisionBullet(D3DXVECTOR3 pos);	//弾との当たり判定
	bool CollisionBomb(D3DXVECTOR3 pos);	//爆弾との当たり判定
	void Shot(D3DXVECTOR3 pos);				//撃つ

	CShadow *m_pShadow;		//影
	float m_fBullet;		//弾撃つまでのカウント
	float m_fBulletMax;		//次撃つまでのカウント
	int m_nHP;				//体力
	int m_nDamageTime;		//ダメージを食らってからの時間
	float m_fSpeed;			//移動速度
	float m_fRot;			//向いてる方向
	int m_nType;			//敵の種類
	bool m_bSlow;			//遅いか
	int m_nMoney;			//金
	int m_nScore;			//スコア
	float m_fBulletSpeed;	//弾速
	int m_nBulletType;		//撃ち方
};
#endif