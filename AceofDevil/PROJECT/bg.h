//=============================================================================
//
// 背景処理 [bg.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_
#include "main.h"
#include "scene.h"
class CScene2D;
class CBg : public CScene
{
public:
	CBg(PRIORITY Priority = PRIORITY_BG);
	~CBg();
	HRESULT Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画
	bool GetDamage(void) { return false; }
	void Damage(int) { ; }
	
	static HRESULT Load(void);			//テクスチャ読み込み
	static void UnLoad(void);			//テクスチャ破棄
	static CBg *Create(bool bScroll);	//作成

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャへのポインタ
	CScene2D *m_apScene;					//シーン
	float m_fTexY;							//どれだけスクロールさせているか
	static bool m_bScroll;					//スクロールさせるか
};

#endif