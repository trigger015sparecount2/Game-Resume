//=============================================================================
//
// 雷弾処理 [thunder.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _THUNDER_H_
#define _THUNDER_H_
#include "main.h"
#include "scene2d.h"

#define THUNDER_SIZE (25.0f)
#define BAND_LENGTH (300.0f)

class CThunder : public CScene2D
{
public:
	CThunder(PRIORITY Priority = PRIORITY_BULLET);
	~CThunder();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画
	bool GetDamage(void) { return false; }
	void Damage(int) { ; }

	static HRESULT Load(void);	//テクスチャ読み込み
	static void UnLoad(void);	//テクスチャ破棄
	static CThunder *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move);	//作成

private:
	void BandCreate(int nEnemy, D3DXVECTOR3 pos);	//雷の帯作成

	int m_nEffectTime;			//エフェクト作るまでの時間
	D3DXVECTOR3 m_move;			//移動速度
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
	CScene2D *m_pThunderEffect;	//雷エフェクト
	bool m_bUninit;				//削除するか
};

#endif