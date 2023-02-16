//=============================================================================
// フェード[Fade.h]
// Author : satou syusuke
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_


#include "manager.h"
#include "Scene2D.h"


class CFade : public CScene2D
{
public:
	typedef enum
	{
		FADE_NONE,	//何もしない
		FADE_IN,	//フェードイン
		FADE_OUT,	//フェードアウト
	} FADE;

	CFade(int nPriority);
	~CFade();

	HRESULT Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CFade *Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType);	//作成

	//フェード設定
	static void SetFade(CManager::MODE ModeNext);
	static FADE GetFade() { return m_afade; }

private:
	static FADE m_afade;				//フェード
	static CManager::MODE m_modenext;	//次のモード
	static int m_ncolorFade;			//色

	static bool m_bUninit;				//終了

};

#endif