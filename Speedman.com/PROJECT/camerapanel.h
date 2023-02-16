//=============================================================================
//
// カメラ移動処理 [camerapanel.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _CAMERAPANEL_H_
#define _CAMERAPANEL_H_

#include "main.h"
#include "plane.h"

class CCameraPanel : public CPlane
{
public:
	CCameraPanel(int nPriority);
	~CCameraPanel();
	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR3 posV, D3DXVECTOR3 posR);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	//CCameraPanel GetCamerapanel();

	static CCameraPanel *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR3 posV, D3DXVECTOR3 posR);	//作成

private:

};
#endif