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

#define ENEMY_SIZE (50.0f)

class CEnemy : public CScene2D
{
public:
	CEnemy(PRIORITY Priority = PRIORITY_CHARA);
	~CEnemy();
	HRESULT Init(D3DXVECTOR3 pos, int nType, float fRotY, int ninterval, float fSpeed);	//初期化処理
	void Uninit(void);						//終了処理
	void Update(void);						//更新処理
	void Draw(void);						//描画処理
	bool GetDamage(void) { return true; }	//ダメージを受けるか
	void Damage(int nDamage);				//ダメージ処理

	static HRESULT Load(void);	//テクスチャ読み込み
	static void UnLoad(void);	//テクスチャ破棄
	static CEnemy *Create(D3DXVECTOR3 pos, int nTexType, float fRotY, int nInterval, float fSpeed);	//作成

private:
	D3DXVECTOR3 Move(D3DXVECTOR3 pos);			//移動処理
	void Shot(D3DXVECTOR3 pos);					//弾発射

	static LPDIRECT3DTEXTURE9 m_pTexture[4];	//テクスチャ
	CScene2D *m_pShadowScene;					//影
	int m_nBullet;								//弾発射カウント
	int m_nBulletInter;							//弾発射カウントの最大
	int m_nHP;									//体力
	int m_nDamageTime;							//ダメージを食らってからの時間
	bool m_bUninit;								//削除するか
	bool m_bShot;								//撃ったか
	float m_fSpeed;								//移動速度
	float m_fRot;								//向いてる方向
	int m_nScore;								//スコア
	int m_nType;								//属性

};
#endif