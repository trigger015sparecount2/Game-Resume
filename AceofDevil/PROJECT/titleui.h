//=============================================================================
//
// タイトルのUI処理 [titleui.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _TITLEUI_H_
#define _TITLEUI_H_
#include "main.h"
#include "scene.h"

class CScene2D;
class CTitleUi : public CScene
{
public:
	CTitleUi(PRIORITY Priority = PRIORITY_UI);
	~CTitleUi();
	HRESULT Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize);		//初期化
	void Uninit(void);						//終了
	void Update(void);						//更新
	void Draw(void);						//描画
	bool GetDamage(void) { return false; }	//ダメージ受けるか
	void Damage(int) { ; }					//ダメージ処理

	static HRESULT Load(void);		//テクスチャ読み込み
	static void UnLoad(void);		//テクスチャ破棄
	static CTitleUi *Create(void);	//作成

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[2];	//テクスチャへのポインタ
	static CScene2D *m_apScene[2];				//シーン
	bool m_bClear;								//点けるか消すか
	float m_fClear;								//UIの透明度
};

#endif