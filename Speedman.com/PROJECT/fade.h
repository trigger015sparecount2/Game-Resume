//=============================================================================
// フェード[Fade.h]
// Author : satou syusuke
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_


#include "manager.h"
#include "Scene2D.h"


class CFade : public CScene2D
{
public:

	typedef enum
	{
		FADE_NONE,
		FADE_IN,
		FADE_OUT,
	} FADE;

	CFade(int nPriority);
	~CFade();

	HRESULT Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType);
	void Uninit();
	void Update();
	void Draw();

	static CFade *Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType);

	static void SetFade(CManager::MODE ModeNext);
	static FADE GetFade();

private:
	static FADE m_afade;
	static CManager::MODE m_modenext;
	static int m_ncolorFade;

	static bool m_bUninit;

};

#endif