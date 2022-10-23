//=============================================================================
//
// 残像処理 [orbit.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _ORBIT_H_
#define _ORBIT_H_

#define MAX_ORBIT (64)

#include "main.h"
#include "scene3d.h"

class COrbit : public CScene
{
public:
	COrbit(PRIORITY Priority);
	~COrbit();
	HRESULT Init(D3DXVECTOR3);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	OBJTYPE GetObjType() { return OBJECTTYPE_NONE; }
	D3DXVECTOR3 GetPos() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	D3DXVECTOR3 GetRot() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	//void SetPos(D3DXVECTOR3) { ; }
	float GetRadius() { return 0.0f; }
	float GetHeight() { return 0.0f; }
	COLLISION GetCollision() { return COLLISION_SPHERE; }
	D3DXVECTOR3 GetVtxMax() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	D3DXVECTOR3 GetVtxMin() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	void Damage(int, D3DXVECTOR3) { ; }
	bool GetRotX() { return false; }
	bool CollisionDamage(D3DXVECTOR3, float, float) { return false; }
	bool SetEnemy(CScene *pEnemy) { return false; }
	void SetOrbit(D3DXVECTOR3 pos, D3DXVECTOR3 Tippos, float fDis, D3DXCOLOR col1, D3DXCOLOR col2);
	int Damage() { return 0; }
	float HClash() { return 0.0f; }
	float VClash() { return 0.0f; }
	void SetParticle() { ; }
	bool GetAttack() { return false; }
	void Reflect() { ; }
	int GetHP() { return 0; }
	int GetMaxHP() { return 0; }
	D3DXMATRIX GetMatrix(int) { return m_mtxWorld; }
	static COrbit *Create(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL; //頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff = NULL;
	D3DXMATRIX m_mtxWorld; //ワールドマトリックス

	int m_nNumOrbit;
	int m_nStart;
	int m_nCnt;
	float m_fDis[MAX_ORBIT];
	D3DXCOLOR m_col[MAX_ORBIT];
};

#endif