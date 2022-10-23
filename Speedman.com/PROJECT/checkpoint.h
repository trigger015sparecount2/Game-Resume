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
	HRESULT Init(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot);
	void Uninit();
	void Update();
	void Draw();

	static CCheckPoint *Create(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot);
private:

};

#endif