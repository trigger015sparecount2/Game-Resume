//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "scene2d.h"

class CEffect : public CScene2D
{
public:
	//カラータイプ
	typedef enum
	{
		COLORTYPE_WHITE = 0,
		COLORTYPE_RED,
		COLORTYPE_LIGHTBLUE,
		COLORTYPE_YELLOW,
		COLORTYPE_BLUE,
		COLORTYPE_GREEN,
		COLORTYPE_MAX
	} COLORTYPE;

	CEffect(PRIORITY Priority = PRIORITY_EFFECT);
	~CEffect();
	HRESULT Init(D3DXVECTOR3 pos, COLORTYPE colType);	//初期化処理
	void Uninit(void);									//終了処理
	void Update(void);									//更新処理
	void Draw(void);									//描画処理
	bool GetDamage(void) { return false; }
	void Damage(int) { ; }

	static HRESULT Load(void);									//テクスチャの読み込み
	static void UnLoad(void);									//テクスチャ破棄
	static CEffect *Create(D3DXVECTOR3 pos, COLORTYPE colType);	//作成

private:
	float m_fSize;							//サイズ
	D3DXCOLOR m_col;						//カラー
	COLORTYPE m_colType;					//カラータイプ
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
};

#endif