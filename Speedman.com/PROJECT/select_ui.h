//*****************************************************************************
// UI（選択式）関連処理(Select_UI.h)
// Author : 佐藤秀亮
//
//UIクラスが肥大化したため処理によるクラス分けを行いました
//*****************************************************************************
#ifndef _SELECT_UI_H_
#define _SELECT_UI_H_

#include "UI.h"
#include "manager.h"

class CSelectUI : public CUI
{
public:

	CSelectUI(int nPriority);
	~CSelectUI();

	typedef enum
	{
		SELECT_UI_NOMAL,
		SELECT_UI_CONTROL,

		SELECT_UI_MAX,
	} SELECT_UI_TYPE;

	HRESULT Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, int SelectNum, SELECT_UI_TYPE UIType);
	void Uninit();
	void Update();
	void Draw();

	static CSelectUI *Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, int SelectNum, SELECT_UI_TYPE UIType);

	static void SetPauseSelect(int nPause);

private:
	D3DXVECTOR3 m_pos;			//現在位置
	D3DCOLORVALUE m_color;
	int m_UINum;

	//大きさ情報
	float m_fSizeX;
	float m_fSizeY;

	bool m_bUninit;//粛清フラグ

	static CKeyboard *m_pKeyboard;
	static int m_PauseSelect;		//ポーズ中のセレクト番号

	static bool m_bFade;

	SELECT_UI_TYPE m_nType;
};

#endif //_SELECT_UI_H_