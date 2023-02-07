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

class CBullet : public CScene2D
{
public:
	CBullet(PRIORITY Priority);
	~CBullet();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSpeed, bool bEnemy);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画
	OBJTYPE GetObjType() { return OBJECTTYPE_BULLET; }	//種類
	bool GetEnemy() { return m_bEnemy; }	//敵の物か返す
	int Damage() { return 1; }	//ダメージ量

	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSpeed, bool bEnemy);	//作成

private:
	bool CollisionBomb(D3DXVECTOR3 pos);	//爆弾との当たり判定

	D3DXVECTOR3 m_move;	//移動速度
	bool m_bEnemy;		//敵のものか
	bool m_bSlow;		//遅いか
};

#endif