//*****************************************************************************
// エフェクト処理 [particle.h]
// Author : 三上航世
//*****************************************************************************

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "plane.h"

class CParticle : public CPlane
{
public:
	CParticle(CScene::PRIORITY PRIORITY_EFFECT);
	~CParticle();

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR color, D3DXCOLOR Mincolor, D3DXVECTOR3 Size, int nLife, int nType, D3DXVECTOR3 Move, D3DXVECTOR3 Rot);
	void Uninit();
	void Update();
	void Draw();

	static CParticle *Create(D3DXVECTOR3 pos, D3DXCOLOR color, D3DXCOLOR Mincolor, D3DXVECTOR3 Size, int nLife, int nType, D3DXVECTOR3 Move, D3DXVECTOR3 Rot);

protected:

private:
	//カラー値
	D3DXCOLOR m_Color;

	//減らすカラー値
	D3DXCOLOR m_MinColor;
	D3DXVECTOR3 m_Rot;
	D3DXVECTOR3 m_Move;
	int m_nLife;
	int m_nType;
};
#endif
