//=============================================================================
//
// âeèàóù [shadow.h]
// Author : éOè„çqê¢
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "plane.h"

class CShadow : public CPlane
{
public:
	CShadow(int nPriority);
	~CShadow();
	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();
	void Move(float fHeight);
	void MoveY(D3DXVECTOR3 pos, float fRotX);
	
	static CShadow *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos);

private:
	float m_fLength;
	float m_fPosY;
	D3DXVECTOR3 m_size;
};

#endif