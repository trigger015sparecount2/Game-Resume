//*****************************************************************************
// UI（エフェクト）関連処理(Effect_UI.h)
// Author : 佐藤秀亮
// 10/24 従来のui.cppが膨れ上がったので細分化しました
//*****************************************************************************
#ifndef _EFFECT_UI_H_
#define _EFFECT_UI_H_

#include "UI.h"

class CEffectUI : public CUI
{
public:

	//なんのエフェクトを出すか
	typedef enum
	{
		EFFECT_NONE,		//エフェクト出さない
		EFFECT_FIRE,		//炎パーティクル
		EFFECT_CYCLE,		//輪っかエフェクト
		EFFECT_RANKCYCLE,	//ランキング用輪っかエフェクト
		EFFECT_PETAL,		//吹雪

		EFFECT_MAX,
	} EFFECT_TYPE;

	CEffectUI(int nPriority);
	~CEffectUI();

	HRESULT Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, CEffectUI::EFFECT_TYPE EffectType);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CEffectUI *Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, CEffectUI::EFFECT_TYPE EffectType);	//作成

private:
	EFFECT_TYPE m_aEffectType;	//出すエフェクト
	D3DXVECTOR3 m_pos;			//現在位置

	int m_cooltime;				//クールタイム
};

#endif //_EFFECT_UI_H_