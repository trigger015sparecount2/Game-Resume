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

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR color, D3DXCOLOR Mincolor, D3DXVECTOR3 Size, int nLife, int nType, D3DXVECTOR3 Move, D3DXVECTOR3 Rot);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CParticle *Create(D3DXVECTOR3 pos, D3DXCOLOR color, D3DXCOLOR Mincolor, D3DXVECTOR3 Size, int nLife, int nType, D3DXVECTOR3 Move, D3DXVECTOR3 Rot);	//作成

protected:

private:
	//カラー値
	D3DXCOLOR m_Color;

	//減らすカラー値
	D3DXCOLOR m_MinColor;
	D3DXVECTOR3 m_Rot;	//方向
	D3DXVECTOR3 m_Move;	//移動量
	int m_nLife;		//寿命
	int m_nType;		//種類
};
#endif
