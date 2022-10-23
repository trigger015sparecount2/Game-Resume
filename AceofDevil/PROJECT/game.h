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

	HRESULT Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);

private:
	int m_nTime;
};
#endif