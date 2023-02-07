//=============================================================================
//
// 影処理 [shadow.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "scene2d.h"

class CShadow : public CScene2D
{
public:
	CShadow(PRIORITY Priority = PRIORITY_BG);
	~CShadow();
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画
	bool GetDamage(void) { return false; }	//ダメージ受けるか
	void Damage(int) { ; }			//ダメージ処理

	static HRESULT Load(void);	//テクスチャ読み込み
	static void UnLoad(void);	//テクスチャ破棄
	static CShadow *Create(D3DXVECTOR3 pos, float fWidth, float fHeight, SHADOW shadow);	//作成

private:
	float m_fWidth;		//横幅
	float m_fHeight;	//縦幅
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_SHADOW];	//テクスチャ
	bool m_bUninit;		//消すか
};

#endif