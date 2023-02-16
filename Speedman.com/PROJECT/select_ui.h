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

	//UIの種類
	typedef enum
	{
		SELECT_UI_NOMAL = 0,
		SELECT_UI_CONTROL,
		SELECT_UI_MAX
	} SELECT_UI_TYPE;

	CSelectUI(int nPriority);
	~CSelectUI();

	HRESULT Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, int SelectNum, SELECT_UI_TYPE UIType);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CSelectUI *Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, int SelectNum, SELECT_UI_TYPE UIType);	//作成

	static void SetPauseSelect(int nPause) { m_nPauseSelect = nPause; }	//ポーズ番号変更

private:
	D3DXVECTOR3 m_pos;		//現在位置
	D3DCOLORVALUE m_color;	//色
	int m_UINum;			//UI番号

	//大きさ情報
	float m_fSizeX;
	float m_fSizeY;

	bool m_bUninit;//粛清フラグ

	static CKeyboard *m_pKeyboard;	//キーボード
	static int m_nPauseSelect;		//ポーズ中のセレクト番号

	static bool m_bFade;			//フェード中か
	
	SELECT_UI_TYPE m_nType;			//UIの種類
};

#endif //_SELECT_UI_H_