//*****************************************************************************
// UI関連処理(UI.h)
// Author : 佐藤秀亮
//*****************************************************************************

#ifndef _UI_H_
#define _UI_H_

#include "Scene2D.h"

class CUI :public CScene2D
{
public:

	CUI(PRIORITY Priority);
	~CUI();

	HRESULT Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, D3DXCOLOR col);
	void Uninit();
	void Update();
	void Draw();
	OBJTYPE GetObjType() { return OBJECTTYPE_UI; }
	/*D3DXVECTOR3 GetPos() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	float GetRadius() { return 0.0f; }
	COLLISION GetCollision() { return COLLISION_SPHERE; }
	D3DXVECTOR3 GetVtxMax() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	D3DXVECTOR3 GetVtxMin() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	void Damage(int, D3DXVECTOR3, float);*/

	static CUI *Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, D3DXCOLOR col);
private:

	D3DXVECTOR3 m_pos;			//現在位置

	D3DCOLORVALUE m_Color;		//カラー
	bool m_bUninit;				//消すか


	//大きさ情報
	float m_fSizeX;
	float m_fSizeY;

};
#endif