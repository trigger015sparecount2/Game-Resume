//=============================================================================
//
// タイトルシーン [title.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "scene.h"

//前方宣言
class CInputKeyboard;
class CInputGamePad;
class CScene2D;

class CTitle : public CScene
{
public:
	CTitle(PRIORITY Priority = PRIORITY_BG);
	~CTitle();

	HRESULT Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize);	//初期化
	void Uninit(void);						//終了
	void Update(void);						//更新
	void Draw(void);						//描画
	bool GetDamage(void) { return false; }	//ダメージ受けるか
	void Damage(int) { ; }					//ダメージ処理

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
	static CScene2D *m_pScene2D;			//シーン
	CInputKeyboard *m_pInputKeyboard;		//キーボードのポインタ
	CInputGamePad *m_pInputGamePad;			//ゲームパッドのポインタ
};
#endif