//=============================================================================
//
// チュートリアルシーン [tutorial.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "scene.h"

class CInputKeyboard;
class CInputGamePad;

class CTutorial : public CScene
{
public:
	CTutorial(PRIORITY Priority = PRIORITY_BG);
	~CTutorial();

	HRESULT Init(float, float, D3DXVECTOR3, float);	//初期化
	void Uninit(void);						//終了
	void Update(void);						//更新
	void Draw(void);						//描画
	bool GetDamage(void) { return false; }	//ダメージ受けるか
	void Damage(int) { ; }					//ダメージ処理

private:
	int m_nTime;						//時間
	CInputKeyboard *m_pInputKeyboard;	//キーボードのポインタ
	CInputGamePad *m_pInputGamePad;		//ゲームパッドのポインタ
};
#endif