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

	HRESULT Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);

private:
	int m_nTime;
	CInputKeyboard *m_pInputKeyboard;
	CInputGamePad *m_pInputGamePad;
};
#endif