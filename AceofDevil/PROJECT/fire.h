//=============================================================================
//
// 火属性弾処理 [fire.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _FIRE_H_
#define _FIRE_H_
#include "main.h"
#include "scene2d.h"

class CFire : public CScene2D
{
public:
	CFire(PRIORITY Priority = PRIORITY_BULLET);
	~CFire();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move);	//初期化
	void Uninit(void);									//終了
	void Update(void);									//更新
	void Draw(void);									//描画
	bool GetDamage(void) { return false; }				//ダメージ受けるか
	void Damage(int) { ; }						//ダメージ処理

	static HRESULT Load(void);		//テクスチャ読み込み
	static void UnLoad(void);		//テクスチャ破棄
	static CFire *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move);	//作成
	D3DXVECTOR3 *RockEnemy(void);	//敵をロックオンする
	void Homing(D3DXVECTOR3 pos);	//追尾

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
	D3DXVECTOR3 *m_pPos;					//敵のposのポインタ
	int m_nEnemy;							//何番目の敵をロックオンするか
	int m_nEffect;							//エフェクトカウント
	float m_fRotY;							//向いてる方向
	bool m_bUninit;							//削除するか
	D3DXVECTOR3 m_move;						//移動速度
	int m_nLife;							//寿命
};

#endif