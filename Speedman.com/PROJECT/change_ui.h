//*****************************************************************************
// UI（何かしら変化する）関連処理(Change_UI.h)
// Author : satou syusuke
//*****************************************************************************
#ifndef _CHANGE_UI_H_
#define _CHANGE_UI_H_

#include "UI.h"
class CChangeUI : public CUI
{
public:
	//チェンジ型のUIの動き
	typedef enum
	{
		CHANGE_NONE,		//変化なし
		CHANGE_COUNTDOWN,	//カウントダウン
		CHANGE_TARGETMOVE,	//目標に向けて移動
		CHANGE_PAUSE,		//ポーズ中に表示
		CHANGE_CONTROL,		//操作説明
		CHANGE_BOUNCE,		//バウンド

		CHANGE_MAX,
	} CHANGE_TYPE;

	CChangeUI(int nPriority);
	~CChangeUI();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 Size, D3DXVECTOR2 MinSize, int nType, D3DCOLORVALUE color, D3DCOLORVALUE MinColor, 
		CChangeUI::CHANGE_TYPE ChangeType, int nLife, D3DXVECTOR2 Nextpos, D3DXVECTOR2 move);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CChangeUI *Create(D3DXVECTOR3 pos, D3DXVECTOR2 Size, D3DXVECTOR2 MinSize, int nType, D3DCOLORVALUE color, D3DCOLORVALUE MinColor, 
		CChangeUI::CHANGE_TYPE ChangeType, int nLife, D3DXVECTOR2 Nextpos, D3DXVECTOR2 move);	//作成

	static void SetCountDown();	//カウントダウン設定


private:

	D3DXVECTOR3 m_pos;			//現在位置

	CHANGE_TYPE m_aChangeType;	//どの動きをするか
	int m_nLife;				//UIの表示時間

	D3DXVECTOR2 m_nextpos;		//次の移動先
	D3DXVECTOR2 m_move;			//移動量でどの角度か
	D3DXVECTOR2 m_moveSpeed;	//移動量
	D3DXVECTOR2 m_Distanse;		//角度計算用

	D3DXVECTOR2 m_Size;			//大きさ
	D3DXVECTOR2 m_MinSize;		//大きさ増減

	D3DCOLORVALUE m_Color;			//カラー
	D3DCOLORVALUE m_MinColor;		//カラー増減

	bool m_bUninit;					//抹消

	float m_fAngle;					//角度

	static int m_nCountDown;		//カウント

	int m_nCntBounce;				//バウンド回数
};

#endif
