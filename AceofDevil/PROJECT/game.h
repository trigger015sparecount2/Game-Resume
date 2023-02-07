//=============================================================================
//
// ゲームシーン [game.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "scene.h"

class CGame : public CScene
{
public:
	CGame(PRIORITY Priority = PRIORITY_BG);
	~CGame();

	HRESULT Init(float, float, D3DXVECTOR3, float);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画
	bool GetDamage(void) { return false; }
	void Damage(int) { ; }

private:
	int m_nTime;	//時間
};
#endif