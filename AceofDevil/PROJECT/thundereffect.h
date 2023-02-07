//=============================================================================
//
// 雷エフェクト処理 [thundereffect.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _THUNDEREFFECT_H_
#define _THUNDEREFFECT_H_
#include "main.h"
#include "scene2d.h"
#define THUNDEREFFECT_SIZE (60.0f)
class CThundereffect : public CScene2D
{
public:
	CThundereffect(PRIORITY Priority = PRIORITY_BG);
	~CThundereffect();
	HRESULT Init(D3DXVECTOR3 pos);			//初期化
	void Uninit(void);						//終了
	void Update(void);						//更新
	void Draw(void);						//描画
	bool GetDamage(void) { return false; }	//ダメージ受けるか
	void Damage(int) { ; }			//ダメージ処理

	static HRESULT Load(void);	//テクスチャ読み込み
	static void UnLoad(void);	//テクスチャ破棄
	static CThundereffect *Create(D3DXVECTOR3 pos);	//作成

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
};

#endif