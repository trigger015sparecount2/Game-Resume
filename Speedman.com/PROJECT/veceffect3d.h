//=============================================================================
// (直線距離に応じて消す)3Dエフェクト処理 [VecEffect3D.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _VECEFFECT3D_H_
#define _VECEFFECT3D_H_

#include "main.h"
#include "billEffect.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CCamera;

class CVecEffect3D : public CBillEffect
{
public:
	CVecEffect3D(int nPriority);
	~CVecEffect3D();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 Size, D3DXVECTOR3 MinSize, D3DXVECTOR3 move, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, int nType, int nLife);
	void Uninit();
	void Update();
	void Draw();

	static CVecEffect3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 Size, D3DXVECTOR3 MinSize, D3DXVECTOR3 move, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, int nType, int nLife);
private:

	D3DXVECTOR3 m_FastPos;	//出現地点
	D3DXVECTOR3 m_move;		//移動値
	D3DXVECTOR3 m_rot;		//角度

	float m_Vec;			//出現地点からの直線距離

};

#endif