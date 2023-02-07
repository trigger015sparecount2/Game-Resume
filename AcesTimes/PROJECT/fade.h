//=============================================================================
//
// フェード処理 [fade.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_
#include "main.h"
#include "manager.h"

class CFade
{
public:
	//フェードの種類
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
		FADE_MAX
	} FADE;

	CFade();
	~CFade();
	HRESULT Init(void);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画
	static FADE GetFade(void) { return m_fade; }	//フェードの種類返す
	static void SetFade(CManager::MODE modenext);	//フェードをセットする

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	//頂点バッファへのポインタ
	static CManager::MODE m_mode;				//次にロードするモード
	D3DXCOLOR m_FadeColor;						//カラー
	static FADE m_fade;							//フェードの種類
};
#endif