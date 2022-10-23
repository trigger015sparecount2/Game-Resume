//---------------------------
//Author:三上航世
//移動関連(move.cpp)
//---------------------------
#include "move.h"

//移動速度
void Move(D3DXVECTOR3 *pMove, float fMove, float fRot)
{
	pMove->x -= (sinf(fRot) * fMove + pMove->x) * 0.1f;
	pMove->z -= (cosf(fRot) * fMove + pMove->z) * 0.1f;
}

//移動速度低下横
void Slow(D3DXVECTOR3 *pMove)
{
	pMove->x -= pMove->x * 0.1f;
	pMove->z -= pMove->z * 0.1f;
}