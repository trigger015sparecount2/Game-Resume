//=============================================================================
//
// ゴール処理 [goal.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _GOAL_H_
#define _GOAL_H_

#include "main.h"
#include "plane.h"

class CGoal : public CPlane
{
public:
	CGoal(int nPriority);
	~CGoal();

	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画
	static CGoal *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos);	//作成

private:
	
};

#endif