//=============================================================================
//
// リザルトシーン [result.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "scene.h"

//前方宣言

class CScene2D;

class CResult : public CScene
{
public:
	CResult(PRIORITY Priority = PRIORITY_BG);
	~CResult();

	HRESULT Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);
};
#endif