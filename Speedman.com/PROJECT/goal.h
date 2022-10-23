//=============================================================================
//
// ÉSÅ[Éãèàóù [goal.h]
// Author : éOè„çqê¢
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
	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();
	static CGoal *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos);

private:
	
};

#endif