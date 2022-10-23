//=============================================================================
//
// オブジェクト処理 [object.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"
#include "model.h"
#include "scene3d.h"

class CObject : public CScene3D
{
public:
	CObject(PRIORITY Priority);
	~CObject();
	HRESULT Init(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot, int nMoveTime, D3DXVECTOR3 move, bool bDrop);
	void Uninit();
	void Update();
	void Draw();
	OBJTYPE GetObjType() { return OBJECTTYPE_BLOCK; }
	D3DXVECTOR3 GetPos() { return m_pos; }
	float GetRadius() { return m_fRadius; }
	COLLISION GetCollision() { return m_Collision; }
	D3DXVECTOR3 GetVtxMax() { return m_pModel->GetMaxSize(); }
	D3DXVECTOR3 GetVtxMin() { return m_pModel->GetMinSize(); }
	//void Damage(int, D3DXVECTOR3, float);

	static CObject *Create(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot, int nMoveTime, D3DXVECTOR3 move, bool bDrop);
	static void Load(int nCnt, const char *aModelName);
	static void UnLoad();

private:
	bool Land(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);

	CModel *m_pModel;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_MaxMove;
	D3DXVECTOR3 m_pos;
	CScene::COLLISION m_Collision;
	float m_fRadius;
	int m_nTime;
	int m_nMoveTime;
	bool m_bPlus;
	bool m_bDrop[2];

	static CModel *m_paModel[8];
};

#endif