//=============================================================================
//
// 水属性弾処理 [water.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _WATER_H_
#define _WATER_H_

#include "main.h"
#include "scene2d.h"

//マクロ定義
#define WATER_SIZE (240.0f)
#define WATER_MAX_ENEMY (16)

class CWater : public CScene2D
{
public:
	CWater(PRIORITY Priority = PRIORITY_BG);
	~CWater();
	HRESULT Init(D3DXVECTOR3 pos);				//初期化
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	bool GetDamage(void) { return false; }		//ダメージ受けるか
	void Damage(int) { ; }						//ダメージ処理
	void SetUninit(void) { m_bSize = false; }	//小さくしていく
	void SetRotDesh(float fRotDeshY);			//向き設定
	
	static HRESULT Load(void);				//テクスチャ読み込み
	static void UnLoad(void);				//テクスチャ破棄
	static CWater *Create(D3DXVECTOR3 pos);	//作成

private:
	bool EnemyList(int nEnemy);	//どの敵に当たったのかリスト内を参照
	void ListPush(int nEnemy);	//リストに入れる

	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
	D3DXVECTOR3 *m_pPlayerpos;				//プレイヤーのposのポインタ
	float m_fRotY;							//向き
	float m_fRotDeshY;						//目的の向き
	float m_fSize;							//大きさ
	bool m_bSize;							//大きくするか
	int m_nListCnt;							//リストの何番目に保存するか
	int m_aEnemies[WATER_MAX_ENEMY];		//敵のリスト
	bool m_bDraw;							//描画するか
};

#endif