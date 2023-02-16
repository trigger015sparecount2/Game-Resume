//=============================================================================
//
// 壁処理 [wall.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"
#include "plane.h"

class CWall : public CPlane
{
public:
	CWall(int nPriority);
	~CWall();

	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR2 Tex, D3DXVECTOR3 rot);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CWall *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos, int nTex, D3DXVECTOR2 Tex, D3DXVECTOR3 rot);	//作成

private:

};

#endif