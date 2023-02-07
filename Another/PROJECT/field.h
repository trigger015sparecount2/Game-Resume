//=============================================================================
//
// シーン処理 [field.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"
#include "plane.h"

class CField : public CPlane
{
public:
	CField(PRIORITY Priority);
	~CField();
	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR2 Tex, bool bRotX);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	OBJTYPE GetObjType() { return OBJECTTYPE_FIELD; }	//種類
	//D3DXVECTOR3 GetPos() { return m_pos; }
	//float GetRadius() { return 0.0f; }
	//COLLISION GetCollision() { return COLLISION_SPHERE; }
	D3DXVECTOR3 GetVtxMax() { return m_VtxMax; }		//最大
	D3DXVECTOR3 GetVtxMin() { return m_VtxMin; }		//最小
	//void Damage(int, D3DXVECTOR3, float) { ; }
	bool GetRotX() { return m_bRotX; }					//傾く方向

	static CField *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos, int nTex, D3DXVECTOR2 Tex, bool bRotX);	//作成

private:
	//D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_VtxMax;	//最大
	D3DXVECTOR3 m_VtxMin;	//最小
	bool m_bRotX;			//傾く方向
};

#endif