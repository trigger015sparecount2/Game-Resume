//=============================================================================
//
// チェックポイント処理 [checkpoint.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _CHECKPOINT_H_
#define _CHECKPOINT_H_

#include "main.h"
#include "object.h"

class CModel;

class CCheckPoint : public CObject
{
public:
	CCheckPoint(int nPriority);
	~CCheckPoint();
	HRESULT Init(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CCheckPoint *Create(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot);	//作成
private:

};

#endif