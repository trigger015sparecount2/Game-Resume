//=============================================================================
//
// ボスパーツ処理 [bossparts.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _BOSSPARTS_H_
#define _BOSSPARTS_H_

#include "main.h"
#include "scene2d.h"

class CBossParts : public CScene2D
{
public:
	CBossParts(PRIORITY Priority = PRIORITY_CHARA);
	~CBossParts();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nFireTex, int nHP, int nMoney, int nScore);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画
	OBJTYPE GetObjType() { return OBJECTTYPE_ENEMY; }	//種類
	void Damage(int nDamage);							//ダメージ処理
	void SetDestroy(bool bDeath) { m_bDeath = bDeath; }	//やられたか設定
	bool GetDestroy() { return m_bDeath; }				//壊れているか返す

	static CBossParts *Create(D3DXVECTOR3 pos, int nTexType, int nFireTex, D3DXVECTOR2 size, int nHP, int nMoney, int nScore);	//作成

private:
	void CollisionBullet(D3DXVECTOR3 pos);	//弾との当たり判定

	CScene2D *m_pFire;	//バックファイア
	int m_nHP;			//体力
	int m_nDamageTime;	//ダメージを食らってからの時間
	int m_nMoney;		//金
	int m_nScore;		//スコア
	int m_nCntFire;		//バックファイアの大小カウント
	bool m_bFire;		//バックファイア
	bool m_bDeath;		//やられたか
	int m_nCntDeath;	//やられたあとのカウント
};
#endif