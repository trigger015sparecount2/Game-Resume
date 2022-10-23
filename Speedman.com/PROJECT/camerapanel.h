//=============================================================================
//
// ÉJÉÅÉâà⁄ìÆèàóù [camerapanel.h]
// Author : éOè„çqê¢
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
	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR3 posV, D3DXVECTOR3 posR);
	void Uninit();
	void Update();
	void Draw();

	//CCameraPanel GetCamerapanel();

	static CCameraPanel *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR3 posV, D3DXVECTOR3 posR);

private:

};
#endif