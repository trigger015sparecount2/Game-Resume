//=============================================================================
// (直線)3Dエフェクト処理 [Straight3D.h]
// Author : 佐藤秀亮
// 10/24 引数が多す(ｒｙ
//=============================================================================
#ifndef _STRAIGHT3D_H_
#define _STRAIGHT3D_H_

#include "main.h"
#include "billEffect.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CCamera;

class CStraight3D : public CBillEffect
{
public:
	CStraight3D(int nPriority);
	~CStraight3D();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 Size, D3DXVECTOR3 MinSize, D3DXVECTOR3 move, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, int nType, int nLife);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CStraight3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 Size, D3DXVECTOR3 MinSize, D3DXVECTOR3 move, 
		D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, int nType, int nLife);	//作成
private:

	D3DXVECTOR3 m_move;	//移動値
	D3DXVECTOR3 m_rot;	//方向

	CCamera *m_pCamera;	//カメラ

};

#endif