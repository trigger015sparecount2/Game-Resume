//=============================================================================
//
// 雷の帯処理 [thunderband.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _THUNDERBAND_H_
#define _THUNDERBAND_H_
#include "main.h"
#include "scene2d.h"

class CThunderBand : public CScene2D
{
public:
	CThunderBand(PRIORITY Priority = PRIORITY_CHARA);
	~CThunderBand();
	HRESULT Init(D3DXVECTOR3 pos, float fRotY, float fLength);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画
	bool GetDamage(void) { return false; }	//ダメージ受けるか
	void Damage(int) { ; }					//ダメージ処理

	static HRESULT Load(void);	//テクスチャ読み込み
	static void UnLoad(void);	//テクスチャ破棄
	static CThunderBand *Create(D3DXVECTOR3 pos, float fRotY, float fLength);	//作成

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
	int m_nLife;	//寿命
	int m_nTex;		//アニメーションパターン
};

#endif