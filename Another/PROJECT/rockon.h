//=============================================================================
//
// ロックオン処理 [rockon.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _ROCKON_H_
#define _ROCKON_H_

#include "main.h"
#include "ui.h"

class CRockon : public CUI
{
public:
	CRockon(PRIORITY Priority);
	~CRockon();
	HRESULT Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画
	
	void SetRockon(CScene *pScene);			//ロックオンする
	void RemoveRockon(void);				//ロックオン解除
	bool GetRockon() { return m_bScreen; }	//映っているか

	CScene *GetRockEnemy() { return m_pScene; }	//ロックオンしてる敵取得

	static CRockon *Create();	//作成

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
	CScene *m_pScene;	//ロックオンする敵
	float m_fHeight;	//敵のposからどのぐらいの高さに描画するか
	bool m_bUse;		//使っているか
	bool m_bScreen;		//映るか
};

#endif