//=============================================================================
//
// ポーズ中UI処理 [pauseui.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _PAUSEUI_H_
#define _PAUSEUI_H_

#include "main.h"
#include "scene.h"

#define MAX_PAUSEDATA (7)

class CScene2D;
class CInputGamePad;
class CInputKeyboard;

class CPauseUi : public CScene
{
public:
	CPauseUi(PRIORITY Priority = PRIORITY_UI);
	~CPauseUi();
	HRESULT Init(float, float, D3DXVECTOR3, float);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画

	bool GetDamage(void) { return false; }
	void Damage(int) { ; }
	void Pause(void);		//ポーズする
	void PauseLift(void);	//ポーズ解除

	static HRESULT Load(void);		//テクスチャ読み込み
	static void UnLoad(void);		//テクスチャ破棄
	static CPauseUi *Create(void);	//作成
	static void SetPause(bool bPause) { m_bUse = bPause; }	//ポーズするか
	static bool GetPause(void) { return m_bUse; }			//ポーズ中か

private:
	CInputGamePad *m_pInputGamePad;				//ゲームパッドのポインタ
	CInputKeyboard *m_pInputKeyboard;			//キーボードのポインタ
	static LPDIRECT3DTEXTURE9 m_pTexture[6];	//テクスチャへのポインタ
	static CScene2D *m_apScene[MAX_PAUSEDATA];	//UI分のシーン
	static bool m_bUse;							//ポーズ中か
	int m_nSelect;								//何選んでるか
};

#endif