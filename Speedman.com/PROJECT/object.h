//=============================================================================
//
// オブジェクト処理 [object.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"
#include "scene3d.h"

class CModel;

class CObject : public CScene3D
{
public:
	CObject(int nPriority);
	~CObject();
	HRESULT Init(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot, int nMoveTime, D3DXVECTOR3 move, bool bDrop);
	void Uninit();
	void Update();
	void Draw();

	static CObject *Create(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot, int nMoveTime, D3DXVECTOR3 move, bool bDrop);
	static void Load(int nCnt, const char *aModelName);
	static void UnLoad();

private:
	bool Land(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);

	CModel *m_pModel;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_MaxMove;
	D3DXVECTOR3 m_pos;
	int m_nTime;
	int m_nMoveTime;
	bool m_bPlus;
	bool m_bDrop[2];

	static CModel *m_paModel[8];
};

#endif