//=============================================================================
//
// リザルトのUI処理 [resultui.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _RESULTUI_H_
#define _RESULTUI_H_
#include "main.h"
#include "scene.h"

class CScene2D;
class CInputKeyboard;
class CInputGamePad;

class CResultUi : public CScene
{
public:
	CResultUi(PRIORITY Priority = PRIORITY_UI);
	~CResultUi();
	HRESULT Init(float, float, D3DXVECTOR3, float);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画
	bool GetDamage(void) { return false; }
	void Damage(int) { ; }

	static HRESULT Load(void);			//テクスチャ読み込み
	static void UnLoad(void);			//テクスチャ破棄
	static CResultUi *Create(void);		//作成

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[12];	//テクスチャへのポインタ
	static CScene2D *m_apScene[100];			//シーン
	CInputKeyboard *m_pInputKeyboard;			//キーボードのポインタ
	CInputGamePad *m_pInputGamePad;				//ゲームパッドのポインタ
	int m_nTime;		//時間
	bool m_bClear;		//点滅時、次点けるか消すか
	float m_fClear;		//文字列の濃さ
	int m_nClearTime;	//順位の点滅の時間
	int m_nKillRank;	//今回の撃墜数のランキング順位
	int m_nScoreRank;	//今回のスコアのランキング順位
	bool m_bRankClear;	//順位の点滅、次点けるか消すか
};

#endif