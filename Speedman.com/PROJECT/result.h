//=============================================================================
//ÉäÉUÉãÉgä«óù[Result.h]
// Author : satou syusuke
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "manager.h"
#include "Scene.h"
//*****************************************************************************
//ëOï˚êÈåæ
//*****************************************************************************
class CGamePad;

class CResult : public CScene
{
public:
	CResult();
	~CResult();

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();

	static CResult *Create();

private:
	CKeyboard *m_pKeyboard;
	CGamePad *m_pGamePad;

	static bool m_bFade;
};

#endif // _RESULT_H_
