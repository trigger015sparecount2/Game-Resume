//*****************************************************************************
// (終了時に何かある)エフェクト処理 [EndEffect.h]
// Author : 佐藤秀亮
//*****************************************************************************
#ifndef _ENDEFFECT_H_
#define _ENDEFFECT_H_

#include "Effect.h"

class CEndEffect : public CEffect
{
public:

	typedef enum
	{
		TYPE_FIREWORK,	//花火

		TYPE_MAX,
	} TYPE_ENDEFFECT;

	CEndEffect(int nPriority);
	~CEndEffect();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 move, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, D3DXVECTOR2 Size,
		int nLife, int nType, CEndEffect::TYPE_ENDEFFECT atype);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CEndEffect*Create(D3DXVECTOR3 pos, D3DXVECTOR2 move, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, D3DXVECTOR2 Size,
		int nLife, int nType, CEndEffect::TYPE_ENDEFFECT atype);	//作成
private:

	TYPE_ENDEFFECT m_nType;		//出す奴
	D3DXVECTOR2 m_Size;			//大きさ

	int m_nLife;				//寿命
	D3DXVECTOR2 m_move;			//移動

	bool m_bUninit;		//消すかどうか
};

#endif // !_FALLEFFECT_H_
