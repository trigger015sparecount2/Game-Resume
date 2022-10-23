//=============================================================================
//
// ロックオン処理 [rockon.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _ROCKON_H_
#define _ROCKON_H_

#include "main.h"
#include "ui.h"

class CRockon : public CUI
{
public:
	CRockon(PRIORITY Priority);
	~CRockon();
	HRESULT Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	void SetRockon(CScene *pScene);
	void RemoveRockon(void);
	bool GetRockon() { return m_bScreen; }

	CScene *GetRockEnemy() { return m_pScene; }

	static CRockon *Create();

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	CScene *m_pScene;
	float m_fHeight;
	bool m_bUse;
	bool m_bScreen;
};

#endif