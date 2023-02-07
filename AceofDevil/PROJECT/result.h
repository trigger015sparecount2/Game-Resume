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

	HRESULT Init(float, float, D3DXVECTOR3, float);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画
	bool GetDamage(void) { return false; }
	void Damage(int) { ; }
};
#endif