//*****************************************************************************
// (落ちる)エフェクト処理 [FallEffect.h]
// Author : 佐藤秀亮
//*****************************************************************************
#ifndef _FALLEFFECT_H_
#define _FALLEFFECT_H_

#include "Effect.h"

class CFallEffect : public CEffect
{
public:
	CFallEffect(int nPriority);
	~CFallEffect();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 move, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, D3DXVECTOR2 Size, D3DXVECTOR2 MinSize, int nLife, int nType, D3DXVECTOR2 AddMovement);
	void Uninit();
	void Update();
	void Draw();

	static CFallEffect*Create(D3DXVECTOR3 pos, D3DXVECTOR2 move, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, D3DXVECTOR2 Size, D3DXVECTOR2 MinSize, int nLife, int nType, D3DXVECTOR2 AddMovement);
private:
	D3DXVECTOR2 m_Size;			//大きさ
	D3DXVECTOR2 m_MinSize;		//大きさ変動

	int m_nLife;				//寿命
	D3DXVECTOR2 m_move;			//移動
	D3DXVECTOR2 m_Addpos;		//慣性

	bool m_bUninit;		//消すかどうか
};

#endif // !_FALLEFFECT_H_
